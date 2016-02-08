<!DOCTYPE HTML> 
<?php include('header.php') ?>
<html>
<head>
    <title>Welcome to Thought Experiments</title>
    <link rel="stylesheet" type="text/css" href="static/style.css">
</head>

<body>
    <div class="wrapper">
        <?php
            include('connectionData.txt');
            $mysqli = new mysqli(DB_HOST, DB_USERNAME, 
                DB_PASSWORD, DB_DATABASE);
            if (mysqli_connect_error()) {
                die('Connect Error (' . mysqli_connect_errno() 
                    . ') ' . mysqli_connect_error());
 }
        ?>
        <div id="title">    
            <h1>
            Think Boldly.
            </h1>
        </div>
        <div id="summary" class="content">
            The thought plane you are viewing contains 
            <?php
                $result = mysqli_query($conn,"SELECT count(*) FROM
                                              ThoughtExperiment;")
                          or die(mysqli_error($mysqli));
                $row = $result->fetch_row();
                echo "<a href=\"browse.php\"><strong>$row[0]</strong></a>";
            ?>
            thought experiments.
        </div>
        <div class="push"></div>
    </div>
    <div id="footer" class="content">
        <nav>
            <a href="index.php">Home</a> |
            <a href="create.php">Create</a> | 
            <a href="browse.php">Browse</a> | 
            <a href="about.html">About</a>
        </nav>
    </div>
</body>
</html>
