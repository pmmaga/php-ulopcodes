--TEST--
Check emitting an ECHO
--INI--
ulopcodes.enabled = true
--FILE--
<?php 
ulopcodes_emit(ZEND_ECHO, "echo emitted");
?>
--EXPECT--
echo emitted
