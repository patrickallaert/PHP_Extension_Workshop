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

PHP_MINIT_FUNCTION(myext)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "MyClass", NULL);
    ce.create_object = NULL;
    ce_MyClass = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_null(
        ce_MyClass, ZEND_STRL("aPublicProperty"), ZEND_ACC_PUBLIC TSRMLS_CC
    );
    zend_declare_property_string(
        ce_MyClass, ZEND_STRL("aProtectedProperty"), "I am protected", ZEND_ACC_PROTECTED TSRMLS_CC
    );
    zend_declare_property_string(
        ce_MyClass, ZEND_STRL("aPrivateProperty"), "I am private", ZEND_ACC_PROTECTED TSRMLS_CC
    );
    zend_declare_property_long(
        ce_MyClass, ZEND_STRL("aStaticPublicProperty"), 0, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC TSRMLS_CC
    );

    return SUCCESS;
}
