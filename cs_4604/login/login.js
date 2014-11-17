
$(document).ready(init);

var first_name_val;
var last_name_val;

function init() {
	$('.user_name_search').keyup(search_handler);
}

function search_handler(event) {
	var first_name = $('#user_first_name').val().toLowerCase().trim();
	var last_name = $('#user_last_name').val().toLowerCase().trim();

	if (first_name === first_name_val && last_name === last_name_val) {
		return;
	} else {
		first_name_val = first_name;
		last_name_val = last_name;
	}

	if (first_name === "" && last_name === "") {
		$('#search_results').html('<span class="no_matches">No matches found</span>');
	} else {
		$.get("user_query.php",
			{"first_name":first_name, "last_name":last_name},
			update_suggestions);
		$('#search_results').html("Loading...");
	}	
}

function update_suggestions(result) {
	// console.log("Result: " + result);
	$('#search_results').html(result);

	$('#search_results tr').click(user_selected);
}

function user_selected(event) {
	var uid = $($(this).find('.userinfo_uid')[0]).html()
	console.log("User id: " + uid);
	var kv = [['uid', uid]];
	get_page(kv);
}

function get_page(key_value) {
	submit_form("GET", "../user.php", "_blank", key_value);
}
