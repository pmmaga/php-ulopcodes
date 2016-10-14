PHP_ARG_ENABLE(ulopcodes, whether to enable ulopcodes support,
[  --enable-ulopcodes           Enable ulopcodes support])

if test "$PHP_ULOPCODES" != "no"; then
  PHP_NEW_EXTENSION(ulopcodes, ulopcodes.c, yes,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
