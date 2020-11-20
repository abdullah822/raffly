<?php
include "mongod.inc";
define("RECAPTCHA_V3_SECRET_KEY", '6Ld3jNkZAAAAAH2ThcRFyDXkiNCyr1GjJrYqeJOY');

$token = $_POST['g-recaptcha-response'];

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL,"https://www.google.com/recaptcha/api/siteverify");
curl_setopt($ch, CURLOPT_POST, 1);
curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query(array('secret' => RECAPTCHA_V3_SECRET_KEY, 'response' => $token)));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
$response = curl_exec($ch);
curl_close($ch);
$arrResponse = json_decode($response, true);

if($arrResponse["success"] == '1' && $arrResponse["score"] >= 0.5)
{
  $db = new Mongod;
  $client = $db->connect();
  $db->insertNewUser($_POST, $client);
}
else
{
  echo "spam";
}
