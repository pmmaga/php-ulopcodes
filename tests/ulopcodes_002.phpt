--TEST--
Check that ulopcodes_emit is available
--FILE--
<?php 
if (function_exists("ulopcodes_emit")) {
    echo "ulopcodes_emit() is available";
}
?>
--EXPECT--
ulopcodes_emit() is available
