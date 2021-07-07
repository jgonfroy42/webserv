<!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<title>Jeancerver 1.0 - Blog</title>
	<script src="https://kit.fontawesome.com/559dfb4a00.js" crossorigin="anonymous"></script>
	<link href="../style.css" rel="stylesheet">
</head>

<body>
	<div id="top_bubble"></div>
	<div id="main">
		<div id="header">
			<h1><i class="fas fa-server yellow"></i> <a href="..">JEANCERVER 1.0</a></h1>
			<div id="menu">
				<a class="left" href="https://developer.mozilla.org/fr/docs/Web/HTTP" target="_blank"><i
						class="fas fa-laptop-code"></i> HTTP by Mozilla</a>
				<a href="https://cdn.intra.42.fr/pdf/pdf/25509/en.subject.pdf" target="_blank" id="main_menu_link"><i
						class="fas fa-file-alt"></i> Sujet</a>
			</div>
		</div>
		<div id="description">
			What a wonderful blog!
		</div>
		<div class="service">
			<?php
			$title = isset($_SERVER['title']) ? $_SERVER['title'] : NULL;
			$content = isset($_SERVER['content']) ? $_SERVER['content'] : NULL;
			$name = isset($_SERVER['author']) ? $_SERVER['author'] : NULL;
			if ($title)
			{
				echo "<div class=\"title\">
				<h2 class=\"yellow\">";
				echo $title;
				echo "</h2>
				</div>";
			}
			if ($content)
			{
				echo "<div class=\"main_blog\"><p>";
				echo $content;
				echo "</p>";
			}
			if ($name)
			{
				echo "<span id=\"author\">Ecrit avec amour par <span>";
				echo $name;
				echo "</span></span>
				</div>";
			}
		?>
		</div>
	</div>

	<div id="footer">
		<div>
			<i class="fas fa-meteor yellow big_icon"></i>
			<p>
				Design inspiré par <a href="https://www.behance.net/gallery/57065493/CAUworking-One-Page"
					target="_blank">Carlos Silva</a>
				<br><br><br><br>Webserv par <i class="fas fa-user-astronaut yellow"></i> amartin-, jgonfroy et celeloup.
				<br></p>

		</div>
	</div>
</body>

</html>