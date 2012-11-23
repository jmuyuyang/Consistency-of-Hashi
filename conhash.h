#ifndef _CONHASH_

#define _CONHASH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _server_info
{
	char host[22];
	int port;
	int weight;
} server_info;

typedef struct cnode
{
	server_info server;
} cnode;

typedef struct vnode
{
	long hash;
	cnode *node;
} vnode;

typedef struct _conhash
{
	int server_nums;
	int points;
	vnode *server;
} conhash;
conhash *conhash_init();
void conhash_add_server(char *host,int port,int weight,conhash *con);
static int conhash_compare(const void *a,const void *b);
cnode *conhash_find_server(char *key,int key_len,conhash *con);
static int conhash_crc32_hash(const char *key,int key_len);
#endif

