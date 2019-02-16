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
 * This view prepares all the information the theme requires to render an error page.
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 15706 $
 */
class ErrorPageView extends GalleryView {

    function setError($error) {
	$this->_error = $error;
    }

    /**
     * @see GalleryView::getViewType
     */
    function getViewType() {
	return VIEW_TYPE_ERROR;
    }

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	global $gallery;

	list ($ret, $isAdmin) = GalleryCoreApi::isUserInSiteAdminGroup();
	$ErrorPage = array('isAdmin' => !$ret && $isAdmin);

	$showDetails = $ErrorPage['isAdmin'] || $gallery->getDebug();
	if (empty($this->_error)) {
	    $session =& $gallery->getSession();
	    $errorCode = $session->get('core.error.code');
	    $errorTrace = $session->get('core.error.trace');
	} else {
	    $errorCode = $this->_error->getErrorCode();
	    $errorTrace = $this->_error->getAsHtml($showDetails);
	}

	if ($showDetails) {
	    $ErrorPage['stackTrace'] = $errorTrace;
	}

	/* Landing page for errors */
	if ($errorCode & ERROR_OBSOLETE_DATA) {
	    $ErrorPage['code']['obsoleteData'] = true;
	}
	if ($errorCode & (ERROR_PERMISSION_DENIED | ERROR_BAD_PARAMETER)) {
	    $ErrorPage['code']['securityViolation'] = true;
	}
	if ($errorCode & ERROR_STORAGE_FAILURE) {
	    $ErrorPage['code']['storageFailure'] = true;
	}
	if ($errorCode & ERROR_PLATFORM_FAILURE) {
	    $ErrorPage['code']['platformFailure'] = true;
	}
	if ($errorCode & ERROR_MISSING_OBJECT) {
	    $ErrorPage['code']['missingObject'] = true;
	}
	if ($errorCode & ERROR_REQUEST_FORGED) {
	    $ErrorPage['code']['requestAuthenticationFailure'] = true;
	}

	if ($ErrorPage['isAdmin']) {
	    $ErrorPage['phpversion'] = phpversion();
	    $ErrorPage['php_uname'] = php_uname();
	    $ErrorPage['php_sapi_name'] = php_sapi_name();
	    $ErrorPage['webserver'] = GalleryUtilities::getServerVar('SERVER_SOFTWARE');
	    $ErrorPage['browser'] = GalleryUtilities::getServerVar('HTTP_USER_AGENT');
	    if ($gallery->isStorageInitialized()) {
		$storage =& $gallery->getStorage();
		$ErrorPage['dbType'] = $storage->getAdoDbType();
		$ErrorPage['dbVersion'] = @$storage->getVersion();

		list ($ret, $list) = GalleryCoreApi::getToolkitOperationMimeTypes('thumbnail');
		if (!$ret) {
		    $toolkitList = array();
		    foreach ($list as $tmp) {
			$toolkitList = array_merge($toolkitList, $tmp);
		    }
		    $ErrorPage['toolkits'] = implode(', ', array_unique($toolkitList));
		}
	    }

	    GalleryCoreApi::requireOnce('modules/core/module.inc');
	    $installedVersions = CoreModule::getInstalledVersions();
	    $ErrorPage['version'] = $installedVersions['gallery'];
	}

	$template->javascript('lib/javascript/BlockToggle.js');
	$template->setVariable('ErrorPage', $ErrorPage);
	return array(null, array('body' => 'modules/core/templates/ErrorPage.tpl'));
    }

    /**
     * Entry point from main.php
     *
     * @param object GalleryStatus $error
     * @param mixed $g2Data GalleryMain result, if available
     * @param boolean $initOk false if Gallery has not initialized (can't use theme system)
     * @static
     */
    function errorHandler($error, $g2Data=null, $initOk=true) {
	global $gallery;
	$failsafe = false;
	if (!$initOk) {
	    $failsafe = true;
	}

	/* Post Gallery::Error event */
	$event = GalleryCoreApi::newEvent('Gallery::Error');
	$event->setData(array('error' => $error));
	list ($ret, $eventResults) = GalleryCoreApi::postEvent($event);
	if ($ret) {
	    $failsafe = true;
	}

	/* Return HTTP 404 status for ERROR_MISSING_OBJECT */
	if ($error->getErrorCode() & ERROR_MISSING_OBJECT) {
	    GalleryUtilities::setResponseHeader('HTTP/1.0 404 Not Found', false);
	} else {
	    GalleryUtilities::setResponseHeader('HTTP/1.0 500 Internal Server Error', false);
	}

	foreach ($eventResults as $eventResult) {
	    if (!empty($eventResult['suppressBody'])) {
		/* One of our error handlers has dealt with the output */
		return;
	    }
	}

	if (!$failsafe) {
	    list ($ret, $themeId) =
		GalleryCoreApi::getPluginParameter('module', 'core', 'default.theme');
	    if ($ret) {
		$failsafe = true;
	    }
	}

	if (!$failsafe) {
	    list ($ret, $theme) = GalleryCoreApi::loadPlugin('theme', $themeId);
	    if ($ret) {
		$failsafe = true;
	    }
	    $templateAdapter =& $gallery->getTemplateAdapter();
	    $templateAdapter->setTheme($theme);
	}

	if (!$failsafe) {
	    list ($ret, $view) = GalleryView::loadView('core.ErrorPage');
	    if ($ret) {
		$failsafe = true;
	    }
	}

	if (!$failsafe) {
	    $dummyForm = array();
	    GalleryCoreApi::requireOnce('modules/core/classes/GalleryTemplate.class');
	    $template = new GalleryTemplate(dirname(dirname(dirname(__FILE__))));
	    $view->setError($error);
	    list ($ret, $results) = $view->loadTemplate($template, $dummyForm);
	    if ($ret) {
		$failsafe = true;
	    }

	    $t =& $template->getVariableByReference('theme');
	    $t['errorTemplate'] = $results['body'];
	}

	if (!$failsafe) {
	    $template->setVariable('l10Domain', 'modules_core');
	    list ($ret, $templatePath) = $theme->showErrorPage($template);
	    if ($ret) {
		$failsafe = true;
	    }
	}

	if (!$failsafe) {
	    $template->setVariable('l10Domain', 'themes_' . $themeId);
	    $ret = $template->display("themes/$themeId/templates/$templatePath");
	    if ($ret) {
		$failsafe = true;
	    }
	}

	if ($failsafe) {
	    /* A catastrophic failure has occurred so just dump the error out to the browser */
	    print '<h2>Error</h2>' . $error->getAsHtml($gallery->getDebug());
	    if ($gallery->getDebug() == 'buffered') {
		print '<h3>Debug Output</h3><pre>' . $gallery->getDebugBuffer() . '</pre>';
	    }
	}
    }
}
?>
