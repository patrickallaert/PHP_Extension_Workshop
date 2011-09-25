dnl config.m4 for myext

PHP_ARG_ENABLE(myext, whether to enable myext support,
[  --enable-myext          Enable myext support])

if test "$PHP_MYEXT" != "no"; then
    PHP_NEW_EXTENSION(myext, myext.c, $ext_shared)
fi
