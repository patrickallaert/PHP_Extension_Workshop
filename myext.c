#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myext.h"
#include "Zend/zend.h"
#include "Zend/zend_hash.h"

ZEND_BEGIN_ARG_INFO(arginfo_inspect, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO();

zend_function_entry myext_functions[] =
{
    ZEND_FE(inspect,arginfo_inspect)
    {NULL,NULL,NULL} /* Marks the end of function entries */
};


/* {{{ myext_module_entry
 */
zend_module_entry myext_module_entry = {
    STANDARD_MODULE_HEADER,
    "myext",
    myext_functions, /* Function entries */
    NULL, /* Module init */
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

PHP_FUNCTION(inspect)
{
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &value) == FAILURE)
    {
        RETURN_FALSE;
    }

    switch (Z_TYPE_P(value)) {
        case IS_NULL:
            php_printf("The variable passed is of type IS_NULL\n");
            break;

        case IS_LONG:
            php_printf("The variable passed is of type IS_LONG\n");
            php_printf("Computing %ld * 2: %ld\n", Z_LVAL_P(value), Z_LVAL_P(value) * 2 );
            break;

        case IS_DOUBLE:
            php_printf("The variable passed is of type IS_DOUBLE\n");
            php_printf("Computing %f * 0.5: %f\n", Z_DVAL_P(value), Z_DVAL_P(value) * 0.5 );
            break;

#if PHP_VERSION_ID >= 70000
        case IS_TRUE:
            php_printf("The variable passed is of type IS_TRUE\n");
            break;

        case IS_FALSE:
            php_printf("The variable passed is of type IS_FALSE\n");
            break;
#else
        case IS_BOOL:
            php_printf("The variable passed is of type IS_BOOL\n");
            break;
#endif

        case IS_ARRAY:
            php_printf("The variable passed is of type IS_ARRAY\n");
            php_printf("There is %ld direct elements in the array\n", zend_hash_num_elements(Z_ARRVAL_P(value)));
            break;

        case IS_OBJECT:
            php_printf("The variable passed is of type IS_OBJECT\n");
            break;

        case IS_STRING:
            php_printf("The variable passed is of type IS_STRING\n");
            php_printf("The content is \"%s\"\n", Z_STRVAL_P(value));
            php_printf("The content reversed is \"");
            char *p = Z_STRVAL_P(value) + Z_STRLEN_P(value);
            while (--p>=Z_STRVAL_P(value))
                php_printf("%c", *p);
            php_printf("\"\n");
            break;

        case IS_RESOURCE:
            php_printf("The variable passed is of type IS_RESOURCE\n");
            php_printf(
				"The number of the resource is: %ld\n",
#if PHP_VERSION_ID >= 70000
				Z_RES_HANDLE_P(value)
#else
				Z_RESVAL_P(value)
#endif
			);
            break;
    }
}
