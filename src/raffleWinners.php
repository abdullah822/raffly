<?php
include "auth.php";
require __DIR__ . "/vendor/autoload.php";
use \MongoDB\Client as Mongo;
ini_set('display_startup_errors', 1);
ini_set('display_errors', 1);
error_reporting(-1);
//sets up mongodb connection
try {
    $client = new Mongo($mongoURL);
}
catch(MongoDB\Driver\Exception\ConnectionTimeoutException $e) {
    echo "Error: cant connect";
}
$collection = $client->Raffly->Raffle;
$mongoId = "5f84cfbc4a9e73506bbc75b1";
//$mongoCount=$collection->command('aggregate({$group: {"_id":ObjectId("5f84cfbc4a9e73506bbc75b1"),total:{$sum:{$size:"$entries"}}}})');
//sums the total number of entries in the raffle
$ops = array(array('$group' => array('_id' => new MongoDB\BSON\ObjectId("$mongoId"), 'total' => array('$sum' => array('$size' => '$entries')))));

$mongoCount = $collection->aggregate($ops);
$count = $mongoCount->toArray();
$entryCount = $count[0]["total"];
//fetches the quantity of product that is being raffled
$mongoData = $collection->findOne(array("_id" => new MongoDB\BSON\ObjectId("$mongoId")));
$quantity = $mongoData["products"][0]["quantity"];

/**Generates random winners and checks if they haven't already won the product.
 If there are fewer entries than items then all of the entries are win. 
 Else loop until the quantity has been raffled out.
**/

$alreadyWon = array();
$counter = 1;
if ($entryCount >= $quantity) {
    while ($counter <= $quantity) {
        $random = mt_rand(1, $entryCount);
        if (in_array($random, $alreadyWon)) {
            continue;
        } else {
            $counter++;
            array_push($alreadyWon, $random);
        }
    }
} else {
    $alreadyWon = range(1, $entryCount);
}

/**Performs a lookup of the winners and collects email address information and name info.
TODO add in email capability where winners are emailed.
**/
$collectionUsers = $client->Raffly->Users;
foreach ($alreadyWon as $entryId){

$userToLookup=$mongoData["entries"][$entryId-1]["user"];
$winnerData=$collectionUsers->findOne(array("username"=>$userToLookup));
echo $winnerData["username"] .", " . $winnerData["email"];
//mail()
}

?>
