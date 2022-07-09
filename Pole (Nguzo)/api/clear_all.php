<?php
    header("Access-Control-Allow-Orign: *");
    header("Content-Type:application/json; charset:UTF-8");

    include("db_connection.php");

    $sql = "DELETE FROM `pole's_angle_details`";
    $query = mysqli_query($conn, $sql);

    if($query){
        echo "DONE";
    }else{
        echo "FAIL";
    }

    