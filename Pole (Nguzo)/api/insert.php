<?php
    header("Access-Control-Allow-Orign: *");
    header("Content-Type:application/json; charset:UTF-8");

    include("db_connection.php");

    if(isset($_GET['agregate']) && isset($_GET['node'])){
        $agregate = $_GET['agregate'];
        $node = $_GET['node'];

        $sql = "INSERT INTO `pole's_angle_details` (agregate, node, date, time) VALUES ('$agregate', '$node', curdate(), curtime())";
        $query = mysqli_query($conn, $sql);

        if($query){
            echo "SUCCESS";
        }else{
            echo "FAIL";
        }
    }else{
        echo "No data to insert";
    }