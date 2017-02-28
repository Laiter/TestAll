<?php 
	echo '<div id="news">';
	echo '<h3>Последние новости</h3>';
	echo '<hr>';
	echo '<ul class="news-list" id="articles">';
	$resNews = mysqli_query($db, "SELECT * FROM `content` WHERE `fixed` = 0 ORDER BY `id` DESC LIMIT 0,10");
	
	
	while($rowNews = mysqli_fetch_assoc($resNews)) {
		echo '<li class="news-post">';
		echo '<div class="news-info">';
		echo '<h1><a href="news.php?id=' . $rowNews['id'] . '">' . $rowNews['title'] . " №" . $rowNews['id'] . '</a></h1>';
		echo '<p>' . $rowNews['cut'] . '</p>';
		echo '</div>';
		echo '<hr>';
		echo '</li>';
	}
	echo '</ul>';
	echo '</div>';
	echo '</div>';	
?>
