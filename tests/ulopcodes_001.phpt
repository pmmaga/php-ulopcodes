--TEST--
Check for ulopcodes presence
--SKIPIF--
<?php if (!extension_loaded("ulopcodes")) print "skip"; ?>
--FILE--
<?php 
echo "ulopcodes extension is available";
?>
--EXPECT--
ulopcodes extension is available
