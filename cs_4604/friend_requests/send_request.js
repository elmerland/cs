
$(document).ready(init);

var first_name_val;
var last_name_val;
var uid_selected;

function init() {
	$('.user_name_search').keyup(search_handler);
	$('#send_request').click(send_request);
	$('#back_button').click(go_back);
}

function go_back(event) {
	console.log("Go back");
	event.preventDefault();
	var split = location.search.replace('?', '').split('=');
	var kv = [['uid', split[1]]];
	submit_form("GET", "../user/user.php", "", kv);
}

function search_handler(event) {
	$('#search_results').css({'display':'block'});
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
		$.get("../login/user_query.php",
			{"first_name":first_name, "last_name":last_name},
			update_suggestions);
		$('#search_results').html("Loading...");
	}	
}

function update_suggestions(result) {
	// console.log("Result: " + result);
	$('#search_results').html(result);

	$('#search_results tbody tr').click(user_selected);
}

function user_selected(event) {
	uid_selected = $($(this).find('.userinfo_uid')[0]).html();
	var f_name = $($(this).find('.userinfo_first_name')[0]).html();
	var l_name = $($(this).find('.userinfo_last_name')[0]).html();

	$('#user_first_name').val(f_name);
	$('#user_last_name').val(l_name);
	$('#search_results').css({'display':'none'});
}

function send_request() {
	if (!uid_selected) {
		console.log("Nothing selected");
		return;
	}
	var split = location.search.replace('?', '').split('=');
	var uid1 = split[1];
	var uid2 = uid_selected;

	$.get("friend_requests.php",
		{"function": "send_friend_request", "uid1": uid1, "uid2": uid2},
		handle_response);
	uid_selected = undefined;
}

function handle_response(response) {
	$('#responses').css({'display':'table'});
	$('#responses tbody').append(response);
}
