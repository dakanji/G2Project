var search_SearchBlock_prompt;
var search_SearchBlock_error;
var search_SearchBlock_input;

function search_SearchBlock_init(prompt, error) {
    search_SearchBlock_promptString = prompt;
    search_SearchBlock_errorString = error;
    search_SearchBlock_input = document.getElementById('search_SearchBlock').searchCriteria;

    search_SearchBlock_input.value = prompt;
}

function search_SearchBlock_checkForm() {
    var sc = search_SearchBlock_input.value;
    if (sc == search_SearchBlock_promptString || sc == '') {
	alert(search_SearchBlock_errorString);
	return false;
    } else {
	document.getElementById('search_SearchBlock').submit();
	return true;
    }
}

function search_SearchBlock_focus() {
    if (search_SearchBlock_input.value == search_SearchBlock_promptString) {
	search_SearchBlock_input.value = '';
    }
}

function search_SearchBlock_blur() {
    if (search_SearchBlock_input.value == '') {
	search_SearchBlock_input.value = search_SearchBlock_promptString;
    }
}
