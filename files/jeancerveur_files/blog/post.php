
<!doctype html>
<html>
	<meta charset="UTF-8">
  <head>  
    <title>A super blog</title>
  </head>
  <body>
		<?php
			$title = isset($_SERVER['title']) ? $_SERVER['title'] : NULL;
			$content = isset($_SERVER['content']) ? $_SERVER['content'] : NULL;
			$name = isset($_SERVER['name']) ? $_SERVER['name'] : NULL;
			if ($title)
			{
				echo "<h2>";
				echo $title;
				echo "</h2>";
			}
			if ($content)
			{
				echo "<p>";
				echo $content;
				echo "</p>";
			}
			if ($name)
			{
				echo "<p>Written by ";
				echo $name;
				echo "</p>";
			}
		?>
	
  </body>
</html>
