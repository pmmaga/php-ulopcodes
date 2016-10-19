--TEST--
Check emitting jumps
--INI--
ulopcodes.enabled = 1
ulopcodes.dump_oparray = 0
--FILE--
<?php 

$i = 0;
ulopcodes_emit(ZEND_JMP, 14);
ulopcodes_emit(ZEND_ECHO, $i++);
$res = ulopcodes_emit(ZEND_IS_SMALLER, $i, 10);
ulopcodes_emit(ZEND_JMPNZ, (bool)$res, 7);

?>
--EXPECT--
0123456789
