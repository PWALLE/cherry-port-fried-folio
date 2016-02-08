<!DOCTYPE HTML> 

<html>
<head>
    <title>View Thought</title>
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
                $thought_id = $_GET['thought_id'];
                $thought_result = $mysqli->query("SELECT title,date_created,date_modified FROM ThoughtExperiment WHERE id=" . $thought_id) or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                $category_results = $mysqli->query("SELECT cat_name from TE_Cat WHERE TE_id=".$thought_id) or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                if (!($stmt = $mysqli->prepare("SELECT cat_name from TE_Cat WHERE TE_id=?"))) {  
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;                      exit("Quitting.");
                }
                if (!($stmt->bind_param("i",$thought_id))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .                                    $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;                      exit("Quitting.");
                }
                $stmt->bind_result($category);
                print "Categories: ";
                while ($stmt->fetch()) { 
                    print $category . ", ";
                }


            ?>
            <hr/>
            <?php
                while ($row = $thought_result->fetch_assoc()) {
                    echo "<table style=\"margin:auto;\"><tr><td><strong>Title: " . $row['title'] . "</strong></td><td></td></tr>";
                    echo "<tr><td>Created: " . $row['date_created']. "</td><td><a href=\"comment.php?type=ThoughtExperiment&id=" . $thought_id . "\">Comment</a></td></tr>";
                    echo "<tr><td>Last Modified: " . $row['date_modified']. "</td><td><a href=\"addmood.php?type=ThoughtExperiment&id=" . $thought_id . "\">Add Mood</a></td></tr></table>";
                }
            ?>
            <hr/>
            <?php
                if (!($stmt = $mysqli->prepare("SELECT s.id, s.text, s.date_created,s.date_modified,s.cat_name from TE_TextSec t JOIN TextSection s ON t.TS_id=s.id WHERE t.TE_id=?"))) {  
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;                      exit("Quitting.");
                }
                if (!($stmt->bind_param("i",$thought_id))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .                                    $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;                      exit("Quitting.");
                }
                $stmt->bind_result($id,$text,$create,$modify,$cat);
                while($stmt->fetch()) {
                        print "Category: " . $cat ."<br/>";
                        print "<p>". $text ."</p>";
                        print "<table style=\"margin:auto;\"><tr><td>Created: ". $create. "</td><td><a href=\"comment.php?type=TextSection&id=" . $id. "\">Comment</a></td></tr>";
                        print "<tr><td>Modified: ". $modify. "</td><td><a href=\"addmood.php?type=TextSection&id=" . $id . "\">Add Mood</a></td></tr></table>";
                } 

            ?> 
            <?php
                if (!($stmt = $mysqli->prepare("SELECT s.id, s.title, s.location from TE_Mult t JOIN Multimedia s ON t.mult_id=s.id WHERE t.TE_id=?"))) {  
                    echo "Prepare failed: (" . $mysqli->errorno . ") " . $mysqli->error;                      exit("Quitting.");
                }
                if (!($stmt->bind_param("i",$thought_id))) {
                    echo "Binding parameters failed: (" . $mysqli->errorno . ") " .                                    $mysqli->error;
                exit("Quitting.");
                }
                if (!($stmt->execute())) {
                    echo "Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error;                      exit("Quitting.");
                }
                $stmt->bind_result($id,$title,$location);
                while($stmt->fetch()) {
                        print "<table style=\"margin:auto;\"><tr><td>Title: ". $title. "</td><td><a href=\"comment.php?type=Multimedia&id=\"" . $id. "\">Comment</a></td></tr>";
                        print "<tr><td><img height=\"50\" width\"50\" src=\"". $location. "\" alt=\"question.png\"></td><td><a href=\"addmood.php?type=Multimedia&id=\"" . $id . "\">Add Mood</a></td></tr></table>";
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
