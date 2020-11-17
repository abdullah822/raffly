<?php
include "mongod.inc";
ini_set('display_startup_errors', 1);
ini_set('display_errors', 1);
error_reporting(-1);


$db = new Mongod;
$client = $db->connect();
$db->generatePassword($_POST['email'], $client);

?>
