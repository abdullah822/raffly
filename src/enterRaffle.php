<?php
include "mongod.inc";
$raffleID = $_POST['raffle_id'];
$user = $_SESSION['username'] ;
$size = $_POST['size'] ?? false;

$db = new Mongod;
$client = $db->connect();
$db->enterRaffle($raffleID, $user, $client, $size);

?>
