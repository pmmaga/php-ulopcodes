--TEST--
Check emitting a function call
--INI--
ulopcodes.enabled = 1
ulopcodes.dump_oparray = 0
--FILE--
<?php 

function getGreeting() {
    ulopcodes_emit(ZEND_RETURN, "Hello world!");
}

ulopcodes_emit(ZEND_INIT_USER_CALL, "getgreeting", "getgreeting", 0);
$greeting = ulopcodes_emit(ZEND_DO_UCALL);

ulopcodes_emit(ZEND_ECHO, $greeting);

?>
--EXPECT--
Hello world!
