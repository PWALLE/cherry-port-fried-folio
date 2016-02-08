<!DOCTYPE HTML> 

<html>
<head>
    <title>New Thought</title>
    <link rel="stylesheet" type="text/css" href="static/style.css">
</head>

<body>
<script type="text/javascript">
   var attachments = 0;

    function validate(form) {
        var title = document.forms["create"]["thought_title"].value;
        if (title.length > 50 || title.length == 0) { alert("Thought Title: must be 0 < x < 50 characters"); return  false; }
        var text = document.forms["create"]["text"].value;
        if (text.length > 1000 || title.length == 0) { alert("Text: must be 0 < x < 1000 characters"); return  false; }
        for (i = 0; i < attachments; i++) {
            var id = "attachment" + i.toString();
            var attach_title = document.forms["create"][id].value;
            if (attach_title.length > 100 || attach_title.length == 0) { alert("Attachment Title: must be 0 < x < 100 characters"); return  false; }
            var attach_loc = document.forms["create"][id + "_loc"].value;
            if (attach_loc.length > 200 || attach_loc.length == 0) { alert("Attachment Location: must be 0 < x < 200 characters"); return  false; }
            var attach_cat = document.forms["create"]["attachment" + i + "_type"].value;
            if (!(attach_cat === "Image")) { alert("Sorry, this thought plane only supports image attachments right now"); return false; }
        }
        return true;

    }

    function attach_media() {
        document.getElementById("attachments").innerHTML += "Title <input type=\"text\" name=\"attachment" + attachments + "\"/> Location (local or URL): <input type=\"text\" name=\"attachment" + attachments + "_loc\"/>" +
            "<select name=\"attachment" + attachments + "_type\">" +
            "<option value=\"Audio\">Audio</option><option value=\"Video\">Video</option><option value=\"Image\">Image</option></select><br/>";
        attachments += 1;
        document.forms["create"]["attachment_count"].value = attachments;
    }
</script>
    <div class="wrapper">
        <?php
            include('connectionData.txt');

            $conn = mysqli_connect($server, $user, $pass, $dbname, $port)
            or die('Error connecting to MySQL server.');
        ?>
        <div id="title">    
            <h1>
            Think Boldly.
            </h1>
        </div>
        <div id="summary" class="content">
			<form name="create" action="reviewcreation.php" onSubmit="return validate(this);" method="post">
			Title: <input type="text" name="thought_title">
			<?php 
				$cat_results = mysqli_query($conn,"SELECT DISTINCT name FROM ThoughtCategory
								                   WHERE name IS NOT NULL;") 
							   or die(mysqli_error($conn));
				echo "Category <select name=\"thought_category\">";
				while($row = mysqli_fetch_array($cat_results, MYSQLI_BOTH)){
					echo "<option value=\"$row[0]\">$row[0]</option>";
				}
				echo "</select>";
    
				mysqli_free_result($cat_results);
	
			?>
			<hr/>
			<?php 
				$cat_results = mysqli_query($conn,"SELECT DISTINCT name FROM TextCategory
							                       WHERE name IS NOT NULL;") 
							   or die(mysqli_error($conn));
				echo "Category <select name=\"text_category\">";
				while($row = mysqli_fetch_array($cat_results, MYSQLI_BOTH)){
					echo "<option value=\"$row[0]\">$row[0]</option>";
				}
				echo "</select>";
    
				mysqli_free_result($cat_results);
	
            ?>
            <br/>
            <br/>
            <textarea name="text" rows="10" cols="60" autofocus>We are a way for the cosmos to know itself. ~Carl Sagan</textarea>
            <div id="attachments"></div>
            <input type="hidden" name="attachment_count" value=0/>
            <hr/>
            <div id="options">
                <ul>
                    <li><button type="button" onclick="attach_media();">Attach Media</button></li>
                    <li><button type="submit">Create!</button></li>
                </ul>
            </div>
            </form>
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
