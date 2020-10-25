<?php
include "mongod.inc";


$db = new Mongod;
$client = $db->connect();
$db->insertNewUser($_POST, $client);
