#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_myext.h"


/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    NULL, /* Function entries */
    PHP_MINIT(myext), /* Module init */
    NULL, /* Module shutdown */
    NULL, /* Request init */
    NULL, /* Request shutdown */
    NULL, /* Module information */
    "0.1", /* Replace with version number for your extension */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYEXT
ZEND_GET_MODULE(myext)
#endif

zend_class_entry *ce_Int;

static zend_object_handlers int_object_handlers;

ZEND_BEGIN_ARG_INFO_EX(arginfo_int___construct, 0, 0, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

static zend_function_entry int_class_functions[] = {
    PHP_ME( Int, __construct, arginfo_int___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR )
};

static void int_object_free(void *_object TSRMLS_DC)
{
    int_object *object = (int_object*)_object;

    zend_hash_destroy(object->zo.properties);
    FREE_HASHTABLE(object->zo.properties);

    zval_ptr_dtor(&object->value);

    efree(object);
}

static zend_object_value int_object_new(zend_class_entry *class_type TSRMLS_DC)
{
    zend_object_value retval;
    int_object *intern;

    intern = emalloc(sizeof(int_object));
    memset(intern, 0, sizeof(int_object));

    zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
    object_properties_init(&intern->zo, class_type);

    retval.handle = zend_objects_store_put(
        intern,
        (zend_objects_store_dtor_t) zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)int_object_free,
        NULL TSRMLS_CC
    );

    retval.handlers = &int_object_handlers;

    ALLOC_INIT_ZVAL(intern->value);

    return retval;
}

static int int_object_cast(zval *readobj, zval *writeobj, int type TSRMLS_DC)
{
    int_object *object = (int_object*)zend_object_store_get_object(readobj TSRMLS_CC);

    switch (type)
    {
        case IS_STRING:
        case IS_DOUBLE:
        case IS_LONG:
            ZVAL_ZVAL(writeobj, object->value, 1, 0);
            convert_to_explicit_type(writeobj, type);
            return SUCCESS;
    }

    return FAILURE;
}

PHP_MINIT_FUNCTION(myext)
{
    zend_class_entry ce;

    memcpy(&int_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    int_object_handlers.cast_object = int_object_cast;

    INIT_CLASS_ENTRY(ce, "Int", int_class_functions);
    ce.create_object = int_object_new;
    ce_Int = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}

PHP_METHOD( Int, __construct )
{
    zval *value;
    int_object *object;
    zend_error_handling error_handling;

    zend_replace_error_handling(EH_THROW, NULL, &error_handling TSRMLS_CC);

    if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &value ) == SUCCESS )
    {
        object = (int_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
        ZVAL_ZVAL(object->value, value, 1, 0);
        convert_to_long(object->value);
    }

    zend_restore_error_handling(&error_handling TSRMLS_CC);
}
