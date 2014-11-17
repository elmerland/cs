function submit_form(method, action, target, key_value) {
	var id = "07892345345087348hjlagafghjldsfhjkl"
	var form  = $('<form />').attr({"id": id, "method": method, "action": action, "target": target, "style":"display:none;"});

	for (var i = 0; i < key_value.length; i += 1) {
		var input = $('<input />').attr({
			type:"hidden",
			name: key_value[i][0],
			value: key_value[i][1],
		});
		form.append(input);
	}

	// Add button to form.

	var button = $('<input />').attr({
		type: "submit"
	});

	form.append(button);

	$("body").append(form);

	button.trigger('click');

	$('#' + id).remove();
}
