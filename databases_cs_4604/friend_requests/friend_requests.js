$(document).ready(init);

function init() {
	populate_requests();
	$('#accept_request').click(accept_request);
	$('#decline_request').click(decline_request);
	$('#back_button').click(go_back);
}

function go_back() {
	var split = location.search.replace('?', '').split('=');
	var kv = [['uid', split[1]]];
	submit_form("GET", "../user/user.php", "", kv);
}

function populate_requests() {
	var split = location.search.replace('?', '').split('=');

	console.log(split[1]);

	var kv = [['uid', split[1]]];
	$.get("friend_requests.php",
		{"function": "get_friends_requests", "uid":split[1]},
		update_request_list);
	$('#request_container').html("Loading...");
}

function update_request_list(result) {
	$('#request_container').html(result);
	$('#request_container table tbody tr').click(select_row);
}

function select_row(event) {
	if ($(this).hasClass("selected")) {
		$(this).removeClass("selected");	
	} else {
		$(this).addClass("selected");
	}
}

function accept_request(event) {
	console.log("accept");
	var selected = $('.selected');
	if (selected.length > 0) {
		$('#responses').css({"display":"table"});	
	}
	for (var i = 0; i < selected.length; i += 1) {
		console.log($(selected[i]).html());
		var uid1 = $($(selected[i]).children()[0]).html();
		var uid2 = $($(selected[i]).children()[1]).html();
		console.log(uid1, uid2);
		$.get("friend_requests.php",
			{"function": "accept_friend", "uid1": uid1, "uid2": uid2},
			function(response) {
				$('#responses tbody').append(response);
				// $('#prompt').html(response);
				populate_requests();
			});
	}
}

function decline_request(event) {
	console.log("decline");
	var selected = $('.selected');
	if (selected.length > 0) {
		$('#responses').css({"display":"table"});	
	}
	for (var i = 0; i < selected.length; i += 1) {
		console.log($(selected[i]).html());
		var uid1 = $($(selected[i]).children()[0]).html();
		var uid2 = $($(selected[i]).children()[1]).html();
		console.log(uid1, uid2);
		$.get("friend_requests.php",
			{"function": "decline_friend", "uid1": uid1, "uid2": uid2},
			function(response) {
				$('#responses tbody').append(response);
				// $('#prompt').html(response);
				populate_requests();
			});
	}
}
