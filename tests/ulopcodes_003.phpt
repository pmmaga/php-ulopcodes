--TEST--
Check emitting an ECHO
--INI--
ulopcodes.enabled = 1
ulopcodes.dump_oparray = 0
--FILE--
<?php 
ulopcodes_emit(ZEND_ECHO, "echo emitted");
?>
--EXPECT--
echo emitted
