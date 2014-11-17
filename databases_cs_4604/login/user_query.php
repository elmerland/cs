<?php
	include "../util.php";
	$first_name = $_GET["first_name"];
	$last_name = $_GET["last_name"];
	$display_count = 10;

	// Query for the total number of matches.
	$query1 = "SELECT COUNT(first_name) FROM userinfo WHERE LOWER(first_name) LIKE '%".$first_name."%' AND LOWER(last_name) LIKE '%".$last_name."%';";
	// Query for the name matches.
	$query2 = "SELECT uid, first_name, last_name FROM userinfo WHERE LOWER(first_name) LIKE '%".$first_name."%' AND LOWER(last_name) LIKE '%".$last_name."%' LIMIT ".$display_count.";";

	// Run both queries.
	$result1 = get_query($query1, false);
	$result2 = get_query($query2, false);

	if (!$result1 || !$result2) {
		echo '<span class="no_matches">No matches found</span>';
	} else {
		// Print queries in correct format.
		print_user_name_results($display_count, $result1, $result2);
	}
?>