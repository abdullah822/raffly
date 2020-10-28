<?php
include "auth.php";
require __DIR__ . "/vendor/autoload.php";
use \MongoDB\Client as Mongo;

class Mongod
{

  //initiate mongodb connection
  function connect()
  {

    global $mongoURL;

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

  //assign user to raffle NEEDS UPDATE T0 CHECK
  //IF USER HAS ALREADY ENTERED RAFFLE
  function enterRaffle($raffleID, $user, $client)
  {

    $collection = $client->Raffly->Raffle;
    $date       = date_create();

    $entry          = array(
      "user"        => $user,
      "dateCreated" => new MongoDB\BSON\Timestamp(0, date_timestamp_get($date)),
      "size"        => $size
    );

  $updateResult     = $collection->updateOne(
                                ['_id' => new MongoDB\BSON\ObjectID($raffleID)],
                                ['$push' => ['entries' => $entry]]);

            //printf("Matched %d document(s)\n", $updateResult->getMatchedCount());
            //printf("Modified %d document(s)\n", $updateResult->getModifiedCount());
  }

//insert new user NEEDS SANITAZIATION, AND MORE...
  function insertNewUser($user, $client)
  {
    //set which collection to instert to
      $collection = $client->Raffly->Users;

      $exists = $collection->count(array("username" => $_POST['uname']));

      if(!$exists)
      {
          $age   = date_diff(date_create($_POST['date']), date_create('now'), True)->y;
          $date  = date_create('now');
          $user  = array(
            "username"    => $_POST['uname'],
            "password"    => password_hash($_POST['psw'], PASSWORD_DEFAULT),
            "email"       => $_POST['email'],
            "firstName"   => $_POST['fname'],
            "lastName"    => $_POST['lname'],
            "phone"       => $_POST['phone'],
            "zip"         => $_POST['zip'],
            "state"       => $_POST['state'],
            "ip"          => $_POST['ip'],
            "creditCard"  => $_POST['card'],
            "dateCreated" => new MongoDB\BSON\Timestamp(0, date_timestamp_get($date)),
            "birthday"    => $_POST['date'],
            "age"         => $age
          );

          //insert data
          $result = $collection->insertOne($user);
        }
        else
        {
          echo "user exists";
        }
  }
function getRaffles($client){
$collection = $client->Raffly->Raffle;
/**allows for a limit on the number of results to look for to be set and for a number to be skipped
   this will allow for page 1 to 2,3,4 of results without needing to fetch all the data each time
**/
$limit = $_POST['limit'];
$skip = $_POST['skip'];
if (!is_null($limit) and $limit < 0 and is_null($skip)) {
    $fetchAll = $collection->find(array("complete" => false),[limit=>$limit]);
}elseif (!is_null($limit) and $limit < 0 and !is_null($skip) and $skip>0){
$fetchAll = $collection->find(array("complete" => false),[limit=>$limit, skip=>$skip]);
}
else {
    $fetchAll = $collection->find(array("complete" => false));
}
$returnArray = [];
//loops for all raffles and indexes data to be used for frontend
foreach ($fetchAll as $doc) {
    $product = $doc["products"][0];
    $rafflyFetch = array(

    "title" => $doc["title"],
    "description" => $doc["description"],
    "startTime" => $doc["startTime"],
    "endTime" => $doc["endTime"],
    "id" => $doc["_id"],
    "company" => $doc["company"],
    "productImage" => $product["productImage"],
    "quantity" => $product["quantity"],
    "size" => $product["size"],
    "format" => $product["format"],
    "unit_cost" =>$product["unit_cost"],
    "productName" => $product["productName"]);

    array_push($returnArray, $rafflyFetch);
}
echo json_encode($returnArray);
}

function createRaffle($client){
$date = date_create();
$dataToInsert=array(
"title"=>$_POST['title'],
"description"=>$_POST['description'],
"company"=>$_POST['company'],
"startTime"=>$_POST['startTime'],
"endTime"=>$_POST['endTime'],
"winner"=>array(),
"entries"=>array(),
"complete"=>false,
"products"=>array(array(
"dateCreated"=>new MongoDB\BSON\Timestamp(0, date_timestamp_get($date)),
"quantity"=>$_POST['quantity'],
"productName"=>$_POST['productTitle'],
"ProductDescription"=>$_POST['productDescription'],
"unit_cost"=>$_POST['unitCost'],
"format"=>$_POST['format'],
"productImage"=>$_POST['image'],
"size"=>$_POST['size']
))
);


$collection = $client->Raffly->Raffle;

$collection->insertOne($dataToInsert);
}
}


?>
