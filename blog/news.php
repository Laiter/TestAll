<?php
	include 'db.php';
	include 'header.html';
	
	$resNew = mysqli_query($db, "SELECT * FROM `content` WHERE `id` = {$_GET["id"]}");
	$rowNew = mysqli_fetch_assoc($resNew);
	echo '<br>';
	echo '<div id="content">';
	echo '<h2>' . $rowNew['title'] . '</h2>';
	
	echo '<div id="news">';
	echo '<ul class="news-list" id="articles">';
	echo '<li class="news-post">';
	echo '<div class="news-info">';
	echo '<img src = "' . $rowNew['img-full'] . '">';
	echo '<hr>';
	echo '<p>' . $rowNew['content'] . '</p>';
	echo '</div>';
	echo '<hr>';
	echo '</li>';
	echo '</ul>';
	echo '</div>';
	echo '</div>';	
?>

</body>
</html>