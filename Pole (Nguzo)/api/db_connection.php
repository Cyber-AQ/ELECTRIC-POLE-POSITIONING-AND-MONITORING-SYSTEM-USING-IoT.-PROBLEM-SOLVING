<?php
    $host_name = "localhost";
    $user_name = "root";
    $password = "";
    $db_name = "poles";

    $conn = mysqli_connect($host_name, $user_name, $password, $db_name);

    if(!$conn){
        echo "Fail to connect to the Database";
        return;
    }