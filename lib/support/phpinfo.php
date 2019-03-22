<?php

if (!defined('G2_SUPPORT')) {
	define('G2_SUPPORT_FILE', true);

	include_once __DIR__ . '/defaultloc.inc';
}

// Connect to Gallery2
require_once '../../embed.php';

$ret = GalleryEmbed::init(
	array(
		'fullInit' => true,
	)
);

if (isset($ret)) {
	echo '<center><h1>Could not load Gallery2 API Framework</h1></center>' . $ret->getAsHtml();

	exit;
}

// Get Path to G2Data
$g2data_path = $gallery->getConfig('data.gallery.base');

if (function_exists('posix_geteuid')) {
	// use posix to get current uid and gid
	$uid   = posix_geteuid();
	$usr   = posix_getpwuid($uid);
	$user  = $usr['name'];
	$gid   = posix_getegid();
	$grp   = posix_getgrgid($gid);
	$group = $grp['name'];
} else {
	// try to read some ids
	$tmp = tempnam(sys_get_temp_dir(), 'phpinfotmp');
	$uid = fileowner($tmp);
	$gid = filegroup($tmp);

	// try to run ls on it
	$out   = `ls -l $tmp`;
	$lst   = explode(' ', $out);
	$user  = $lst[2];
	$group = $lst[3];
	unlink($tmp);
}

//try to read g2data permissions
$g2data_path = trim($g2data_path);

if (substr($g2data_path, -1) != '/') {
	$g2data_path .= '/';
}

if (substr($g2data_path, 0, 1) != '/') {
	$g2data_path = '/' . $g2data_path;
}

if (file_exists($g2data_path)) {
	$g2_tmp = $g2data_path . 'cache/module/core/0/0/0.inc';

	if (file_exists($g2_tmp)) {
		$data_path = true;
		$g2_uid    = fileowner($g2_tmp);
		$g2_gid    = filegroup($g2_tmp);
		$g2_out    = `ls -l $g2_tmp`;
		$g2_lst    = explode(' ', $g2_out);
		$g2_user   = $g2_lst[2];
		$g2_group  = $g2_lst[3];
	}
}

if (!isset($data_path)) {
	$g2_uid = $g2_gid = $g2_out = $g2_lst = $g2_user = $g2_group = 'No Data';
}

if (array_key_exists('_', $_SERVER)) {
	$php_handler = $_SERVER['_'];
} else {
	$php_handler = 'Gallery 2 could not determine the PHP Handler';
}

ob_start();
phpinfo();
$phpinfo = ob_get_contents();
ob_clean();

//<link rel="stylesheet" type="text/css" href="support.css">
$phpinfo  = str_replace('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml1-transitional.dtd">', '', $phpinfo);
$phpinfo  = str_replace('<html xmlns="http://www.w3.org/1999/xhtml">', '<html lang="en">', $phpinfo);
$phpinfo  = str_replace('<title>phpinfo()</title>', '<title> Gallery Support | PHP Info</title>', $phpinfo);
$styleStr = '

/*!
 * bootswatch v3.3.7
 * Homepage: http://bootswatch.com
 * Copyright 2012-2016 Thomas Park
 * Licensed under MIT
 * Based on Bootstrap
 */

/*!
 * Bootstrap v3.3.7 (http://getbootstrap.com)
 * Copyright 2011-2016 Twitter, Inc.
 * Licensed under MIT (https://github.com/twbs/bootstrap/blob/master/LICENSE)
 */
*,
*:before,
*:after {
	box-sizing: inherit;
	vertical-align: baseline;
}

html {
	box-sizing: border-box;
	font: 16px/1.25 -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol";
	font-weight: 200;
	-webkit-tap-highlight-color: rgba(0, 0, 0, 0);
}

body {
	background: #ffffff;
	color: #77777;
	margin: 10px auto;
}

/*! normalize.css v3.0.3 | MIT License | github.com/necolas/normalize.css */
a {
	background-color: transparent;
}

a:active,
a:hover {
	outline: 0;
}

html input[disabled] {
	cursor: default;
}

a:hover,
a:focus {
	color: #0a6ebd;
	text-decoration: underline;
}

a:focus {
	outline: 5px auto -webkit-focus-ring-color;
	outline-offset: -2px;
}

.container {
	margin-right: auto;
	margin-left: auto;
	padding-left: 15px;
	padding-right: 15px;
}

@media (min-width: 768px) {
	.container {
		width: 750px;
	}
}

@media (min-width: 992px) {
	.container {
		width: 970px;
	}
}

@media (min-width: 1200px) {
	.container {
		width: 1170px;
	}
}

.container:before,
.container:after,
.form-horizontal .form-group:before,
.form-horizontal .form-group:after,
.panel-body:before,
.panel-body:after {
	content: " ";
	display: table;
}

.container:after,
.form-horizontal .form-group:after,
.panel-body:after {
	clear: both;
}

@-ms-viewport {
	width: device-width;
}

body {
	-webkit-font-smoothing: antialiased;
	letter-spacing: .1px;
}

p {
	margin: 0 0 1em;
}

/*!
 * Hydrogen CSS
 * Copyright 2018 Pim Brouwers
 * Licensed under MIT
 *https://github.com/pimbrouwers/hydrogen
 */
a {
	color: #0080ff;
	font-weight: 300;
	text-decoration: none;
}

a:hover,
a:focus {
	color: #0a6ebd;
	text-decoration: underline;
}

a:focus {
	outline: 5px auto -webkit-focus-ring-color;
	outline-offset: -2px;
}

';

$phpinfo   = str_replace('</style>', $styleStr . '</style>', $phpinfo);
$htmlData  = "<div id='title'>";
$htmlData .= "<a href='../../'>Gallery</a> &raquo; ";
$htmlData .= "<a href='../support/index.php'>Support</a> &raquo; PHP Info";
$htmlData .= '</div>';
$phpinfo   = str_replace('<div class="center">', '<div class="container">' . $htmlData . '<div class="center">', $phpinfo);
$htmlData  = "<h2><a name='additional_data'>Gallery 2 User Data</a></h2>";
$htmlData .= "<table border='0' cellpadding='3' width='600'>";
$htmlData .= "<tr class='h'><th>Item</th><th>G2Data Folder</th><th>PHP User</th></tr>";
$htmlData .= "<tr><td class='e'>User Name</td><td class='v'>" . $g2_user . "</td><td class='v'>" . $user . '</td></tr>';
$htmlData .= "<tr><td class='e'>User ID</td><td class='v'>" . $g2_uid . "</td><td class='v'>" . $uid . '</td></tr>';
$htmlData .= "<tr><td class='e'>Group Name</td><td class='v'>" . $g2_group . "</td><td class='v'>" . $group . '</td></tr>';
$htmlData .= "<tr><td class='e'>Group ID</td><td class='v'>" . $g2_gid . "</td><td class='v'>" . $gid . '</td></tr>';
$htmlData .= '</table>';
$htmlData .= '<center>';
$htmlData .= "<h2><a name='additional_data'>Server Information</a></h2>";
$htmlData .= "<table border='0' cellpadding='3' width='600'>";
$htmlData .= "<tr><td class='e'>Webserver</td><td class='v'>" . $_SERVER['SERVER_SOFTWARE'] . '</td></tr>';
$htmlData .= "<tr><td class='e'>Architecture</td><td class='v'>" . @php_uname(m) . '</td></tr>';
$htmlData .= "<tr><td class='e'>PHP Handler</td><td class='v'>" . $php_handler . '</td></tr>';
$htmlData .= '</table>';
$htmlData .= '<br>';
$phpinfo   = str_replace('<div class="center">', '<div class="center">' . $htmlData, $phpinfo);
$phpinfo   = str_replace('</body>', '</div></body>', $phpinfo);
$phpinfo   = str_replace('body {background-color: #fff; color: #222; font-family: sans-serif;}', '', $phpinfo);
$phpinfo   = str_replace('a:link {color: #009; text-decoration: none; background-color: #fff;}', '', $phpinfo);
$phpinfo   = str_replace('a:hover {text-decoration: underline;}', '', $phpinfo);

//output
echo $phpinfo;
