<?php
include "auth.php";
require __DIR__ . "/vendor/autoload.php";
use \MongoDB\Client as Mongo;

class Mongod
{

  //initiate mongodb connection
  function connect()
  {
    $dbUsername="frontend";
    $dbPassword="4XCos813EDn2OM1y";
    $host="3.138.61.22";
    $port="27017";
    $mongoURL = "mongodb://" . $dbUsername . ":" . $dbPassword . "@" . $host . ":" . $port . "/?authSource=admin";

    try
    {
        $client = new Mongo($mongoURL);
    }
    catch(MongoDB\Driver\Exception\ConnectionTimeoutException $e)
    {
        echo "Error: cant connect";
    }
    return $client;
  }

//insert new user NEEDS SANITAZIATION, PASSWORD HASH, AND MORE...
  function insertNewUser($user, $client)
  {
    //set which collection to instert to
    $collection = $client->Raffly->Users;
    $date       = date_create();
    $user       = array(
          "username"    => $_POST['uname'],
          "password"    => $_POST['psw'],
          "email"       => $_POST['email'],
          "firstName"   => $_POST['fname'],
          "lastName"    => $_POST['lname'],
          "phone"       => $_POST['phone'],
          "zip"         => $_POST['zip'],
          "state"       => $_POST['state'],
          "ip"          => '123',
          "creditCard"  => $_POST['card'],
          "dateCreated" => $date,
          "birthday"    => $_POST['date']
      );

    //insert data
    $result = $collection->insertOne($user);
  }

}
