<?php
// Verification
if (!defined('G2_SUPPORT_FILE')) {
	die(Illegal Access);
}
/**
* Change URL string that brought us here to the required format.
*
* FROM:    gallery2/lib/support/script_name.php?any_params_that_may_have_been_passed
* TO:      gallery2/lib/support/index.php?script_name&any_params_that_may_have_been_passed
**/

// Get script filename
$fileInfo = pathinfo($_SERVER['PHP_SELF']);
$file     = $fileInfo['filename'] . '.' . $fileInfo['extension'];
// Get original URL string
$oldURL = $_SERVER['REQUEST_URI'];
// Replace script filename with "index.php" and use a placeholder to represent "?" for later
$newURL = str_replace($file, 'index.php{{={placeholder}=}}' . $fileInfo['filename'], $oldURL);
// Replace any "?" in the string with "&"
$newURL = str_replace('?', '&', $newURL);
// Change placeholder to "?" to finalise the URL
$newURL = str_replace('{{={placeholder}=}}', '?', $newURL);


/**
* If the request was a "POST" request, build page with a javascript auto self submitting form
* from PHP's "$_POST" array to submit the params to our new URL.
*
* For others, most likely "GET" requests, use "header location" to redirect to our new URL.
*
* With this, any params attachd to the original request will survive the redirection.
**/
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	$str .= '<!DOCTYPE html><html><head></head>';
	$str .= '<body>';
	$str .= '<form action="' . $newURL . '" id="g2_support_rebounce_form" method="post">';
	foreach ($_POST as $k => $v) {
		$str .= '<input type="hidden" name="' . $k . '" value="' . $v . '">';
	}
	$str .= '</form>';
	$str .= '<script type="text/javascript">';
	$str .= '    var rebounce_now = function () {';
	$str .= '        var frm = document.getElementById("g2_support_rebounce_form");';
	$str .= '        frm.submit();';
	$str .= '    }';
	$str .= '    window.onload = rebounce_now;';
	$str .= '</script>';
	$str .= '</body></html>';
	echo $str;
} else {
	header('Location: ' . $newURL);
}
