<?php
include "auth.php";
require __DIR__ . "/vendor/autoload.php";
use \MongoDB\Client as Mongo;
ini_set('display_startup_errors', 1);
ini_set('display_errors', 1);
error_reporting(-1);
//connect to db
try {
    $client = new Mongo($mongoURL);
}
catch(MongoDB\Driver\Exception\ConnectionTimeoutException $e) {
    echo "Error: cant connect";
}
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
    "productName" => $product["productName"]);
    
    array_push($returnArray, array($rafflyFetch));
}
echo json_encode($returnArray);
?>
