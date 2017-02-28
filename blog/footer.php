<!--
<div id="footer">
	© 2016 <a href=#> Laiter</a>
</div>
-->

	
	<!-- LOAD JAVASCRIPT LATE - JUST BEFORE THE BODY TAG 
		 That way the browser will have loaded the images
	  	 and will know the width of the images. No need to
		 specify the width in the CSS or inline. -->

	<!-- jQuery library - Please load it from Google API's -->
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js" type="text/javascript"></script>
	
	<!-- jQuery UI Widget and Effects Core (custom download)
		 You can make your own at: http://jqueryui.com/download -->
	<script src="js/jquery-ui-1.8.23.custom.min.js" type="text/javascript"></script>
	
	<!-- Latest version of jQuery Mouse Wheel by Brandon Aaron
		 You will find it here: http://brandonaaron.net/code/mousewheel/demos -->
	<script src="js/jquery.mousewheel.min.js" type="text/javascript"></script>

	<!-- jQuery Kinetic - for touch -->
	<script src="js/jquery.kinetic.js" type="text/javascript"></script>
	<!-- Smooth Div Scroll 1.3 minified-->
	<script src="js/jquery.smoothdivscroll-1.3-min.js" type="text/javascript"></script>

	<!-- If you want to look at the uncompressed version you find it at
		 js/jquery.smoothDivScroll-1.3.js -->
		 
	<!-- Ajax -->
	
	<script type="text/javascript" src="js/scripts.js"></script>
	
	<!-- Plugin initialization -->
	
	
	<script type="text/javascript">
		$(document).ready(function () {
			$("#miniScroller").smoothDivScroll({
				hotSpotScrolling: false,
				visibleHotSpotBackgrounds: "",
				mousewheelScrollingStep: 45,
				mousewheelScrolling: "allDirections",
				touchScrolling: true
			});
		});
	</script>
</body>
</html>