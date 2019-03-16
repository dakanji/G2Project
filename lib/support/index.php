<?php
define('G2_SUPPORT_URL_FRAGMENT', '');

require_once __DIR__ . '/security.inc';

ob_start();

// Tell other scripts we passed security.inc ok
define('G2_SUPPORT', true);

if (!empty($_SERVER['QUERY_STRING'])) {
	foreach (array(
		'phpinfo',
		'cache',
		'gd',
		'chmod',
		'import',
		'password',
		'search_db',
		'missingObjectFix',
	) as $script) {
		/*
		 * Do not use isset($_GET[$script]) since we want to allow for GET args could collide
		 * with the above mentioned script names
		 */
		if ($_SERVER['QUERY_STRING'] == $script
			|| strncmp($_SERVER['QUERY_STRING'], $script . '&', strlen($script) + 1) == 0
		) {
			include __DIR__ . '/' . $script . '.php';

			$results = ob_get_contents();
			ob_end_clean();
			echo $results;

			return;
		}
	}
}

function generateUrl($uri, $print = true) {
	// If session.use_trans_sid is on then it will add the session id.
	if (!GallerySetupUtilities::areCookiesSupported()
		&& !ini_get('session.use_trans_sid')
	) {
		$sid  = session_name() . '=' . session_id();
		$uri .= (!strpos($uri, '?') ? '?' : '&amp;') . $sid;
	}

	if ($print) {
		echo $uri;
	}

	return $uri;
}

?>
<!DOCTYPE html>
<html lang="en">
<head>
	<title>Gallery Support</title>
	<link rel="stylesheet" type="text/css" href="<?php echo $baseUrl; ?>support.css">
</head>
<body>
	<div class="container">
		<div id="title">
			<a href="../../">Gallery</a> &raquo; Support
		</div>
		<h1>
			Gallery2 Support Tools
		</h1>
		<h2>
			A Collection of Tools for Troubleshooting Gallery 2
		</h2>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					PHP Info Tool
				</h3>
			</div>
			<div class="panel-body">
				View information about your PHP configuration.
				<br><br>
				<a href="<?php generateUrl('index.php?phpinfo'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					Cache Maintenance Tool
				</h3>
			</div>
			<div class="panel-body">
				Delete files in the Gallery2 data cache.
				<br>
				Gallery2 caches data on disk to improve performance. These caches can get out of date and need to be deleted.
				<br><br>
				<a href="<?php generateUrl('index.php?cache'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					Filesystem Permissions Tool
				</h3>
			</div>
			<div class="panel-body">
				Change the filesystem permissions of your gallery and storage folders.
				<br><br>
				<a href="<?php generateUrl('index.php?chmod'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					GD Info Tool
				</h3>
			</div>
			<div class="panel-body">
				Get information about your GD configuration.
				<br><br>
				<a href="<?php generateUrl('index.php?gd'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					Import Database Tool
				</h3>
			</div>
			<div class="panel-body">
				Restore your Gallery database from an export that was made from the site administration maintenance screen or from the Database Backup Step of the Gallery2 Upgrader.
				<br><br>
				<a href="<?php generateUrl('index.php?import'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					Reset User Password Tool
				</h3>
			</div>
			<div class="panel-body">
				Change or Reset Passwords. Can be used to regain access to an administrator
				account when the "forgot password" feature cannot be used.
				<br><br>
				<a href="<?php generateUrl('index.php?password'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					Search Database Tool
				</h3>
			</div>
			<div class="panel-body">
				Search the Gallery2 database.
				<br><br>
				<a href="<?php generateUrl('index.php?db'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
		<div class="panel panel-info">
			<div class="panel-heading">
				<h3 class="panel-title">
					Fix Missing Object Tool
				</h3>
			</div>
			<div class="panel-body">
				Fix "ERROR_MISSING_OBJECT" Error Messages.
				<br><br>
				<a href="<?php generateUrl('index.php?missingObjectFix'); ?>" class="btn btn-primary">Continue</a>
			</div>
		</div>
	</div>
</body>
</html>
