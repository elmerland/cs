
$(document).ready(init);

var first_name_val;
var last_name_val;
var uid_selected;

var ignore_change = false;

function init() {
	$('.user_name_search').keyup(search_handler);
	$('#submit_button').click(get_page);
}

function search_handler(event) {
	if (ignore_change) {
		ignore_change = false;
		return;
	}
	var first_name = $('#first_name').val().toLowerCase().trim();
	var last_name = $('#last_name').val().toLowerCase().trim();

	if (first_name === first_name_val && last_name === last_name_val) {
		return;
	} else {
		first_name_val = first_name;
		last_name_val = last_name;
	}

	if (first_name === "" && last_name === "") {
		$('#pop_over').css({"display": "none"});
	} else {
		$('#pop_over').css({"display": "block"})
		$.get("login/user_query.php",
			{"first_name":first_name, "last_name":last_name},
			update_suggestions);
		$('#pop_over').html("Loading...");
	}	
}

function update_suggestions(result) {
	// console.log("Result: " + result);
	$('#pop_over').html(result);

	$('#pop_over tr').click(user_selected);
}

function user_selected(event) {
	uid_selected = $($(this).find('.userinfo_uid')[0]).html()
	console.log("User id: " + uid_selected);

	var f_name = $($(this).find('.userinfo_first_name')[0]).html()
	var l_name = $($(this).find('.userinfo_last_name')[0]).html()
	// ignore_change = true;
	$('#first_name').val(f_name);
	$('#last_name').val(l_name);
	$('#pop_over').css({"display": "none"});
}

function get_page(event) {
	event.preventDefault();
	var key_value = [['uid', uid_selected]];
	submit_form("GET", "user/user.php", "", key_value);
}
