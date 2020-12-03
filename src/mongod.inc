<?php
include "auth.php";

require __DIR__ . "/vendor/autoload.php";
use \MongoDB\Client as Mongo;
include 'mail.inc';
class Mongod {
    //initiate mongodb connection
    function connect() {
        global $mongoURL;
        try {
            $client = new Mongo($mongoURL);
        }
        catch(MongoDB\Driver\Exception\ConnectionTimeoutException $e) {
            echo "Error: cant connect";
        }
        return $client;
    }
    //assign user to raffle NEEDS UPDATE T0 CHECK
    //IF USER HAS ALREADY ENTERED RAFFLE
    function enterRaffle($raffleID, $user, $client, $size) {
	$collection = $client->Raffly->Users;
	$fetch = $collection->findOne(array("username" => $user));
	$ip = $fetch['ip'];
	$collection = $client->Raffly->Raffle;
	$fetch = $collection->findOne(array('_id' => new MongoDB\BSON\ObjectID($raffleID)));
	$cond = true;
	$d = $fetch['entries'];
//		ob_start();
//		var_dump($d);
//		$data = ob_get_clean();	
//		$f = fopen("tf.txt", "w");
//		fwrite($f,$data);
	$len = count($d);
	for($i=0;$i < $len;$i++)
	{
		if($d[$i]['ip'] == $ip)
		{
			$cond = false;
//			$c1 = $d[$i]['ip'];
		}
	}
//	fwrite($f, $c1);
//	fwrite($f, $ip);
//	fclose($f);
//	return;

	
	if($cond)
	{
		$date = date_create();
		$entry = array("user" => $user, "dateCreated" => new MongoDB\BSON\Timestamp(0, date_timestamp_get($date)), "size" => $size, "ip" => $ip);
		$updateResult = $collection->updateOne(['_id' => new MongoDB\BSON\ObjectID($raffleID) ], ['$push' => ['entries' => $entry]]);
		echo "Success";
	}
	else
	{
		echo "You have already entered this raffle";
//		fwrite($f, "yoyoyo");
	}
    }
    //generates alphanumeric tokens
    private function randomString($input, $strength = 16) {
        $input_length = strlen($input);
        $random_string = '';
        for ($i = 0;$i < $strength;$i++) {
            $random_character = $input[random_int(0, $input_length - 1) ];
            $random_string.= $random_character;
        }
        return $random_string;
    }
    //insert new user NEEDS SANITAZIATION, AND MORE...
    function insertNewUser($user, $client) {
        global $emailUsername;
        global $emailPassword;
        //set which collection to instert to
        $collection = $client->Raffly->Users;
        $exists = $collection->count(array("username" => $_POST['uname']));
        $permitted_chars = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ';
        $token = $this->randomString($permitted_chars, 8);
        
        $data = array(
            'secret' => "0xD4a32f4948388E31d81121adbCfB4749A7E1AD6B",
            'response' => $_POST['h-captcha-response']
        );
       $verify = curl_init();
       curl_setopt($verify, CURLOPT_URL, "https://hcaptcha.com/siteverify");
       curl_setopt($verify, CURLOPT_POST, true);
       curl_setopt($verify, CURLOPT_POSTFIELDS, http_build_query($data));
       curl_setopt($verify, CURLOPT_RETURNTRANSFER, true);
       $response = curl_exec($verify);
       $responseData = json_decode($response);
if($responseData->success) {
     if (!$exists) {
            $age = date_diff(date_create($_POST['date']), date_create('now'), True)->y;
            $date = date_create('now');
            $user = array("username" => $_POST['uname'], 
            "password" => password_hash($_POST['psw'], PASSWORD_DEFAULT), 
            "email" => $_POST['email'], 
            "firstName" => $_POST['fname'], 
            "lastName" => $_POST['lname'], 
            "phone" => $_POST['phone'], 
            "zip" => $_POST['zip'], 
            "state" => $_POST['state'], 
            "ip" => $_POST['ip'], 
            "creditCard" => $_POST['card'], 
            "dateCreated" => new MongoDB\BSON\Timestamp(0, date_timestamp_get($date)), 
            "birthday" => $_POST['date'], 
            "age" => $age,
             "token" => $token, 
             "verified" => false);
            $sendRegistrationMail = new rafflyMail;
            $urlToken = "https://rafflyraffles.com/middle/php/verifyEmail.php?t=" . $token;
            $sendRegistrationMail->setEmailUsername($emailUsername);
            $sendRegistrationMail->setEmailPassword($emailPassword);
            $message = "Hello, " . $_POST['uname'] . ",<br>
Your Raffly account is almost ready to be activated! Click the link below to verify your email address:<br>
" . $urlToken . "<br>
If you have not submitted this request, it is safe to ignore this message.<br><br>

Sincerely, <br> 
The Raffly Team";
            $sendRegistrationMail->sendMailRaffly($_POST['uname'], $_POST['email'], $message);
            //insert data
            $result = $collection->insertOne($user);
            header('Location: https://rafflyraffles.com/success');
            exit();
        } else {
        header('Location: https://rafflyraffles.com/register');
        
        echo "user exists";
        exit();
        }
        
} 
else {
   header('Location: https://rafflyraffles.com/register');
   echo "user didn't pass captcha";

   exit();
}
      
    }
    function getRaffles($client) {
        $collection = $client->Raffly->Raffle;
        /**allows for a limit on the number of results to look for to be set and for a number to be skipped
        this will allow for page 1 to 2,3,4 of results without needing to fetch all the data each time
        **/
        $limit = $_POST['limit'];
        $skip = $_POST['skip'];
        if (!is_null($limit) and $limit < 0 and is_null($skip)) {
            $fetchAll = $collection->find(array("complete" => false), [limit => $limit]);
        } elseif (!is_null($limit) and $limit < 0 and !is_null($skip) and $skip > 0) {
            $fetchAll = $collection->find(array("complete" => false), [limit => $limit, skip => $skip]);
        } else {
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
      function getRafflesComplete($client) {
        $collection = $client->Raffly->Raffle;
        
        $limit = $_POST['limit'];
        $skip = $_POST['skip'];
        //slices browser get variable to obtain the raffle hash. may fail if it slices invalid data
        $raffleID=substr($_GET['t'],6);
        try { 
        $id= new MongoDB\BSON\ObjectID($raffleID);
        
        }
        catch(Exception $e) {
        echo json_encode("Token not valid");
        exit();
        }
        
        //slice to obtain the token
        $token=substr($_GET['t'],0,6);
        
        /**allows for a limit on the number of results to look for to be set and for a number to be skipped
        this will allow for page 1 to 2,3,4 of results without needing to fetch all the data each time
        **/
        if (!is_null($limit) and $limit < 0 and is_null($skip)) {
            $fetchAll = $collection->find(array("complete" => true,'_id'=>$id), [limit => $limit]);
        } elseif (!is_null($limit) and $limit < 0 and !is_null($skip) and $skip > 0) {
            $fetchAll = $collection->find(array("complete" => true,'_id'=>$id), [limit => $limit, skip => $skip]);
        } else {
            $fetchAll = $collection->find(array("complete" => true,'_id'=>$id));
        }
        
        
        $returnArray = [];
        $tokensArray=[];
        $iterations=0;
        $tokensArray=$fetchAll->toArray();
        
       //accesses winner array and iterates to validate if any of the winners have the same token as the user
        foreach ($tokensArray[0]['winner'] as $winnerToken){
        $iterations+=1;
        if ($winnerToken['token']===$token){
          break;        
        }
        elseif ($winnerToken['token']!=$token and $iterations==count($winnerToken)){
        echo json_encode('Token not valid');
        exit();
        }
        }
      
         //loops for all raffles and indexes data to be used for frontend
      foreach ($tokensArray as $doc) {
       
    
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
    


    function createRaffle($client) {
        $date = date_create();
        //$sizeArray=explode(',', $_POST['size']);
        $collection=$client->Raffly->Raffle;
        $startTime=new  \MongoDB\BSON\Timestamp(0,$_POST['startTime']/1000);
        $endTime=new  \MongoDB\BSON\Timestamp(0,$_POST['endTime']/1000);
       $dataToInsert=array(
"title"=>$_POST['title'],
"description"=>$_POST['description'],
"company"=>$_POST['company'],
"startTime"=>$startTime,
"endTime"=>$endTime,
"winner"=>array(),
"entries"=>array(),
"complete"=>false,
"products"=>array(array(
"dateCreated"=>new MongoDB\BSON\Timestamp(0, date_timestamp_get($date)),
"quantity"=>$_POST['quantity'],
"productName"=>$_POST['productTitle'],
"ProductDescription"=>$_POST['productDescription'],
"unit_cost"=>$_POST['unit_cost'],
"format"=>$_POST['format'],
"productImage"=>"http://rafflyraffles.com/assets/images/".$_FILES["imageFile"]["name"],
"size"=>$_POST['size']
))
);
if ($_POST['endTime']>time()){

$insertResult=$collection->insertOne($dataToInsert);
$documentID=$insertResult->getInsertedId();

$target_dir = "/var/www/html/assets/images/";

$target_file = $target_dir . basename($_FILES["imageFile"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
  $check = getimagesize($_FILES["imageFile"]["tmp_name"]);
  if($check !== false) {
    echo "File is an image - " . $check["mime"] . ".";
    $uploadOk = 1;
  } else {
    echo "File is not an image.";
    $uploadOk = 0;
  }
}

// Check if file already exists
if (file_exists($target_file)) {
  echo "Sorry, file already exists.";
  $uploadOk = 0;
}

// Check file size
if ($_FILES["imageFile"]["size"] > 500000) {
  echo "Sorry, your file is too large.";
  $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
  echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
  $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["imageFile"]["tmp_name"], $target_file)) {
    echo "The file ". htmlspecialchars( basename( $_FILES["imageFile"]["name"])). " has been uploaded.";
  } else {
    echo "Sorry, there was an error uploading your file.";
  }
}

$duration=$_POST['endTime']-time();
$schedule="php /var/www/html/middle/php/raffleWinners.php ".$documentID." | at ". $duration;
shell_exec($schedule);
}
header("Location: https://rafflyraffles.com/admin");
exit();
}
    
    //php that checks the link and verifies the proper user
    function verifyEmail($client) {
        $collection = $client->Raffly->Users;
        $tokenCheck = $_GET['t'];
        $fetch = $collection->findOne(array("token" => $tokenCheck));
        $id = $fetch["_id"];
        if ($tokenCheck === $fetch["token"]) {
            $updateResult = $collection->updateOne(['_id' => new MongoDB\BSON\ObjectID($id) ], ['$set' => ['verified' => true, 'token' => '']]);
            header("Location: https://rafflyraffles.com/login");
            
            exit();
        } else {
            header("Location:https://rafflyraffles.com/");
           
            exit();
        }
    }
}
?>
