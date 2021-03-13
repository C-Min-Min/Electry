<?php

define('DB_HOST', '34.107.85.119');
define('DB_USER', 'root');
define('DB_PASS', 'hacktues_c');
define('DB_NAME', 'electry');

//connecting to database and getting the connection object
$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);

//Checking if any error occured while connecting
if (mysqli_connect_errno()) {
    echo "Failed to connect to MySQL: " . mysqli_connect_error();
    die();
}

$result = $conn->query("SHOW TABLES FROM `electry` LIKE '%measure%'");
$measurement_json = array();
while($table = mysqli_fetch_array($result)) { // go through each row that was returned in $result
    $query = "SELECT * FROM `".$table[0]."`";

    $measurements = $conn->query($query);

    $measures = array();

    while($row = $measurements->fetch_assoc()){
        array_push($measures, $row);

    }
    array_push($measurement_json, $measures);
}
echo json_encode($measurement_json);
?>