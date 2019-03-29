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

/**
 * Perform all necessary initialization of the Gallery framework
 * @package Gallery
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 20968 $
 */
require_once __DIR__ . '/modules/core/classes/GalleryUtilities.class';

function GalleryInitFirstPass($params = array()) {
	global $gallery;

	ini_set('magic_quotes_runtime', 0);
	ini_set('magic_quotes_sybase', 0);
	$script_tz = date_default_timezone_get();
	@date_default_timezone_set($script_tz);

	// Specify that when an assertion fails, we terminate right away
	assert_options(ASSERT_WARNING, 1);
	assert_options(ASSERT_BAIL, 1);

	// Load all the core Gallery classes
	$galleryBase = __DIR__ . '/';

	/*
	 * Conditional class loading confuses APC resulting in
	 *   "Cannot redeclare class gallerycoreapi"
	 * log spam.  Only load GalleryCoreApi if it is not loaded.
	 */
	if (!class_exists('GalleryCoreApi')) {
		include_once $galleryBase . 'modules/core/classes/GalleryCoreApi.class';
	}

	GalleryCoreApi::requireOnce('modules/core/classes/GalleryConstants.class');
	GalleryCoreApi::requireOnce('modules/core/classes/GalleryCapabilities.class');
	GalleryCoreApi::requireOnce('modules/core/classes/GalleryView.class');
	GalleryCoreApi::requireOnce('modules/core/classes/GalleryModule.class');

	// Extra hack code for APC: preload some common classes.
	if (function_exists('apc_add')) {
		GalleryCoreApi::requireOnce('modules/core/classes/GalleryPhotoItem.class');
		GalleryCoreApi::requireOnce('modules/core/classes/GalleryMovieItem.class');
	}

	if (!strncasecmp(PHP_OS, 'win', 3)) {
		GalleryCoreApi::requireOnce('modules/core/classes/GalleryPlatform/WinNtPlatform.class');

		$platform = new WinNtPlatform();
	} else {
		GalleryCoreApi::requireOnce('modules/core/classes/GalleryPlatform/UnixPlatform.class');

		$platform = new UnixPlatform();
	}

	$gallery->setPlatform($platform);

	$slash = $platform->getDirectorySeparator();

	if (isset($params['debug'])) {
		$gallery->setDebug($params['debug']);
	}

	// Sanitize the data path
	$dataBase = $gallery->getConfig('data.gallery.base');

	if ($dataBase[strlen($dataBase) - 1] != $slash) {
		$dataBase .= $slash;

		$gallery->setConfig('data.gallery.base', $dataBase);
	}

	// Init for downloadable plugins. Allow config.php to override.
	$repositoryUrl = @$gallery->getConfig('repository.url');

	if (empty($repositoryUrl)) {
		$gallery->setConfig('repository.url', 'http://gallery.menalto.com/repository/');
	}

	$gallery->setConfig('repository.cache', $dataBase . 'cache' . $slash . 'repository' . $slash);

	// Set our various data paths
	$gallery->setConfig('data.gallery.cache', $dataBase . 'cache' . $slash);
	$gallery->setConfig('data.gallery.albums', $dataBase . 'albums' . $slash);
	$gallery->setConfig('data.gallery.locks', $dataBase . 'locks' . $slash);
	$gallery->setConfig('data.gallery.tmp', $dataBase . 'tmp' . $slash);
	$gallery->setConfig('data.smarty.base', $dataBase . 'smarty' . $slash);
	$gallery->setConfig('data.smarty.templates_c', $dataBase . 'smarty' . $slash . 'templates_c' . $slash);
	$gallery->setConfig('data.gallery.plugins', $galleryBase . 'plugins' . $slash);
	$gallery->setConfig('data.gallery.plugins_data', $dataBase . 'plugins_data' . $slash);
	$gallery->setConfig('data.gallery.version', $dataBase . 'versions.dat');
	$gallery->setConfig('data.gallery.backup', $dataBase . 'backups' . $slash);
	$gallery->setConfig('data.gallery.locale', $dataBase . 'locale' . $slash);

	// Configure our url generator
	if (!isset($params['noDatabase'])) {
		/*
		 * Swallow error to prevent GalleryFactoryHelper_loadRegistry cache from breaking upgrade to
		 * core 1.0.6
		 */
		list($ret, $urlGenerator) = @GalleryCoreApi::newFactoryInstance('GalleryUrlGenerator');

		// Swallow ERROR_STORAGE_FAILURE, or automatic upgrading fails
		if ($ret && !($ret->getErrorCode() & ERROR_STORAGE_FAILURE)) {
			return $ret;
		}
	}

	if (!isset($urlGenerator)) {
		GalleryCoreApi::requireOnce('modules/core/classes/GalleryUrlGenerator.class');

		$urlGenerator = new GalleryUrlGenerator();
	}

	// Allow for overrides from GalleryEmbed ($param) or from config.php
	$configBaseUri = @$gallery->getConfig('baseUri');

	if (empty($configBaseUri)) {
		$configBaseUri = null;
	}

	if (isset($params['baseUri'])) {
		$ourBaseUri = $params['baseUri'];
	} else {
		$ourBaseUri = $configBaseUri;
	}

	if (isset($params['g2Uri'])) {
		$ourG2Uri = $params['g2Uri'];
	} else {
		$ourG2Uri = null;
	}

	if (isset($params['embedSessionString'])) {
		$ourEmbedSessionString = $params['embedSessionString'];
	} else {
		$ourEmbedSessionString = null;
	}

	$ret = $urlGenerator->init($ourBaseUri, $ourG2Uri, $ourEmbedSessionString);

	if ($ret) {
		return $ret;
	}

	$gallery->setUrlGenerator($urlGenerator);

	// Initialize our session
	if (!isset($params['noDatabase'])) {
		if (isset($params['gallerySessionId'])) {
			GalleryCoreApi::requireOnce('modules/core/classes/GallerySession.class');
			GalleryUtilities::putRequestVariable(SESSION_ID_PARAMETER, $params['gallerySessionId']);
		}

		$ret = $gallery->initSession();

		if ($ret) {
			return $ret;
		}
	} else {
		$gallery->initEmptySession();
	}

	if (version_compare(phpversion(), '5.0') < 0 && !isset($params['noDatabase'])) {
		/*
		 * The error handler relies on a database connection and a session
		 * So do not set it before they are initialized.
		 */
		set_error_handler('GalleryPhpErrorHandler');
		$GLOBALS['gallerySetErrorHandler'] = true;
	}

	// Initialize our translator
	$language = GalleryUtilities::getRequestVariables('language');

	if (isset($params['activeLanguage']) || !empty($language)) {
		GalleryCoreApi::requireOnce('modules/core/classes/GalleryTranslator.class');

		list($language) = GalleryTranslator::getMe()->getSupportedLanguageCode(
			empty($language) ? $params['activeLanguage'] : $language
		);

		$gallery->setActiveLanguageCode($language);
	}

	$ret = $gallery->initTranslator(isset($params['noDatabase']));

	if ($ret) {
		return $ret;
	}

	return null;
}

function GalleryInitSecondPass() {
	global $gallery;

	$session      =& $gallery->getSession();
	$urlGenerator =& $gallery->getUrlGenerator();

	$ret = $urlGenerator->initNavigation();

	if ($ret) {
		return $ret;
	}

	/*
	 * Set our active user.  Try getting our active user from registered authentication plugins.  If
	 * not, make us the anonymous user.  If we do not have a session, this will initiate one for us.
	 */
	list($ret, $pluginIds) = GalleryCoreApi::getAllFactoryImplementationIds('GalleryAuthPlugin');

	if ($ret) {
		return $ret;
	}

	foreach ($pluginIds as $pluginId => $className) {
		list($ret, $plugin) = GalleryCoreApi::newFactoryInstanceById('GalleryAuthPlugin', $pluginId);

		if ($ret) {
			return $ret;
		}

		list($ret, $user) = $plugin->getUser();

		if ($ret) {
			return $ret;
		}

		if (isset($user)) {
			break;
		}
	}

	if (!isset($user)) {
		// Missing user, be anonymous
		list($ret, $userId) = GalleryCoreApi::getAnonymousUserId();

		if ($ret) {
			return $ret;
		}

		list($ret, $user) = GalleryCoreApi::loadEntitiesById($userId, 'GalleryUser');

		if ($ret) {
			return $ret;
		}
	}

	$gallery->setActiveUser($user);

	return null;
}

/**
 * Interceptor for the standard PHP error handler.  We log any errors in the database, then
 * proceed with the regular error handler.
 * @see http://www.php.net/manual/en/function.set-error-handler.php
 *
 * @param int $errorNumber the code for the error
 * @param string $errorString an error message
 * @param string $file the file where the error occurred
 * @param int $line the line number of the error
 * @param mixed $context the complete context at the time of the error
 * @return boolean true if we should skip the regular PHP error handler
 */
function GalleryPhpErrorHandler($errorNumber, $errorString, $file, $line, $context) {
	if (error_reporting() == 0) {
		// The @ error suppression operator was used, so do not log this error.
		return false;
	}

	$errorType = array(
		E_ERROR           => 'Error',
		E_WARNING         => 'Warning',
		E_PARSE           => 'Parsing Error',
		E_NOTICE          => 'Notice',
		E_CORE_ERROR      => 'Core Error',
		E_CORE_WARNING    => 'Core Warning',
		E_COMPILE_ERROR   => 'Compile Error',
		E_COMPILE_WARNING => 'Compile Warning',
		E_USER_ERROR      => 'User Error',
		E_USER_WARNING    => 'User Warning',
		E_USER_NOTICE     => 'User Notice',
	// PHP5+: E_STRICT => 'Runtime Notice'
	// PHP5.2+: E_RECOVERABLE_ERROR => 'Catchable Fatal Error'
	);

	GalleryCoreApi::addEventLogEntry(
		'PHP Error',
		sprintf(
			'[%s] %s in file %s on line %d',
			$errorType[$errorNumber],
			$errorString,
			$file,
			$line
		),
		$errorString
	);

	// Fall back to the internal error handler
	return false;
}
