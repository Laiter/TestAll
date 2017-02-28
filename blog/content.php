<?php
	include 'db.php';
	
	$resFix = mysqli_query($db, "SELECT * FROM `content` WHERE `fixed` = 1 ORDER BY `id` DESC");
	$rowFix = mysqli_fetch_assoc($resFix);	
	echo '<div id="content">';
	echo '<div id="cards">';
	echo '<div class="fix-card-p">';
	echo '<div class="fix-card-c">';
	echo '<a href="news.php?id=' . $rowFix['id'] . '"><img src = "' . $rowFix['img-580x600'] . '"></a>';
	echo '<a class="card-title" href="news.php?id=' . $rowFix['id'] . '">' . $rowFix['title'] . '</a>';
	echo '</div>';
	echo '</div>';
	echo '<div id="miniScroller">';
	
	$resMain = mysqli_query($db, "SELECT * FROM `content` WHERE `fixed` = 0 ORDER BY `id` DESC LIMIT 0,16");
	
	while($rowMain = mysqli_fetch_assoc($resMain)) {
		echo '<div class="main-card-c">';
		echo '<div class="main-card-c-1">';
		echo '<a href="news.php?id=' . $rowMain['id'] . '"><img src = "' . $rowMain['img-280x290'] . '"></a>';
		echo '<a class="card-title" href="news.php?id=' . $rowMain['id'] . '">' . $rowMain['title'] . '</a>';
		echo '</div>';
		next($rowMain = mysqli_fetch_assoc($resMain));
		echo '<div class="main-card-c-2">';
		echo '<a href="news.php?id=' . $rowMain['id'] . '"><img src = "' . $rowMain['img-280x290'] . '"></a>';
		echo '<a class="card-title" href="news.php?id=' . $rowMain['id'] . '">' . $rowMain['title'] . '</a>';
		echo '</div>';
		echo '</div>';
	}
	echo '</div>';
	echo '</div>';
	
	include 'list-news.php';
	include 'footer.php';
?>