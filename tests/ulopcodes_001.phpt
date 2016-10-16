--TEST--
Check for ulopcodes presence
--FILE--
<?php 
if (extension_loaded("ulopcodes")) {
    echo "ulopcodes extension is available";
}
?>
--EXPECT--
ulopcodes extension is available
