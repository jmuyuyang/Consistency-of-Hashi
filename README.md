Consistency of Hashi IN PHP EXTENSION
================= 
consistency of hashi written in c and built as a PHP extension
###install
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config
$make && make install

###Document
$consistent = new YConsistent();
####conhash init
$consistent->consistent_init();
####add servers
$consistent->add_server(host,port,weight£©   
paramters:   
char host  
int port   
int weight  
example:   
```
$consistent->add_server("192.168.1.1",11211,12);  
$consistent->add_server("192.168.1.2",11212,12);
```
####find server
paramters:  
char key   
example:  
```
$serverInfo = $consistent->find_server("yuyang");
```
return  
array server_info  
```
array(
	'host' => '192.168.1.1',
	'port' => '11211'
)
```