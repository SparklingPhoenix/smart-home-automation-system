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


$sql = "SELECT id, state FROM m_s";
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