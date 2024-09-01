<?php

$temperature = filter_var($_GET['temperature'], FILTER_VALIDATE_FLOAT);
$humidity = filter_var($_GET['humidity'], FILTER_VALIDATE_FLOAT);

$fp = fopen('mesures.csv', 'a');
$ts = date('c');

if (!$fp) {
    die('Could not open file!');
}
$fields = [$ts, $temperature, $humidity];
fputcsv($fp, $fields);
fclose($fp);