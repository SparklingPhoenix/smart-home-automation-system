<?php
$conn = mysqli_connect(
    DB_HOST,
    DB_USER,
    DB_PASSWORD,
    DB_NAME
);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}


$name = $_POST["id"];
if ($name == '1') {$sql = "SELECT number, state FROM msp WHERE id BETWEEN 1 AND 3";};
if ($name == '2') {$sql = "SELECT number, state FROM msp WHERE id BETWEEN 4 AND 6";};
if ($name == '3') {$sql = "SELECT number, state FROM msp WHERE id BETWEEN 7 AND 9";};
if ($name == '4') {$sql = "SELECT number, state FROM msp WHERE id BETWEEN 10 AND 13";};
if ($name == '5') {$sql = "SELECT number, state FROM msp WHERE id BETWEEN 14 AND 18";};
if ($name == '6') {$sql = "SELECT number, state FROM msp WHERE id BETWEEN 19 AND 21";};
if ($name == '7') {$sql = "SELECT number, state FROM msp WHERE id = '22'";};
$result = mysqli_query($conn, $sql);
if (mysqli_num_rows($result) > 0) {
    while ($row = mysqli_fetch_assoc($result)) {
        echo json_encode($row);
    }
 } else {
     echo '0 results';
 }

mysqli_close($conn);

?>