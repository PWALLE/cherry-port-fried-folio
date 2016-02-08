<!DOCTYPE HTML> 
<?
    php ob_start('ob_gzhandler');
    include('connectionData.txt');
    $mysqli = new mysqli($server, $user, $pass, $dbname, $port);
    if ($mysqli->connect_errno) {
        echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " .
            $mysqli->connect_error;
        exit("Quitting.");
    }
?>

<html>
<head>
    <title>Make a Comment</title>
    <link rel="stylesheet" type="text/css" href="static/style.css">
</head>

<body>
    <div class="wrapper">
        <div id="title">    
            <h1>
            Think Boldly.
            </h1>
        </div>
        <div id="summary" class="content">

            <?php           
                $id = $_GET['id'] or die;
                $type = $_GET['type'];
                
                $result = $mysqli->query("SELECT * FROM ".$type." WHERE id=" . $id) or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);

                while ($row = $result->fetch_assoc()) {
                    if ($type == "ThoughtExperiment") {
                        echo "Commenting on thought experiment: <strong>".$row['title'] ."</strong>";
                    }
                    elseif ($type == "TextSection") {
                        echo "Commenting on text section: <p>".$row['text'] ."</p>";
                    }
                    elseif ($type == "Multimedia") {
                        echo "Commenting on multimedia: <img src=\"".$row['location']."\" alt=question.png/ height=\"50\" width=\"50\">";
                    }
                }
            ?>
            <hr/>
            <form action="comment.php?type=<?php echo $type;?>&id=<?php echo $id ?>" method="POST">
                <textarea name="text" row=30 col=60>What say you?</textarea>
                <input type="hidden" name="id" value="<?php echo $id;?>">
                <input type="hidden" name="type" value="<?php echo $type;?>">
                <button type="submit">Comment!</button>
            </form>
            <?php
                if ($_POST) {
                    if ($_POST['type'] == "ThoughtExperiment") {
                        if (!($stmt = $mysqli->prepare("INSERT INTO Comment (text,TE_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                        exit("Quitting.");
                        }
                    }
                    elseif ($_POST['type'] == "TextSection") {
                        if (!($stmt = $mysqli->prepare("INSERT INTO Comment (text,TS_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                        exit("Quitting.");
                        }
                    }
                    elseif ($_POST['type'] == "Multimedia") {
                        if (!($stmt = $mysqli->prepare("INSERT INTO Comment (text,M_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                        exit("Quitting.");
                        }
                    }
                    if (!($stmt->bind_param("si",$_POST['text'],$_POST['id']))) {
                        echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                             $mysqli->error;
                    exit("Quitting.");
                    }
                    if (!($stmt->execute())) {
                        echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                    exit("Quitting.");
                    }
                    else {
                        echo "Comment successfully added!";
                    }
                }
            ?> 
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
