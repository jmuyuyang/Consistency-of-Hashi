#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "conhash.h"
#include "php.h"
#include "ext/standard/crc32.h"
#include "php_consistent.h"

conhash *conhash_init(){
	conhash *con = (conhash *)emalloc(sizeof(conhash));
	con->server_nums = 0;
	con->points = 0;
	return con;
}

static int conhash_crc32_hash(const char *key,int key_len){
	unsigned int crc = ~0;
	int i;
	for (i=0; i<key_len; i++) {
		CRC32(crc, key[i]);
	}
  	return ~crc;
}

void conhash_add_server(char *host,int port,int weight,conhash *con){
	cnode *node = (cnode *)emalloc(sizeof(cnode));
	memcpy(node->server.host,host,22);
	node->server.port = port;
	node->server.weight = weight;
	vnode virtual_node;
	if(con->server_nums == 0){
		con->server = (vnode *)emalloc(weight*sizeof(vnode));
	}else{
		con->server = (vnode *)erealloc(con->server,(con->points+weight)*sizeof(vnode));
	}
	char *key = emalloc(strlen(host)+MAX_LENGTH_OF_LONG*2+2);
	int i;
	for(i = 0;i<weight;i++){
		spprintf(&key,0,"%s:%d-%03d",host,port,i);
	 	virtual_node.hash = conhash_crc32_hash(key,strlen(key));
	 	virtual_node.node = node;
	 	con->server[con->points+i] = virtual_node;
	}
	con->points+=weight;
	con->server_nums++;
	efree(key);
}

static int conhash_compare(const void *a,const void *b){
	if(((vnode *)a)->hash > ((vnode *)b)->hash){
		return 1;
	}
	if(((vnode *)a)->hash < ((vnode *)b)->hash){
		return -1;
	}
	return 0;
}

cnode *conhash_find_server(char *key,int key_len,conhash *con){
	long hash = conhash_crc32_hash(key,key_len);
	qsort((void *)con->server,con->points,sizeof(vnode),conhash_compare);
	int lo = 0;int hi = con->points-1;int mid = 0;
	if(con->server_nums > 1){
		while(1){
			if(hash < con->server[lo].hash || hash > con->server[hi].hash){
				return con->server[lo].node;
			}
			mid = lo + (hi-lo) / 2;
			if(hash < con->server[mid].hash && hash > (mid ? con->server[mid-1].hash : 0)){
				return con->server[mid].node;
			}
			if(hash > con->server[mid].hash){
				lo = mid + 1;
			}else{
				hi = mid - 1;
			}
		}
	}else{
		return con->server[0].node;
	}
}