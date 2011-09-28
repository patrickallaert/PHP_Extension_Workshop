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

#define LOG_ERROR 0
#define LOG_DEBUG 1
#define LOG_INFO 2

zend_class_entry *ce_Logger;

ZEND_BEGIN_ARG_INFO_EX(arginfo_logger___construct, 0, 0, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_logger_log, 0, 0, 2)
    ZEND_ARG_INFO(0, level)
    ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

static zend_function_entry logger_class_functions[] = {
    PHP_ME( Logger, __construct, arginfo_logger___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR )
    PHP_ME( Logger, log, arginfo_logger_log, ZEND_ACC_PUBLIC )
};

PHP_INI_BEGIN()
    PHP_INI_ENTRY("myext.default_file", "log.txt", PHP_INI_ALL, NULL)
PHP_INI_END()

PHP_MINIT_FUNCTION(myext)
{
    zend_class_entry ce;

    REGISTER_INI_ENTRIES();

    INIT_CLASS_ENTRY(ce, "Logger", logger_class_functions);
    ce.create_object = NULL;
    ce_Logger = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_string(ce_Logger, ZEND_STRL("file"), INI_STR("myext.default_file"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(ce_Logger, ZEND_STRL("handle"), ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_class_constant_long(ce_Logger, ZEND_STRL("INFO"), LOG_INFO TSRMLS_CC);
    zend_declare_class_constant_long(ce_Logger, ZEND_STRL("DEBUG"), LOG_DEBUG TSRMLS_CC);
    zend_declare_class_constant_long(ce_Logger, ZEND_STRL("ERROR"), LOG_ERROR TSRMLS_CC);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(myext)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_METHOD( Logger, __construct )
{
    char *file;
    int file_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &file, &file_len) == FAILURE)
    {
        zend_error(E_ERROR, "Wrong parameters for Logger([string $file])");
    }

    if (file) {
        zend_update_property_string(ce_Logger, getThis(), ZEND_STRL("file"), file TSRMLS_CC);
    }
}

PHP_METHOD( Logger, log )
{
    long level;
    char *message, *level_str;
    int message_len;
    zval *file, *object, *tmp;
    FILE *fd;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &level, &message, &message_len) == FAILURE)
    {
        zend_error(E_ERROR, "Wrong parameters for Logger->log(int $level, string $message)");
    }

    object = getThis();

    file = zend_read_property(ce_Logger, object, ZEND_STRL("handle"), 1);
    switch(Z_TYPE_P(file))
    {
        case IS_NULL:
            // Opening the file and storing the file descriptor as a resource
            ALLOC_ZVAL(tmp);
            Z_UNSET_ISREF_P(tmp);
            Z_SET_REFCOUNT_P(tmp, 0);
            fd = fopen( Z_STRVAL_P(zend_read_property(ce_Logger, object, ZEND_STRL("file"), 1)), "a");
            ZVAL_RESOURCE(tmp, (long)fd);
            zend_update_property(ce_Logger, object, ZEND_STRL("handle"), tmp TSRMLS_CC);
            break;
        case IS_RESOURCE:
            // File descriptor already known in resource property
            fd = (FILE *)Z_RESVAL_P(file);
            break;
        default:
            zend_error(E_ERROR, "Something really wrong happened here, type of 'file' property cannot be something else than NULL or a resource.");
    }
    switch(level)
    {
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        case LOG_DEBUG:
            level_str = "DEBUG";
            break;
        case LOG_INFO:
            level_str = "INFO";
            break;
        default:
            level_str = "UNKNOWN";
    }

    fprintf(fd, "%s: %s\n", level_str, message);
}
