<?php

// Fichier contenant le "slack hook"
require("config.php");

$temperature = filter_var($_GET['temperature'], FILTER_VALIDATE_FLOAT);
$humidity = filter_var($_GET['humidity'], FILTER_VALIDATE_FLOAT);

// AJOUT: on lit le voltage
$battery_level = filter_var($_GET['battery_level'], FILTER_VALIDATE_INT);

$fp = fopen('mesures.csv', 'a');
$ts = date('c');

if (!$fp) {
    die('Could not open file!');
}
$fields = [$ts, $temperature, $humidity];
fputcsv($fp, $fields);
fclose($fp);

// AJOUT: Envoi d'une alerte sur Slack si le voltage passe sous 2800
if ($battery_level < 2800) {
    $url = $slack_hook;
    $data = array("text" => "Je manque d’énergie! <@ID-UTILISATEUR-A-NOTIFIER>");

    $postdata = json_encode($data);

    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $postdata);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json'));
    $result = curl_exec($ch);
    curl_close($ch);
}
