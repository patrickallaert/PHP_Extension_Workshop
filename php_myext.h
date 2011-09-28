typedef struct {
    zend_object zo;
    zval *value;
} int_object;

PHP_MINIT_FUNCTION(myext);
PHP_METHOD( Int, __construct );
