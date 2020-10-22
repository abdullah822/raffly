<?php
include 'mongod.inc';
$fetchRaffle=new Mongod;
$connection=$fetchRaffle->connect();
$fetchRaffle->createRaffle($connection);
?>
