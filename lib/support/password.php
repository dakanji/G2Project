<?php
if (!defined('G2_SUPPORT')) {
	define('G2_SUPPORT_FILE', true);

	include_once __DIR__ . '/lib/support/defaultloc.inc';
}

// Connect to Gallery2
function connect() {
	include_once '../../embed.php';
	$ret = GalleryEmbed::init(
		array(
			'fullInit' => true,
		)
	);

	if (isset($ret)) {
		return '<div class=\"error center\">Could not load Gallery2 API Framework.</center><br>' . $ret->getAsHtml() . '</div>';
	}

	return null;
}

// Process the request
function process(&$process_password_string, &$process_user_name, $process_admin_change, $process_advance, $process_auth) {
	global $gallery, $status, $remember, $caches;

	if (isset($process_password_string)) {
		$storage =& $gallery->getStorage();
		// Empty FailedLoginsMap table
		$sql = 'TRUNCATE [FailedLoginsMap]';
		$ret = $storage->execute($sql);

		// Empty Lock table
		$sql = 'TRUNCATE [Lock]';
		$ret = $storage->execute($sql);

		// Empty RecoverPasswordMap table
		$sql = 'TRUNCATE [RecoverPasswordMap]';
		$ret = $storage->execute($sql);

		// Empty SessionMap table
		$sql = 'TRUNCATE [SessionMap]';
		$ret = $storage->execute($sql);

		// Disable captcha module if active
		list($ret, $moduleStatus) = GalleryCoreApi::fetchPluginStatus('module');

		if (!$ret) {
			if (isset($moduleStatus['captcha']) && !empty($moduleStatus['captcha']['active'])) {
				$sql     = '
				UPDATE
				[PluginMap]
				SET
				[::active] = ?,
				WHERE
				[::pluginId] = ?
				';
				$ret     = $storage->execute($sql, array(0, 'captcha'));
				$captcha = 'off';
			}
		}

		if (isset($process_admin_change)) {
			// Set target username to change admin username to
			$process_user_name = 'Admin';
			// Get the G2 setup password
			$process_password_string = $gallery->getConfig('setup.password');

			if (!isset($process_password_string)) {
				// Bail out if unable to get G2 setup password
				$html .= '<div class="error center">Unable To Reset Admin Details</div>';
			} else {
				// Build and execute the query
				$sql = '
				UPDATE
				[GalleryUser]
				SET
				[GalleryUser::userName] = ?,
				WHERE
				[GalleryUser::id] = ?
				';
				$ret = $storage->execute($sql, array($process_user_name, 6));
			}
		}

		if (GalleryUtilities::strToLower($process_user_name) == 'guest') {
			// Bail out if trying to change Guest password
			$html .= "<div class='error center'>The Guest User Password Cannot be Changed</div>";
		} else {
			// Try to load entity for provided user name
			list($ret, $user) = GalleryCoreApi::fetchUserByUsername($process_user_name);

			if (isset($ret)) {
				// Bail out user does not exist
				$html .= "<div class=\"error center\">Invalid Gallery 2 Username: <i>'" . $process_user_name . "'</i></div>";
			} else {
				// Build and execute the query
				list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($user->getId());
				list($ret, $user)   = $user->refresh();
				GalleryUtilities::unsanitizeInputValues($process_password_string, false);
				$user->changePassword($process_password_string);
				$ret = $user->save();
				$ret = GalleryCoreApi::releaseLocks($lockId);

				if (!isset($ret)) {
					// Prime success flag
					$success = true;
					// Prevent the G2 setup password from being displayed in the form
					$process_password_string = null;
				}
			}
		}

		if ($success == true) {
			// Show success report
			$html = '<div class="success center">Password Updated For User: ' . $process_user_name . '</div>';

			if (isset($_REQUEST['target'])) {
				// Refreseh elements of G2 cache
				foreach ($_REQUEST['target'] as $key => $ignored) {
					// Make sure the dir is legit
					if (!array_key_exists($key, $caches)) {
						$status[] = array('error', "Ignoring illegal cache: $key");

						continue;
					}

					$func       = $caches[$key][1];
					$args[0]    = $caches[$key][2];
					$args[1]    = $caches[$key][3];
					$status     = array_merge($status, call_user_func_array($func, array($args[0], $args[1])));
					$remember[] = $key;
				}
				$_COOKIE['g2pwdcache'] = join(',', $remember);
			}
		}
	} elseif (isset($process_advance) && !isset($process_auth)) {
		// html error message for empty search string if not first page load
		$html .= '<div class="error center">Empty Password String</div>';
	}

	// If no html is not found and this is not not the first page load, it means the udate failed
	if (!isset($html) && isset($process_advance) && !isset($process_auth)) {
		$html = "<div class=\"warning center\">Unable To Update Password For <i>'" . $process_user_name . "'</i></div>";
	}
	// return html output
	return $html;
}

function getCacheDirs() {
	$dirs = array(
		'entity'   => array(true, 'refreshCache', array('cache/entity'), 'album and photo data'),
		'module'   => array(true, 'refreshCache', array('cache/module'), 'module setting'),
		'theme'    => array(true, 'refreshCache', array('cache/theme'), 'theme setting'),
		'template' => array(true, 'refreshCache', array('smarty/templates_c'), 'template'),
		'tmp'      => array(true, 'refreshCache', array('tmp'), 'temporary directory'),
	);

	if (!empty($_COOKIE['g2pwdcache'])) {
		$set = array_flip(explode(',', $_COOKIE['g2pwdcache']));

		foreach ($dirs as $key => $ignored) {
			$dirs[$key][0] = isset($set[$key]);
		}
	}

	return $dirs;
}

function recDelDir($dirname, &$status) {
	if (!file_exists($dirname) || !($fd = opendir($dirname))) {
		return;
	}

	while (($filename = readdir($fd)) !== false) {
		if (!strcmp($filename, '.') || !strcmp($filename, '..')) {
			continue;
		}
		$path = "$dirname/$filename";

		if (is_dir($path)) {
			recDelDir($path, $status);
		} elseif (!@unlink($path)) {
			$status[] = array('error', "Unable to remove cache file: $path");
		}
	}
	closedir($fd);

	if (!@rmdir($dirname)) {
		$status[] = array('error', "Unable to remove cache directory: $dirname");
	}
}

function refreshCache($dir, $mark) {
	global $gallery;
	$path = $gallery->getConfig('data.gallery.base') . $dir;
	recDelDir($path, $status);

	if (@mkdir($path)) {
		$status[] = array('info', "Refreshed $mark cache");
	} else {
		$status[] = array('error', "Unable to refresh $mark cache");
	}

	return $status;
}

function validate() {
	global $gallery, $advance, $authError, $authString;
	$platform =& $gallery->getPlatform();

	if (!isset($advance)) {
		// Generate the auth string on the first visit to this view
		$key = GallerySetupUtilities::generateAuthenticationKey();
		GallerySetupUtilities::setAuthenticationKey($key);
	}

	$authFile   = GALLERY_CONFIG_DIR . '/authFile.txt';
	$authError  = null;
	$authString = GallerySetupUtilities::getAuthenticationKey();

	if (!$platform->file_exists($authFile)) {
		$authError = 'Authentication File Missing';
	} elseif (!$platform->is_readable($authFile)) {
		$authError = 'Authentication File Unreadable';
	} else {
		$authStringFromFile = trim($platform->file_get_contents($authFile));

		if ($authStringFromFile != $authString) {
			$authError = 'Invalid Authentication String';
		}
	}

	return array($authError, $authString);
}

// Set global variables
$status  = $remember  = array();
$captcha = $auth = $authError = $authString = null;

if (isset($_POST['new_password'])) {
	$new_password_string = $_POST['new_password'];
}

if (isset($_POST['user_name'])) {
	$user_name = $_POST['user_name'];
}

if (isset($_POST['admin_change'])) {
	$admin_change = $_POST['admin_change'];
}

if (isset($_POST['advance'])) {
	$advance = true;
}

if (isset($_POST['reset'])) {
	$reset = true;
}

if (isset($_POST['auth'])) {
	$auth = true;
}

// Activate G2 API Framework
$output = connect();
// if connect function returned data, this is an error.
if (!isset($output)) {
	// Validate user
	list($authError, $authString) = validate();
	// Check if authenticated and this is not a reset call
	if (!isset($authError) && !isset($reset)) {
		$caches = getCacheDirs();
		$output = process($new_password_string, $user_name, $admin_change, $advance, $auth);
	}
}
// Deactivate G2 API Framework
GalleryEmbed::done();
?>

<html lang="en">
<head>
	<title>Gallery Support | Password Reset</title>
	<link rel="stylesheet" type="text/css" href="<?php echo $baseUrl; ?>support.css">
</head>
<body>
	<div id="content">
		<div id="title">
			<a href="../../">Gallery</a> &raquo;
			<a href="<?php generateUrl('index.php'); ?>">Support</a> &raquo; Reset Passwords
		</div>
		<h2>
			Set new password for any user.  Can be used to regain access to an administrator
			account when the "forgot password" feature cannot be used due to invalid/missing
			email address or other email problems.
		</h2>
		<div class="center">
			<?php
			if (isset($authError)) {
				?>
				<p class="description">
					<b>Additional Authentication Required</b>
				</p>
				<blockquote>
					<fieldset>
						Please create a file called "authFile.txt" in <br> <?php echo GALLERY_CONFIG_DIR; ?> <br> with the following content:<br>
						<h1><?php echo $authString; ?></h1>
						Click the "Authenticate" button to proceed once you are done.
						<form action="<?php echo basename($_SERVER['PHP_SELF']); ?>" method="POST">
							<input type="hidden" name="auth" value=true>
							<input type="hidden" name="advance" value=true>
							<input type="submit" value="Authenticate">
						</form>
					</fieldset>
				</blockquote>
			</div>
				<?php
				if (isset($advance)) {
					?>
				<hr class="faint">
				<div class="error center">
					<?php echo $authError; ?>
				</div>
					<?php
				} ?>
				<?php
			} elseif (isset($invalidVersion)) {
				?>
			<p class="description">
				<b>Incompatible Gallery2 Version</b>
			</p>
			<blockquote>
				<?php echo $output; ?>
			</blockquote>
		</div>
				<?php
				if (isset($advance)) {
					?>
			<hr class="faint">
			<div class="error center">
						<?php echo $authError; ?>
			</div>
					<?php
				} ?>
				<?php
			} else {
				?>
		<p class="description">
			"Reset Admin" sets admin username to 'Admin' and the admin password to the Gallery 2 setup password
		</p>
		<blockquote>
			<fieldset>
				<form action="<?php echo basename($_SERVER['PHP_SELF']); ?>" method="POST">
				<?php $caches = getCacheDirs(); ?>
				<?php
				foreach ($caches as $key => $info) {
					?>
						<?php
						if (isset($info[0])) {
							?>
							<input type="hidden" name="target[<?php echo $key; ?>]" value=true
							<?php
						} ?>
						>
					<?php
				} ?>
					<input type="hidden" name="advance" value=true><br>
								<?php
								if (isset($user_name)) {
									?>
						<label for="user_name">User Name</label><br><input required type="text" name="user_name" id="user_name" value=<?php echo $user_name; ?>><br>
									<?php
								} else {
									?>
						<label for="user_name">User Name</label><br><input required type="text" name="user_name" id="user_name"  placeholder="User Name"><br>
									<?php
								} ?>
					<?php
					if (isset($new_password_string)) {
						?>
						<label for="new_password">New Password</label><br><input required type="password" name="new_password" id="new_password" value=<?php echo $new_password_string; ?>><br>
						<?php
					} else {
						?>
						<label for="new_password">New Password</label><br><input required type="password"  name="new_password" id="new_password" placeholder="New Password"><br>
						<?php
					} ?>
					<br><input type="submit" value="Change Password">
				</form>
			</fieldset>
			<fieldset>
				<form action="<?php echo basename($_SERVER['PHP_SELF']); ?>" onSubmit="return confirm('Reset admin username and password?');" method="POST">
								<?php $caches = getCacheDirs(); ?>
								<?php
								foreach ($caches as $key => $info) {
									?>
									<?php
									if (isset($info[0])) {
										?>
							<input type="hidden" name="target[<?php echo $key; ?>]" value=true
										<?php
									} ?>
						>
									<?php
								} ?>
					<input type="hidden" name="advance" value=true>
					<input type="hidden" name="admin_change" id="admin_change" value="admin_change">
					<input type="hidden" name="user_name" id="user_name" value="Admin">
					<input type="hidden" name="new_password" id="new_password" value="Admin">
					<input type="submit" value="Reset Admin">
				</form>
			</fieldset>
			<fieldset>
				<form action="<?php echo basename($_SERVER['PHP_SELF']); ?>" method="POST">
					<input type="hidden" name="reset" value=true>
					<input type="hidden" name="advance" value=true>
					<input type="submit" value="Clear Form">
				</form>
			</fieldset>
		</blockquote>
	</div>
				<?php
			}
			?>
<?php
if (isset($output)) {
				?>
	<hr class="faint">
	<?php echo $output; ?>
	<?php
	if (!empty($status)) {
		?>
		<hr class="faint">
		<div class="warning">
			<?php
			foreach ($status as $line) {
				?>
				<pre class="<?php echo $line[0]; ?>"><?php echo $line[1]; ?></pre>
				<?php
			} ?>
		</div>
		<?php
		if (isset($captcha)) {
			?>
			<hr class="faint">
			<div class="warning center">
				The Captcha Module has been deactivated.<br>
				Please reactivate it in the G2 Admin Interface.
			</div>
			<?php
		} ?>
		<?php
	} ?>
	<?php
			}
?>
</div>
</body>
</html>
