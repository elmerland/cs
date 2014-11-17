$(document).ready(init);

var video_title_val;

var rating;
var vid_selected;

function init() {
	$('#video_title').keyup(search_handler);
	$('#submit_rating').click(submit_rating);
	$('#back_button').click(go_back);
}

function go_back() {
	var split = location.search.replace('?', '').split('=');
	var kv = [['uid', split[1]]];
	submit_form("GET", "../user/user.php", "", kv);
}

function search_handler(event) {
	$('#search_results').css({'display':'block'});
	var video_title = $('#video_title').val().toLowerCase().trim();

	if (video_title === video_title_val) {
		return;
	} else {
		video_title_val = video_title;
	}

	if (video_title === "") {
		$('#search_results').html('<span class="no_matches">No matches found</span>');
	} else {
		$.get("video_rating.php",
			{"function": "get_video_matches", "title":video_title},
			update_suggestions);
		$('#search_results').html("Loading...");
	}	
}

function update_suggestions(result) {
	// console.log("Result: " + result);
	$('#search_results').html(result);

	$('#search_results tbody tr').click(select_video);
}

function select_video(event) {
	vid_selected = $($(this).find('.videoinfo_vid')[0]).html();
	var title = $($(this).find('.videoinfo_title')[0]).html();

	$('#video_title').val(title);
	$('#search_results').css({'display':'none'});
}

function submit_rating() {
	var rating = $('input[name=rating]:checked').val();
	var uid = location.search.replace('?', '').split('=')[1];
	var vid = vid_selected;

	if (!vid_selected) {
		console.log("No video selected");
		return;
	}

	if (!rating) {
		alert("Please select a rating!");
		return;
	}

	if (typeof(rating) === "undefined") {
		alert("Please select a rating");
		return;
	}

	console.log(vid, uid, rating);

	$.get("video_rating.php",
		{"function": "submit_rating", "vid":vid, "uid": uid, "rating":rating},
		handle_response);
}

function handle_response(response) {
	$('#responses').css({'display':'table'});
	$('#responses tbody').append(response);
}