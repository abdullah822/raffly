<?php
include "mongod.inc";


$db = new Mongod;
$client = $db->connect($_POST);
$db->insertNewUser($_POST, $client);
