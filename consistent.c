/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_consistent.h"
#include "conhash.h"

/* If you declare any globals in php_consistent.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(consistent)
*/

/* True global resources - no need for thread safety here */
static int le_consistent;
static int le_conhash;
zend_class_entry *consistent_class_entry;
/* {{{ consistent_functions[]
 *
 * Every user visible function must have an entry in consistent_functions[].
 */

ZEND_METHOD(YConsistent,consistent_init){
	zval *object = getThis();
	conhash *con = conhash_init();
	int res_id = ZEND_REGISTER_RESOURCE(NULL,con,le_conhash);
	add_property_resource(object,"conhash",res_id);
}

ZEND_METHOD(YConsistent,add_server){
	char *host;
	int host_len,port,weight;
	zval *object = getThis();
	zval **conhash_prop;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sll",&host,&host_len,&port,&weight) == FAILURE){
		return;
	}
	if(zend_hash_find(Z_OBJPROP_P(object),"conhash",sizeof("conhash"),(void **)&conhash_prop) == FAILURE){
		RETURN_NULL();
	} 
	conhash *con = zend_list_find(Z_LVAL_PP(conhash_prop),&le_conhash);
	conhash_add_server(host,port,weight,con);
}

ZEND_METHOD(YConsistent,find_server){
	char *key;
	char *return_key;
	int key_len;
	zval *object = getThis();
	zval **conhash_prop;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&key,&key_len) == FAILURE){
		return;
	}
	if(zend_hash_find(Z_OBJPROP_P(object),"conhash",sizeof("conhash"),(void **)&conhash_prop) == FAILURE){
		RETURN_NULL();
	}
	conhash *con = zend_list_find(Z_LVAL_PP(conhash_prop),&le_conhash);
	cnode *node = conhash_find_server(key,key_len,con);
	array_init(return_value);
	add_assoc_string(return_value,"host",node->server.host,1);
	add_assoc_long(return_value,"port",node->server.port);
}

ZEND_METHOD(YConsistent,consistent_end){
	zval *object = getThis();
	zval **conhash_prop;
	if(zend_hash_find(Z_OBJPROP_P(object),"conhash",sizeof("conhash"),(void **)&conhash_prop) == FAILURE){
		RETURN_NULL();
	}
	if(zend_list_delete(Z_LVAL_PP(conhash_prop)) == FAILURE){
		RETURN_FALSE;
	}
	RETURN_TRUE;
}


const zend_function_entry consistent_functions[] = {
	PHP_FE(confirm_consistent_compiled,	NULL)		/* For testing, remove later. */
	{NULL,NULL,NULL}	/* Must be the last line in consistent_functions[] */
};

const zend_function_entry php_YConsistent_class_functions[] = {
	ZEND_ME(YConsistent,consistent_init,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME(YConsistent,add_server,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME(YConsistent,find_server,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME(YConsistent,consistent_end,NULL,ZEND_ACC_PUBLIC)		
};
/* }}} */

static void conhash_dtor( zend_rsrc_list_entry *rsrc TSRMLS_DC )
{
	conhash *con = (conhash *)rsrc->ptr;
	efree(con);
}

/* {{{ consistent_module_entry
 */
zend_module_entry consistent_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"consistent",
	consistent_functions,
	PHP_MINIT(consistent),
	PHP_MSHUTDOWN(consistent),
	PHP_RINIT(consistent),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(consistent),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(consistent),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CONSISTENT
ZEND_GET_MODULE(consistent)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("consistent.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_consistent_globals, consistent_globals)
    STD_PHP_INI_ENTRY("consistent.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_consistent_globals, consistent_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_consistent_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_consistent_init_globals(zend_consistent_globals *consistent_globals)
{
	consistent_globals->global_value = 0;
	consistent_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */

PHP_MINIT_FUNCTION(consistent)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry consistent_ce;
	INIT_CLASS_ENTRY(consistent_ce,"YConsistent",php_YConsistent_class_functions);
	consistent_class_entry = zend_register_internal_class(&consistent_ce TSRMLS_CC);
	le_conhash = zend_register_list_destructors_ex(conhash_dtor,NULL,"conhash class",module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(consistent)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(consistent)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(consistent)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(consistent)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "consistent support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_consistent_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_consistent_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "consistent", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
