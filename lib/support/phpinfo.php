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

//output
echo '<!DOCTYPE html>';
echo "<html lang='en'>";
echo '<head>';
echo '<title> Gallery Support | PHP Info</title>';
echo '</head>';
echo '<body>';
echo "<div id='title'>";
echo "<a href='../../'>Gallery</a> &raquo; ";
echo "<a href='../support/index.php'>Support</a> &raquo; PHP Info";
echo '</div>';

echo '<center>';
echo "<h2><a name='additional_data'>User Data</a></h2>";
echo "<table border='0' cellpadding='3' width='600'>";
echo "<tr class='h'><th>Item</th><th>G2Data Folder</th><th>PHP User</th></tr>";
echo "<tr><td class='e'>User Name</td><td class='v'>" . $g2_user . "</td><td class='v'>" . $user . '</td></tr>';
echo "<tr><td class='e'>User ID</td><td class='v'>" . $g2_uid . "</td><td class='v'>" . $uid . '</td></tr>';
echo "<tr><td class='e'>Group Name</td><td class='v'>" . $g2_group . "</td><td class='v'>" . $group . '</td></tr>';
echo "<tr><td class='e'>Group ID</td><td class='v'>" . $g2_gid . "</td><td class='v'>" . $gid . '</td></tr>';
echo '</table>';
echo '<center>';
echo "<h2><a name='additional_data'>Server Information</a></h2>";
echo "<table border='0' cellpadding='3' width='600'>";
echo "<tr><td class='e'>Webserver</td><td class='v'>" . $_SERVER['SERVER_SOFTWARE'] . '</td></tr>';
echo "<tr><td class='e'>Architecture</td><td class='v'>" . @php_uname(m) . '</td></tr>';
echo "<tr><td class='e'>PHP Handler</td><td class='v'>" . $php_handler . '</td></tr>';
echo '</table>';
echo '</center><br>';
echo '<body>';
echo '</html>';
phpinfo();
