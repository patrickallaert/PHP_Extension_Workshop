#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_myext.h"


/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    NULL, /* Function entries */
    PHP_MINIT(myext), /* Module init */
    PHP_MSHUTDOWN(myext), /* Module shutdown */
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

PHP_INI_BEGIN()
    PHP_INI_ENTRY("myext.pi", NULL, PHP_INI_SYSTEM, NULL)
    PHP_INI_ENTRY("myext.euler", NULL, PHP_INI_SYSTEM, NULL)
    PHP_INI_ENTRY("myext.golden", NULL, PHP_INI_SYSTEM, NULL)
PHP_INI_END()

PHP_MINIT_FUNCTION(myext)
{
    REGISTER_INI_ENTRIES();

    REGISTER_DOUBLE_CONSTANT("MYEXT_PI", INI_FLT("myext.pi"), CONST_CS | CONST_PERSISTENT);
    REGISTER_DOUBLE_CONSTANT("MYEXT_EULER", INI_FLT("myext.euler"), CONST_CS | CONST_PERSISTENT);
    REGISTER_DOUBLE_CONSTANT("MYEXT_GOLDEN", INI_FLT("myext.golden"), CONST_CS | CONST_PERSISTENT);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(myext)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}
