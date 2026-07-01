<?php
error_reporting(E_ALL);
ini_set('display_errors', '1');
include "Php_Serial/php_serial.class.php";

if (isset($_POST['id']) && $_POST['id'])
{ 
    $value = $_POST['id'];
    echo "Success";
   
    $serial = new PhpSerial;
    $serial->deviceSet("/dev/ttyS5");
    $serial->confBaudRate(57600);
    $serial->confParity("none");
    $serial->confCharacterLength(8);
    $serial->confStopBits(1);
    $serial->confFlowControl("none");

    $serial->deviceOpen();
    $serial->sendMessage($value);
    $serial->deviceClose();
}

?>