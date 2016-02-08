<!DOCTYPE HTML> 

<html>
<head>
    <title>New Thought</title>
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
                $title = $_POST['thought_title'];
                $thought_category = $_POST['thought_category'];
                $text_category = $_POST['text_category'];
                $text = $_POST['text'];
                $attachment_count = $_POST['attachment_count'];
                $attachments = array();
                for ($i = 0; $i < $attachment_count; $i++) {
                    $attach_array = array($_POST["attachment" . $i], $_POST["attachment" . $i . "_loc"], $_POST["attachment" . $i . "_type"]);
                    $attachments[$i] = $attach_array;
                }
                $date = date('Y-m-d H:i:s');
                if (!($stmt = $mysqli->prepare("INSERT INTO ThoughtExperiment (title,date_created,date_modified) VALUES(?,?,?)"))) {
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->bind_param("sss",$title,$date,$date))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                         $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }
                $thought_id = $stmt->insert_id;

                if (!($stmt = $mysqli->prepare("INSERT INTO TE_Cat (cat_name, TE_id) VALUES(?,?)"))) {
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->bind_param("si",$thought_category,$thought_id))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                         $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }

                if (!($stmt = $mysqli->prepare("INSERT INTO TextSection (text,date_created,date_modified,cat_name) VALUES(?,?,?,?)"))) {
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->bind_param("ssss",$text,$date,$date,$text_category))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                         $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }
                $text_id = $stmt->insert_id;

                if (!($stmt = $mysqli->prepare("INSERT INTO TE_TextSec (TS_id,TE_id) VALUES(?,?)"))) {
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->bind_param("ii",$text_id,$thought_id))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                         $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                }

                for ($i = 0; $i < count($attachments); $i++) {
                    if (!($stmt = $mysqli->prepare("INSERT INTO Multimedia (title, type, location) VALUES(?,?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                    }
                    if (!($stmt->bind_param("sss",$attachments[$i][0],$attachments[$i][2],$attachments[$i][1]))) {
                        echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                             $mysqli->error;
                exit("Quitting.");
                    }
                    if (!($stmt->execute())) {
                        echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                    }
                    $attach_id = $stmt->insert_id;
                    if (!($stmt = $mysqli->prepare("INSERT INTO TE_Mult (mult_id, TE_id) VALUES(?,?)"))) {
                        echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                    }
                    if (!($stmt->bind_param("ii",$attach_id,$thought_id))) {
                        echo "Binding parameters failed: (" . $mysqli->errorno . ") " .
                             $mysqli->error;
                exit("Quitting.");
                    }
                    if (!($stmt->execute())) {
                        echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;
                exit("Quitting.");
                    }
                }
                $result = $mysqli->query("SELECT title,sub.count,c.cat_name from ThoughtExperiment t JOIN (SELECT count(*) AS count, TE_id from TE_Mult WHERE TE_id=" . $thought_id . ") AS sub ON t.id=sub.TE_id JOIN TE_Cat c ON t.id = c.TE_id") or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                while ($row = $result->fetch_assoc()) { 
                   echo "Successfully created a thought experiment called " . $row['title'] . " of type " . $row['cat_name'] . " with " . $row['count'] . " media attachments. <a href=\"view.php?thought_id=" . $thought_id . "\">Check it out!</a>";
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
