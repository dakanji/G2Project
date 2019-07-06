<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */
if (php_sapi_name() == 'cli') {
	// We do not apply this security check to command-line PHP
	return;
}

if (!defined('G2_SUPPORT_URL_FRAGMENT')) {
	echo 'Error: Missing value for G2_SUPPORT_URL_FRAGMENT';

	exit;
}

require_once __DIR__ . '/GallerySetupUtilities.class';

GallerySetupUtilities::startSession();

// The $baseUrl global is used by other scripts to figure out where to find our CSS
$config = GallerySetupUtilities::getGalleryConfig();

if (!empty($config) && $config['galleryBaseUrl'] != '') {
	$baseUrl = $config['galleryBaseUrl'] . 'lib/support/';
} else {
	$baseUrl = G2_SUPPORT_URL_FRAGMENT;
}

/*
 * If the config, or setup.password is missing, main.php will try to send us to the installer.
 * We do not want that to happen, so check for ourselves first.
 */
$options = array();

if (empty($config)) {
	render('missingConfig', $options);
} elseif (empty($config['setup.password'])) {
	render('missingSetupPassword', $options);
} elseif (!GallerySetupUtilities::isSessionAuthenticated()) {
	$baseDir             = defined('GALLERY_CONFIG_DIR') ? GALLERY_CONFIG_DIR : dirname(dirname(__DIR__));
	$authFile            = $baseDir . '/authFile.txt';
	$options['attempts'] = GallerySetupUtilities::getLoginAttempts();
	$options['error']    = ($options['attempts'] === false);
	$options['authFile'] = sprintf(
		'%s%s%s',
		basename(dirname($authFile)),
		DIRECTORY_SEPARATOR,
		basename($authFile)
	);

	$options['baseDir'] = basename($baseDir) . DIRECTORY_SEPARATOR;
	$options['key']     = GallerySetupUtilities::getAuthenticationKey();

	if (empty($options['key'])) {
		$options['key'] = GallerySetupUtilities::generateAuthenticationKey();

		GallerySetupUtilities::setAuthenticationKey($options['key']);
	}

	if ($options['error'] || $options['attempts'] >= G2_SUPPORT_MAX_LOGIN_ATTEMPTS) {
		if (!file_exists($authFile)) {
			$options['authFileErrors']['missing'] = 1;
		} elseif (!is_readable($authFile)) {
			$options['authFileErrors']['unreadable'] = 1;
		} else {
			$authKeyFromFile = trim(join('', file($authFile)));

			if ($authKeyFromFile == $options['key']) {
				GallerySetupUtilities::authenticateThisSession();
				GallerySetupUtilities::redirectBackToSelf();

				exit;
			}

			$options['authFileErrors']['mismatch'] = 1;
		}

		render('loginTxtForm', $options);
	} elseif (!empty($_POST['password'])) {
		if ($_POST['password'] == $config['setup.password']) {
			GallerySetupUtilities::authenticateThisSession();
			GallerySetupUtilities::redirectBackToSelf();

			exit;
		}

		$options['attempts']++;

		if (!GallerySetupUtilities::setLoginAttempts($options['attempts'])) {
			$options['error'] = true;
		}

		if ($options['error'] || $options['attempts'] >= G2_SUPPORT_MAX_LOGIN_ATTEMPTS) {
			if ($options['attempts'] >= G2_SUPPORT_MAX_LOGIN_ATTEMPTS) {
				$ret = GallerySetupUtilities::notifySiteAdministrator();

				// swallow return code; we cannot do anything with it
			}

			render('loginTxtForm', $options);
		} else {
			$options['wrongPassword'] = 1;
			render('passwordForm', $options);
		}
	} else {
		if (isset($_POST['password'])) {
			$options['missingPassword'] = 1;
		}

		render('passwordForm', $options);
	}
}

if (GallerySetupUtilities::isSessionAuthenticated()) {
	// This is the only safe way out of this include.  Everything else aborts now
	return;
}

// Unless we are properly authenticated, this is the end of the line
exit;

function startForm($action = '', $name = null) {
	echo '<form method="POST" action="' . $action . (isset($name) ? "\" name=\"$name" : '') . '">';

	if (!GallerySetupUtilities::areCookiesSupported()) {
		echo '<input type="hidden" name="' . session_name() . '" value="' . session_id() . '">';
	}
}

function render($renderType, $options = array()) {
	global $baseUrl; ?>

	<!DOCTYPE html>
	<html lang="en">
	<head>
		<title>Security Check</title>
		<link rel="stylesheet" type="text/css" href="<?php echo $baseUrl; ?>support.css">
	</head>
	<body>
		<div class="container">
			<div id="title">
				<a href="../../">Gallery</a> &raquo;
				Support &raquo; Authentication
			</div>
			<h1>
				Restricted Access Area
			</h1>
			<h2>
				Authentication Required
			</h2>
			<?php

			if ($renderType == 'missingConfig') {
				?>
				<div class="alert alert-danger">
					Create a Gallery2 config.php file to continue configuring Gallery2.  Use the <a href="../../install">installer</a> to create one.
				</div>
				<?php
			}

	if ($renderType == 'missingSetupPassword') {
		?>
				<div class="alert alert-danger">
					You must enter a setup password in the config.php file to set up Gallery2. If the config.php file is empty, run the <a href="../../install">installer</a> to install Gallery2.
				</div>
				<?php
	}

	if ($renderType == 'passwordForm') {
		startForm(); ?>
				<div class="alert alert-danger">
					<p>
						Please enter the Gallery2 setup password created during installation.
						<br>
						It can be found in the config.php file:
					</p>
					<br>
					<pre>
						$gallery-&gt;setConfig('setup.password', '<b>YOUR SETUP PASSWORD</b>');
					</pre>
					<br>
					<p>
						<input type="password" name="password" placeholder="Password" required>
						<input type="submit" value="Authenticate" class="btn btn-primary">
						<script>document.forms[0]['password'].focus();</script>
					</p>
				</div>
				<?php

				if (!empty($options['attempts'])) {
					echo '
					<div class="alert alert-warning">
						Failed attempts: ' . $options['attempts'] . ' of ' . G2_SUPPORT_MAX_LOGIN_ATTEMPTS . '
					</div>';
				}

		if (isset($options['missingPassword'])) {
			echo '
					<div class="alert alert-warning">
						Please Enter a Password
					</div>';
		}

		echo '</form>';
	}

	if ($renderType == 'loginTxtForm') {
		startForm();
		echo '
			<p class="alert alert-danger">
				<b>';

		if (!empty($options['error'])) {
			echo 'There was a problem connecting to the database.';
		} else {
			echo 'You have exceeded the maximum attempts to login by entering a password.';
		} ?>
				</b>
			</p>
			<p>
				To prevent unauthorized access, Gallery2 requires addtional  authentication.
				<br>
				You must create a file with a specific name with specific contents and upload this to your Gallery2 installation to authenticate yourself.
			</p>
			<div class="alert alert-info">
				Create this file: <b><?php echo $options['baseDir']; ?>authFile.txt</b>
				<br>
				Put this in the file: <b><?php echo $options['key']; ?></b>
				<br>
				Please upload this to your Gallery2 installation.
			</div>
				<?php

				if (!empty($options['authFileErrors'])) {
					?>
				<div class="alert alert-danger">
					<?php

					if (!empty($options['authFileErrors']['missing'])) {
						?>
						The authFile.txt file does not exist.
						<?php
					} elseif (!empty($options['authFileErrors']['unreadable'])) {
						?>
						The authFile.txt file exists, but is not readable by the webserver.
						<br> Try: <b>chmod 644 authFile.txt</b>
						<?php
					} elseif (!empty($options['authFileErrors']['mismatch'])) {
						?>
						The password in your authFile.txt file does not match the key above!
						<?php
					} ?>
				</div>
					<?php
				} ?>
			<p>
				<?php

				if (!empty($options['error'])) {
					// DA-Note
					// TODO: Why is there a redundant if block?
				} ?>
				<input type="submit" value="Authenticate">
			</p>
		</form>
				<?php
	} ?>
</div>
</body>
</html>
	<?php
}

?>
