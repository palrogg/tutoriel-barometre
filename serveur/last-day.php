<?php
$csvfiles = scandir("output");
readfile("output/" . $csvfiles[count($csvfiles) - 1]);
if (intval(date('H')) < 12){
    readfile("output/" . $csvfiles[count($csvfiles) - 2]);
}