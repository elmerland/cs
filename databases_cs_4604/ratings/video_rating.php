<?php
	include "../util.php";
	$function = $_GET['function'];

	if ($function == "get_video_matches") {
		$title = $_GET["title"];
		$display_count = 15;
		return get_video_matches($title, $display_count);
	} else if ($function == "submit_rating") {
		$vid = $_GET['vid'];
		$uid = $_GET['uid'];
		$rating = $_GET['rating'];
		return submit_rating($vid, $uid, $rating);
	}
	
	function get_video_matches($title, $display_count) {
		// Query for the total number of matches.
		$query1 = "SELECT COUNT(title) FROM videoinfo WHERE LOWER(title) LIKE '%".$title."%';";
		// Query for the name matches.
		$query2 = "SELECT vid, title, release_year, certification_name FROM videoinfo WHERE LOWER(title) LIKE '%".$title."%' LIMIT ".$display_count.";";
		
		// Run both queries.
		$result1 = get_query($query1, false);
		$result2 = get_query($query2, false);
		
		if (!$result1 || !$result2) {
			echo '<span class="no_matches">No matches found</span>';
		} else {
			// Print queries in correct format.
			return print_videoinfo_results($display_count, $result1, $result2);
		}
	}

	function submit_rating($vid, $uid, $rating) {
		// echo $vid." ".$uid." ".$rating;

		$query = "insert into ratings values(".$vid.", ".$uid.", ".$rating.", current_time)";

		// echo $query;
		// return;

		$result = get_query($query, true);

		if (!$result) {
			echo "<tr><td>".$vid."</td><td>".$rating."</td><td>Could not submit rating</td></tr>";
		} else {
			echo "<tr><td>".$vid."</td><td>".$rating."</td><td>Rating submitted</td></tr>";
		}
	}
	
?>