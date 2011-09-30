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

zend_class_entry *ce_MyClass;

ZEND_BEGIN_ARG_INFO_EX(arginfo_myclass_foo, 0, 0, 1)
    ZEND_ARG_INFO(0, number)
    ZEND_ARG_INFO(1, string)
ZEND_END_ARG_INFO()

static zend_function_entry myclass_class_functions[] = {
    PHP_ME( MyClass, foo, arginfo_myclass_foo, ZEND_ACC_PUBLIC )
    PHP_FE_END
};

PHP_MINIT_FUNCTION(myext)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "MyClass", myclass_class_functions);
    ce.create_object = NULL;
    ce_MyClass = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}

PHP_METHOD(MyClass, foo)
{
    char *string;
	size_t string_len;
#if PHP_VERSION_ID >= 70000
	zend_long number;
#else
    long number;
#endif

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &number, &string, &string_len) == FAILURE)
    {
        return;
    }

    php_printf("Number: %ld, String: ", number);
    PHPWRITE(string, string_len);
}
