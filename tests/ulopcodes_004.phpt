--TEST--
Check emitting a function call
--INI--
ulopcodes.enabled = true
--FILE--
<?php 

function getGreeting() {
    ulopcodes_emit(ZEND_RETURN, "Hello world!");
}

ulopcodes_emit(ZEND_INIT_FCALL, IS_UNUSED, "getgreeting");
$greeting = ulopcodes_emit(ZEND_DO_FCALL);

ulopcodes_emit(ZEND_ECHO, $greeting);

?>
--EXPECT--
Hello world!
