<?php
include "auth.php";
require __DIR__ . "/vendor/autoload.php";
use \MongoDB\Client as Mongo;

ini_set('display_startup_errors', 1);
ini_set('display_errors', 1);
error_reporting(-1);

$username = $_POST["uname"];
$password = $_POST["pswd"];

//initiates the database connection
$mongoURL = "mongodb://" . $dbUsername . ":" . $dbPassword . "@" . $host . ":" . $port . "/?authSource=admin";

try
{
    $client = new Mongo($mongoURL);
}
catch(MongoDB\Driver\Exception\ConnectionTimeoutException $e)
{
    echo "Error: cant connect";
}
$collection = $client
    ->Raffly->Users;


//checks if username field is an email or a username and changes the field to the email or username field
if (filter_var($username, FILTER_VALIDATE_EMAIL))
{

    $result = $collection->findOne(['email' => $username], ['password' => $returnedPassword]);
}
else
{
    $result = $collection->findOne(['username' => $username], ['password' => $returnedPassword]);

}
//Logic to check the username and the password. Denies entry if it fails, and starts a session if it succeeds
if ($result['username'] == $username)
{
    if (password_verify($password, $result['password']))
    {
        echo "User authenticated";
        session_start();
        $_SESSION["username"] = $username;
        header('Location: https://rafflyraffles.com/shop.html');
        exit();

    }
    else
    {
        
        echo "<strong>Error: Incorrect password</strong>";
        exit();
    }
}
else
{
    echo "<strong>Error: Username does not exist!</strong>";
    exit();
}
?>
