<!DOCTYPE HTML> 

<html>
<head>
    <title>Browse Thought</title>
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
            <div id="filter">
                <table><tr><td>Filter:</td><td> <form action="browse.php" method="get">
                    <select name="category_id">
                        <option value="ThoughtExperiments">Thought Experiments</option>
                        <option value="TextSections">Text Sections</option>
                        <option value="Multimedia">Multimedia</option>
                    </select></td><td>
                    <button type="submit">Go!</button></td>
                </form></table>
            </div>
            <br class="clear"/>
            <?php
            //if(!($category_id = $_GET['category_id']) || !($category_id = $_POST['category_id']) ) {
            if(!($category_id = $_GET['category_id']) ) {

                $category_id = "ThoughtExperiments";
            }
            if ($category_id == "ThoughtExperiments") {
                echo "<h2>Viewing All Thought Experiments</h2>";
                $thought_result = $mysqli->query("SELECT id,title,date_created,date_modified FROM ThoughtExperiment") or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                while ($row = $thought_result->fetch_assoc()) {
                    echo "<table style=\"margin:auto;\"><tr><td><strong>Title: <a href=\"view.php?thought_id=" . $row['id'] . "\">" . $row['title'] . "</a></strong></td><td></td></tr>";
                    echo "<tr><td>Created: " . $row['date_created']. "</td><td><a href=\"comment.php?type=ThoughtExperiment&id=" . $row['id'] . "\">Comment</a></td></tr>";
                    echo "<tr><td>Last Modified: " . $row['date_modified']. "</td><td><a href=\"addmood.php?type=ThoughtExperiment&id=" . $row['id'] . "\">Add Mood</a></td></tr></table>";
                }
            }
            elseif ($category_id == "TextSections") {
                echo "<h2>Viewing All Text Sections</h2>";
                $text_result = $mysqli->query("SELECT id,text,date_created,date_modified,cat_name FROM TextSection") or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                while ($row = $text_result->fetch_assoc()) {
                    echo "<p style=\"margin:10px;\">" . $row['text'] . "</p>";
                    echo "<table style=\"margin:auto;\"><tr><td>Created: " . $row['date_created']. "</td><td><a href=\"comment.php?type=TextSection&id=" . $row['id'] . "\">Comment</a></td></tr>";
                    echo "<tr><td>Last Modified: " . $row['date_modified']. "</td><td><a href=\"addmood.php?type=TextSection&id=" . $row['id'] . "\">Add Mood</a></td></tr></table>";
                }
            }   
            elseif ($category_id == "Multimedia") {
                echo "<h2>Viewing All Multimedia</h2>";
                $multi_result = $mysqli->query("SELECT id,title,type,location FROM Multimedia") or die("Execute failed: (" . $mysqli->errorno . ") " . $mysqli->error);
                while ($row = $multi_result->fetch_assoc()) {
                    echo "<img src=\"" . $row['location'] . "\" alt=\"question.png\" height=\"50\" width=\"50\"/><br/>";
                    echo "<table style=\"margin:auto;\"><tr><td>Title: " . $row['title'] . "</td><td></td></tr>";
                    echo "<tr><td>Created: " . $row['date_created']. "</td><td><a href=\"comment.php?type=Multimedia&id=" . $row['id'] . "\">Comment</a></td></tr>";
                    echo "<tr><td>Last Modified: " . $row['date_modified']. "</td><td><a href=\"addmood.php?type=Multimedia&id=" . $row['id'] . "\">Add Mood</a></td></tr></table>";
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
            <a href="browse.php?">Browse</a> | 
            <a href="about.html">About</a>
        </nav>
    </div>
</body>
</html>
