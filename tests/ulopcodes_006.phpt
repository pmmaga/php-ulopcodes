--TEST--
Check using a function call as an argument
--INI--
ulopcodes.enabled = 1
ulopcodes.dump_oparray = 0
--FILE--
<?php 

function addTen($value) {
    $value = ulopcodes_emit(ZEND_ADD, $value, 10);
    ulopcodes_emit(ZEND_RETURN, $value);
}

ulopcodes_emit(ZEND_ASSIGN, $twenty, addTen(10));
ulopcodes_emit(ZEND_ECHO, $twenty);

?>
--EXPECT--
20
