<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>

    <script>
        window.setInterval(function(){
            document.location.reload(true);
            console.log('reloaded')
        }, 3000);
    </script>
    <style>
        body{
            background-color: rgb(128, 128, 128);
        }
        .title{
            color: rgb(9, 19, 135);
            text-transform: uppercase;
            text-decoration: underline;
            font-size: 40px;
        }
        div{
            font-size: 20px;
            color: white;
            text-align: center;
            font-weight: bolder;
            height: 35px;
            width: 80px;
            border-radius: 25px;
            border: solid black 1px;
        }

        div b{
            position: relative;
            top: 2.5px;
        }

        table{
            font-family: arial, sans-serif;
            border-collapse: collapse;
            width: 80%;
        }

        td, th {
            text-align: center;
            font-size: 20px;
            font-weight: bold;
            padding: 9px;
        }

        th{
            background-color: rgb(66, 75, 158);
            font-size: 30px;
            color: white;
            height: 50px;
        }

        tr:nth-child(even){
            background-color: rgb(224, 227, 235);
        }

        tr:hover{
            /* background-color: rgba(123, 131, 145, 0.516); */
            background-color: lightblue;
        }
    </style>
</head>
<body>
    <center>
        <h1 class="title">Poles' Angle Details</h1>
        <table>
            <tr>
                <th>Agregate</th>
                <th>Node A</th>
                <th>Node B</th>
                <th>Date</th>
                <th>Time</th>
            </tr>

            <?php
                include("api/db_connection.php");
                $sql = "SET time_zone = '+3:00'";
                $query1 = mysqli_query($conn, $sql);

                $sql = "SELECT agregate, node, date, TIME_FORMAT(time, '%H:%i') AS time FROM `pole's_angle_details`";
                $query2 = mysqli_query($conn, $sql);

                if($query1 && $query2){
                    while($row = mysqli_fetch_assoc($query2)){
                        $agregate = $row['agregate'];
                        $node = $row['node'];
                        $date = $row['date'];
                        $time = $row['time'];

                        if($agregate < 10){
                            echo'<tr>
                                    <td><center><div style="background-color: green;"><b>' .$agregate. '&deg;</b></div></center></td>';
                        }else{
                            echo'<tr>
                                    <td><center><div style="background-color: red;"><b>' .$agregate. '&deg;</b></div></center></td>';
                        }

                        if($node < 10){
                            echo'<td><center><div style="background-color: green;"><b>' .$node. '&deg;</b></div></center></td>';
                        }else{
                            echo'<td><center><div style="background-color: red;"><b>' .$node. '&deg;</b></div></center></td>';
                        }

                        echo'
                                <td><center><div style="background-color: grey;"></div></center></td>
                                <td>' .$date. '</td>
                                <td>' .$time. '</td>   
                            </tr> 
                        ';
                    }
                }

            ?>
            
        </table>

    </center>
</body>
</html>