<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "testesp";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}


if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $temperature = $_POST['temperature'];
    $humidity = $_POST['humidity'];

    $sql = "INSERT INTO dht11 (temperature, humidity)
VALUES ($temperature, $humidity)";

if ($conn->query($sql) === TRUE) {
  
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}

} else {
    echo "Invalid request.";
}
$conn->close();
?>