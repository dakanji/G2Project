<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */
/**
 * Perform all necessary initialization of the Gallery framework
 * @package Gallery
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 15698 $
 */

require_once(dirname(__FILE__) . '/modules/core/classes/GalleryUtilities.class');

function GalleryInitFirstPass($params=array()) {
    global $gallery;

    ini_set('magic_quotes_runtime', 0);
    ini_set('magic_quotes_sybase', 0);

    /* Specify that when an assertion fails, we terminate right away */
    assert_options(ASSERT_WARNING, 1);
    assert_options(ASSERT_BAIL, 1);

    /* Load all the core Gallery classes */
    $galleryBase = dirname(__FILE__) . '/';
    require_once($galleryBase . 'modules/core/classes/GalleryCoreApi.class');
    GalleryCoreApi::requireOnce('modules/core/classes/GalleryConstants.class');
    GalleryCoreApi::requireOnce('modules/core/classes/GalleryCapabilities.class');
    GalleryCoreApi::requireOnce('modules/core/classes/GalleryView.class');
    GalleryCoreApi::requireOnce('modules/core/classes/GalleryModule.class');

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

    /* Sanitize the data path */
    $dataBase = $gallery->getConfig('data.gallery.base');
    if ($dataBase{strlen($dataBase) - 1} != $slash) {
	$dataBase .= $slash;
	$gallery->setConfig('data.gallery.base', $dataBase);
    }

    /* Init for downloadable plugins */
    $gallery->setConfig('repository.url', 'http://gallery.menalto.com/repository/');
    $gallery->setConfig('repository.cache', $dataBase . 'cache' . $slash . 'repository' . $slash);

    /* Set our various data paths */
    $gallery->setConfig('data.gallery.cache', $dataBase . 'cache' . $slash);
    $gallery->setConfig('data.gallery.albums', $dataBase . 'albums' . $slash);
    $gallery->setConfig('data.gallery.locks', $dataBase . 'locks'. $slash);
    $gallery->setConfig('data.gallery.tmp', $dataBase . 'tmp' . $slash);
    $gallery->setConfig('data.smarty.base', $dataBase . 'smarty' . $slash);
    $gallery->setConfig('data.smarty.templates_c',
	$dataBase . 'smarty' . $slash . 'templates_c' . $slash);
    $gallery->setConfig('data.gallery.plugins', $galleryBase . 'plugins' . $slash);
    $gallery->setConfig('data.gallery.plugins_data', $dataBase . 'plugins_data' . $slash);

    /* Configure our url generator */
    if (!isset($params['noDatabase'])) {
	/*
	 * Swallow error to prevent GalleryFactoryHelper_loadRegistry cache from breaking upgrade to
	 * core 1.0.6
	 */
	list ($ret, $urlGenerator) = @GalleryCoreApi::newFactoryInstance('GalleryUrlGenerator');
	/* Swallow ERROR_STORAGE_FAILURE, or automatic upgrading fails */
	if ($ret && !($ret->getErrorCode() & ERROR_STORAGE_FAILURE)) {
	    return $ret;
	}
    }
    if (!isset($urlGenerator)) {
	GalleryCoreApi::requireOnce('modules/core/classes/GalleryUrlGenerator.class');
	$urlGenerator = new GalleryUrlGenerator();
    }
    /* Allow for overrides from GalleryEmbed ($param) or from config.php */
    $configBaseUri = @$gallery->getConfig('baseUri');
    $ret = $urlGenerator->init(
	isset($params['baseUri']) ? $params['baseUri']
				  : (!empty($configBaseUri) ? $configBaseUri : null),
	isset($params['g2Uri']) ? $params['g2Uri'] : null,
	isset($params['embedSessionString']) ? $params['embedSessionString'] : null);
    if ($ret) {
	return $ret;
    }
    $gallery->setUrlGenerator($urlGenerator);

    /* Initialize our session */
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

    /* Initialize our translator */
    $language = GalleryUtilities::getRequestVariables('language');
    if (isset($params['activeLanguage']) || !empty($language)) {
	GalleryCoreApi::requireOnce('modules/core/classes/GalleryTranslator.class');
	list ($language) = GalleryTranslator::getSupportedLanguageCode(
	    empty($language) ? $params['activeLanguage'] : $language);
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
    $session =& $gallery->getSession();
    $urlGenerator =& $gallery->getUrlGenerator();

    $ret = $urlGenerator->initNavigation();
    if ($ret) {
	return $ret;
    }

    /*
     * Set our active user.  Try getting our active user from registered authentication plugins.  If
     * not, make us the anonymous user.  If we don't have a session, this will initiate one for us.
     */
    list ($ret, $pluginIds) = GalleryCoreApi::getAllFactoryImplementationIds('GalleryAuthPlugin');
    if ($ret) {
	return $ret;
    }

    foreach ($pluginIds as $pluginId) {
	list ($ret, $plugin) =
	    GalleryCoreApi::newFactoryInstanceById('GalleryAuthPlugin', $pluginId);
	if ($ret) {
	    return $ret;
	}

	list ($ret, $user) = $plugin->getUser();
	if ($ret) {
	    return $ret;
	}

	if (isset($user)) {
	    break;
	}
    }

    if (!isset($user)) {
	/* Missing user, be anonymous */
	list ($ret, $userId) = GalleryCoreApi::getAnonymousUserId();
	if ($ret) {
	    return $ret;
	}

	list ($ret, $user) = GalleryCoreApi::loadEntitiesById($userId);
	if ($ret) {
	    return $ret;
	}
    }

    $gallery->setActiveUser($user);

    return null;
}
?>
