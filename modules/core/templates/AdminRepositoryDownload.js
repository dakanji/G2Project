function getLangpackCheckboxes(source) {
    var parentId = source + '_languages';
    var parentEl = document.getElementById(parentId);

    var result = [];
    var inputs = document.getElementsByTagName('input');
    for (var i in inputs) {
	if (inputs[i].parentNode == parentEl) {
	    result.push(inputs[i]);
	}
    }
    return result;
}

function selectAll(source) {
    var checkboxes = getLangpackCheckboxes(source);
    for (var i in checkboxes) {
	checkboxes[i].checked = 'checked';
    }
    document.getElementById(source + '_selectAllLink').style.display = 'none';
    document.getElementById(source + '_selectNoneLink').style.display = 'inline';
}

function selectNone(source) {
    var checkboxes = getLangpackCheckboxes(source);
    for (var i in checkboxes) {
	checkboxes[i].checked = null;
    }
    document.getElementById(source + '_selectAllLink').style.display = 'inline';
    document.getElementById(source + '_selectNoneLink').style.display = 'none';
}

function showLanguagePacks(source) {
    for (var i in allSources) {
	var el = document.getElementById(allSources[i] + '_languagePacks');
	el.style.display = (allSources[i] == source)
	    ? 'block'
	    : 'none';
    }
    var el = document.getElementById('languageListPlaceholder');
    if (el) {
	el.style.display = 'none';
    }
}
