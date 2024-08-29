<?php

$temperature = filter_var($_GET['temperature'], FILTER_VALIDATE_FLOAT);
$humidity = filter_var($_GET['humidity'], FILTER_VALIDATE_FLOAT);

date_default_timezone_set('Europe/Zurich');

$today = date('Y-m-d');
$fp = fopen('output/' . $today . '.csv', 'a');
$ts = date('c');

if (!$fp) {
    die('Could not open file!');
}
$fields = [$today, $ts, $temperature, $humidity];
fputcsv($fp, $fields);
fclose($fp);