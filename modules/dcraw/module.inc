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
 * Dcraw Graphics Module
 *
 * This module provides the Dcraw graphics toolkit for Gallery
 *
 * @package Dcraw
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 16034 $
 */
class DcrawModule extends GalleryModule {

    function DcrawModule() {
	global $gallery;

	$this->setId('dcraw');
	$this->setName($gallery->i18n('Dcraw'));
	$this->setDescription(
	    $gallery->i18n('Graphics toolkit for processing images in raw format'));
	$this->setVersion('1.0.8');
	$this->setGroup('toolkits', $gallery->i18n('Graphics Toolkits'));
	$this->setCallbacks('getSiteAdminViews');
	$this->setRequiredCoreApi(array(7, 4));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::performFactoryRegistrations
     */
    function performFactoryRegistrations() {
	/* Register our graphics class with the factory */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'GalleryToolkit', 'DcrawToolkit', 'Dcraw',
	    'modules/dcraw/classes/DcrawToolkit.class', 'dcraw', null);
	if ($ret) {
	    return $ret;
	}

	return null;
    }

    /**
     * @see GalleryModule::activate
     */
    function activate($postActivationEvent=true) {
	global $gallery;

	list ($ret, $priority) = GalleryCoreApi::getToolkitPriorityById('Dcraw');
	if ($ret) {
	    return array($ret, null);
	}

	if (!$priority) {
	    list ($ret, $priority) = GalleryCoreApi::getMaximumManagedToolkitPriority();
	    if ($ret) {
		return array($ret, null);
	    }
	    $priority = min($priority + 1, 40);
	}

	$ret = GalleryCoreApi::registerToolkitOperation(
	    'Dcraw', array('image/x-dcraw'), 'convert-to-image/x-portable-pixmap',
	    array(), $gallery->i18n('Convert to PPM'), 'image/x-portable-pixmap', $priority);
	if ($ret) {
	    return array($ret, null);
	}

	$ret = GalleryCoreApi::registerToolkitProperty(
	    'Dcraw', array('image/x-dcraw'), 'dimensions', 'int,int',
	    $gallery->i18n('Get the width and height of the image'));
	if ($ret) {
	    return array($ret, null);
	}

	/* Assign all extensions we handle to our image mime type */
	foreach (array('bay', 'bmq', 'cr2', 'crw', 'cs1', 'dc2', 'dcr',
		       'fff', 'k25', 'kdc', 'mos', 'mrw', 'nef', 'orf',
		       'pef', 'raf', 'rdc', 'srf', 'x3f') as $extension) {
	    $ret = GalleryCoreApi::addMimeType($extension, 'image/x-dcraw', 0);
	    if ($ret) {
		if (!($ret->getErrorCode() & ERROR_COLLISION)) {
		    return array($ret, null);
		}
	    }
	}

	/* Detect the version of the binary (it may have changed since last activation) */
	GalleryCoreApi::requireOnce('modules/dcraw/classes/DcrawToolkitHelper.class');

	list ($ret, $path) = $this->getParameter('path');
	if ($ret) {
	    return array($ret, null);
	}

	list ($ret, $testResults, $version) = DcrawToolkitHelper::testBinary($path);
	if ($ret) {
	    return array($ret, null);
	}

	$ret = $this->setParameter('version', $version);
	if ($ret) {
	    return array($ret, null);
	}

	list ($ret, $redirect) = parent::activate($postActivationEvent);
	if ($ret) {
	    return array($ret, null);
	}

	return array(null, $redirect);
    }

    /**
     * @see GalleryModule::needsConfiguration
     */
    function needsConfiguration() {
	list ($ret, $path) = $this->getParameter('path');
	if ($ret) {
	    return array($ret, null);
	}

	list ($ret, $version) = $this->getParameter('version');
	if ($ret) {
	    return array($ret, null);
	}

	return array(null, empty($path) || empty($version));
    }

    /**
     * @see GalleryModule::autoConfigure
     */
    function autoConfigure() {
	global $gallery;

	list ($ret, $needsConfiguration) = $this->needsConfiguration();
	if ($ret) {
	    return array($ret, false);
	}

	if (!$needsConfiguration) {
	    return array(null, true);
	}

	/* Try a bunch of likely seeming paths to see if any of them work. */
	$platform =& $gallery->getPlatform();
	$slash = $platform->getDirectorySeparator();

	/*
	 * Start with system paths.  Tack on a trailing slash if necessary,
	 * then tack on other likely paths, based on our OS
	 */
	$paths = array();
	if (GalleryUtilities::isA($platform, 'WinNtPlatform')) {
	    foreach (explode(';', getenv('PATH')) as $path) {
		$path = trim($path);
		if (empty($path)) {
		    continue;
		}
		if ($path{strlen($path)-1} != $slash) {
		    $path .= $slash;
		}
		$paths[] = $path;
	    }

	    /*
	     * Double-quoting the paths removes any ambiguity about the
	     * double-slashes being escaped or not
	     */
	    $paths[] = "C:\\Program Files\\Dcraw\\dcraw.exe";
	    $paths[] = "C:\\apps\Dcraw\\dcraw.exe";
	    $paths[] = "C:\\Dcraw\\dcraw.exe";
	} else if (GalleryUtilities::isA($platform, 'UnixPlatform')) {
	    foreach (explode(':', getenv('PATH')) as $path) {
		$path = trim($path);
		if (empty($path)) {
		    continue;
		}
		if ($path{strlen($path)-1} != $slash) {
		    $path .= $slash;
		}
		$paths[] = $path;
	    }

	    $paths[] = '/usr/bin/dcraw';
	    $paths[] = '/usr/local/bin/dcraw';
	    $paths[] = '/bin/dcraw';
	    $paths[] = '/sw/bin/dcraw';
	} else {
	    return array(null, false);
	}

	/* Load any classes we require */
	GalleryCoreApi::requireOnce('modules/dcraw/classes/DcrawToolkitHelper.class');

	/* Now try each path in turn to see which ones work */
	foreach ($paths as $path) {
	    list ($ret, $testResults, $version) = DcrawToolkitHelper::testBinary($path);
	    if ($ret) {
		/* This path failed.  Continue with the next one */
		continue;
	    }

	    $failCount = 0;
	    foreach ($testResults as $testResult) {
		/* All tests should work, else this path is not a valid one */
		if (!$testResult['success']) {
		    $failCount++;
		}
	    }

	    if ($failCount == 0) {
		/* We have a winner */
		$ret = GalleryCoreApi::setPluginParameter('module', 'dcraw', 'path', $path);
		if ($ret) {
		    return array($ret, false);
		}

		return array(null, true);
	    }
	}

	return array(null, false);
    }

    /**
     * @see GalleryModule::getSiteAdminViews
     */
    function getSiteAdminViews() {
	return array(null,
		     array(array('name' => $this->translate('Dcraw'),
				 'view' => 'dcraw.AdminDcraw')));
    }

    /**
     * @see GalleryModule::getConfigurationView
     */
    function getConfigurationView() {
	return 'dcraw.AdminDcraw';
    }
}
?>
