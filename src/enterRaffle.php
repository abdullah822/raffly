<?php
include "mongod.inc";
$raffleID = $_POST['raffle_id'];
$user = $_SESSION['username'] ;

$db = new Mongod;
$client = $db->connect();
$db->enterRaffle($raffleID, $user, $client);

?>
