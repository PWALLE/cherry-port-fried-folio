<!DOCTYPE HTML> 

<html>
<head>
    <title>Add Mood</title>
    <link rel="stylesheet" type="text/css" href="static/style.css">
</head>

<body>
    <div class="wrapper">
        <?php
            include('connectionData.txt');
            $mysqli = new mysqli($server, $user, $pass, $dbname, $port);
            if ($mysqli->connect_errno) {
                echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " .
                    $mysqli->connect_error;
                exit("Quitting.");
            }
        ?>
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
                        echo "Adding mood for thought experiment: <strong>".$row['title'] ."</strong>";
                    }
                    elseif ($type == "TextSection") {
                        echo "Adding mood for text section: <p>".$row['text'] ."</p>";
                    }
                    elseif ($type == "Multimedia") {
                        echo "Adding mood for multimedia: <img src=\"".$row['location']."\" alt=question.png/ height=\"50\" width=\"50\">";
                    }
                }
            ?>
            <hr/>
            <form action="addmood.php?type=<?php echo $type;?>&id=<?php echo $id ?>" method="POST">
                <?php
                    $result = $mysqli->query("SELECT name FROM Mood") or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                    echo "<select name=\"mood\">";
                    while ($row = $result->fetch_assoc()) {
                        echo "<option value=\"".$row['name']."\">".$row['name']."</option>";
                    }
                    echo "</select>";
                ?>
                <input type="hidden" name="id" value="<?php echo $id;?>">
                <input type="hidden" name="type" value="<?php echo $type;?>">
                <button type="submit">Moodify!</button>
            </form>
            <?php
                if ($_POST) {
                    if ($_POST['type'] == "ThoughtExperiment") {
                        if (!($stmt = $mysqli->prepare("INSERT INTO TE_Mood(mood_name,TE_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                        exit("Quitting.");
                        }
                    }
                    elseif ($_POST['type'] == "TextSection") {
                        if (!($stmt = $mysqli->prepare("INSERT INTO TS_Mood(mood_name,TS_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                        exit("Quitting.");
                        }
                    }
                    elseif ($_POST['type'] == "Multimedia") {
                        if (!($stmt = $mysqli->prepare("INSERT INTO Mult_Mood(mood_name,mult_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                        exit("Quitting.");
                        }
                    }
                    if (!($stmt->bind_param("si",$_POST['mood'],$_POST['id']))) {
                        echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                             $mysqli->error;
                    exit("Quitting.");
                    }
                    if (!($stmt->execute())) {
                        echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                    exit("Quitting.");
                    }
                    else {
                        echo "Mood successfully added!";
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
