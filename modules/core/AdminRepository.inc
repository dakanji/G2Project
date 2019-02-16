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

GalleryCoreApi::requireOnce('modules/core/classes/GalleryRepository.class');

/**
 * This controller will handle an administration request for a module
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Jozef Selesi <selesi at gmail dot com>
 * @version $Revision: 16471 $
 */
class AdminRepositoryController extends GalleryController {

    /**
     * Get the repositories, but allow tests to inject their own version.
     * @see GalleryRepository::getRepositories
     * @access private
     */
    function _getRepositories() {
	if (isset($this->_repositories)) {
	    return array(null, $this->_repositories);
	}
	return GalleryRepository::getRepositories();
    }

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	global $gallery;

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret, null);
	}

	$status = $error = array();
	if (isset($form['action']['update'])) {
	    foreach (array('module', 'theme') as $pluginType) {
		list ($ret, $pluginStatus[$pluginType]) =
		    GalleryCoreApi::fetchPluginStatus($pluginType, true);
		if ($ret) {
		    return array($ret, null);
		}
	    }

	    if (empty($form['repositories'])) {
		$form['repositories'] = array('released' => 1);
	    }

	    /* filter the form values through our list of valid keys to avoid exploits */
	    $data = array();
	    foreach (array('released', 'experimental', 'community') as $key) {
		if (isset($form['repositories'][$key])) {
		    $data[$key] = 1;
		}
	    }
	    $ret = GalleryCoreApi::setPluginParameter(
		'module', 'core', 'core.repositories', serialize($data));
	    if ($ret) {
		return array($ret, null);
	    }

	    list ($ret, $repositories) = $this->_getRepositories();
	    if ($ret) {
		return array($ret, null);
	    }

	    $templateAdapter =& $gallery->getTemplateAdapter();
	    $templateAdapter->registerTrailerCallback(
		array($this, 'updatePluginStatus'),
		array($pluginStatus, $repositories));
	    $delegate['view'] = 'core.ProgressBar';
	} else if (isset($form['action']['upgradeAll'])) {
	    list ($ret, $repositories) = $this->_getRepositories();
	    if ($ret) {
		return array($ret, null);
	    }

	    /* Get list of upgradeable packages in repository */
	    foreach ($repositories as $source => $repository) {
		list ($ret, $tmp) = $repository->getAllUpgradeablePackages();
		if ($ret) {
		    return array($ret, null);
		}

		if (!empty($tmp)) {
		    $packages[$source] = $tmp;
		}
	    }

	    if (empty($packages)) {
		$status['noUpgradeAvailable'] = 1;
	    } else {
		/*
		 * Start upgrade process and show progress bar.
		 *
		 * performDownloadAndInstallation used to live in this controller, so we'd just
		 * call it directly.  But it got refactored into AdminRepositoryDownloadController
		 * so for now, just call it there.
		 *
		 * @todo: refactor it into a common base class
		 */

		GalleryCoreApi::requireOnce('modules/core/AdminRepositoryDownload.inc');
		$controller = new AdminRepositoryDownloadController();
		$templateAdapter =& $gallery->getTemplateAdapter();
		$templateAdapter->registerTrailerCallback(
		    array($controller, 'performDownloadAndInstallation'),
		    array($packages, array(), $repositories));
		$delegate['view'] = 'core.ProgressBar';
	    }
	}

	if (!empty($redirect)) {
	    $results['redirect'] = $redirect;
	} else {
	    if (empty($delegate)) {
		$results['delegate']['view'] = 'core.SiteAdmin';
		$results['delegate']['subView'] = 'core.AdminRepository';
	    } else {
		$results['delegate'] = $delegate;
	    }
	}
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }

    /**
     * Update the repository index, and scan all installed plugins and update
     * GalleryPluginPackageMap with a complete listing of everything that's installed.
     *
     * @todo Show a summary page (or at least a link to it) which contains details about
     * the exact tasks that were performed and any errors that were encountered.
     *
     * @param $pluginStatus status of all available installed plugins
     * @param $repositories the repositories available for new plugins
     * @return object GalleryStatus a status code
     */
    function updatePluginStatus($pluginStatus, $repositories) {
	global $gallery;
	$templateAdapter =& $gallery->getTemplateAdapter();
	$platform =& $gallery->getPlatform();

	$status = array();
	list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'core');
	if ($ret) {
	    return $ret;
	}
	$heading = $module->translate('Updating Repository Index');
	$templateAdapter->updateProgressBar($heading, '', 0);

	/* Erase all plugin data, so that a full scan drops old entries */
	$ret = GalleryCoreApi::removeAllMapEntries('GalleryPluginPackageMap');
	if ($ret) {
	    return $ret;
	}

	/* Update the index */
	$currentProgress = 0;
	$indexWeight = 5;
	$increment = 1.0 / ($indexWeight * count($repositories) + count($pluginStatus['module']) +
			    count($pluginStatus['theme']));

	foreach ($repositories as $source => $repository) {
	    list ($ret, $message) = GalleryRepository::translateRepositoryName($source);
	    if ($ret) {
		return $ret;
	    }

	    $templateAdapter->updateProgressBar($heading, $message, $currentProgress);
	    $ret = $repository->downloadIndex();
	    if ($ret) {
		/* TODO: Internationalize this error message */
		$status['error']['download'][] = $ret->getErrorMessage();
	    }
	    $currentProgress += $indexWeight * $increment;
	}

	$heading = $module->translate('Scanning plugins');
	$sSlash = 's' . $platform->getDirectorySeparator();

	if (!isset($repository)) {
	    /*
	     * The loop below needs a working repository, but we don't have one because all of our
	     * existing indexes are missing or corrupted.  So create a new bogus one expressly for
	     * the purpose of calling GalleryRepository::scanPlugin().
	     */
	    $repository = new GalleryRepository();
	    $repository->init('bogus');
	}

	foreach (array('module', 'theme') as $pluginType) {
	    foreach (array_keys($pluginStatus[$pluginType]) as $pluginId) {
		$gallery->guaranteeTimeLimit(30);

		$templateAdapter->updateProgressBar(
			$heading, $pluginType . $sSlash . $pluginId, $currentProgress);
		$ret = $repository->scanPlugin($pluginType, $pluginId);
		if ($ret && !($ret->getErrorCode() & ERROR_STORAGE_FAILURE)) {
		    /*
		     * Something is wrong with this plugin. Maybe it's a 3rd party plugin w/o
		     * MANIFEST file, maybe it has no revisions in the po files, maybe the
		     * module.inc is foobar. Just log and ignore it.
		     */
		    if ($gallery->getDebug()) {
			$gallery->debug_r($ret);
		    }
		    $status['error']['scanPlugin'][] = $pluginId;
		} else if ($ret) {
		    return $ret;
		}
		$currentProgress += $increment;
	    }
	}

	$templateAdapter->updateProgressBar(
	    $module->translate('Update Plugin List'), $module->translate('Done.'), 1);

	$redirect['view'] = 'core.SiteAdmin';
	$redirect['subView'] = 'core.AdminRepository';

	$session =& $gallery->getSession();
	$redirect['statusId'] = $session->putStatus($status);

	$urlGenerator =& $gallery->getUrlGenerator();
	$templateAdapter->completeProgressBar($urlGenerator->generateUrl($redirect));

	return null;
    }
}

/**
 * This view will show all repository-related features.
 */
class AdminRepositoryView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	global $gallery;
	$platform =& $gallery->getPlatform();
	$session =& $gallery->getSession();

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret, null);
	}

	$AdminRepository = array();

	/* Init repository. */
	list ($ret, $repositories, $repositoryInitErrorCount) =
	    GalleryRepository::getRepositories();
	if ($ret) {
	    return array($ret, null);
	}

	if ($form['formName'] != 'AdminRepository') {
	    /* Set some defaults */
	    $form['formName'] = 'AdminRepository';
	    list ($ret, $sources) =
		GalleryCoreApi::getPluginParameter('module', 'core', 'core.repositories');
	    if ($ret) {
		return array($ret, null);
	    }
	    @$form['repositories'] = unserialize($sources);
	}
	if (empty($form['repositories'])) {
	    $form['repositories'] = array('released' => 1);
	}

	/* Check if incompatible plugins should be shown. */
	$coreApis = array();
	$showIncompatible = GalleryUtilities::getRequestVariables('showIncompatible');
	if ($showIncompatible == 'true') {
	    list ($coreApiVersion, $themeApiVersion, $moduleApiVersion) =
		GalleryUtilities::getRequestVariables('coreApi', 'themeApi', 'moduleApi');
	    if (empty($coreApiVersion) || empty($themeApiVersion) || empty($moduleApiVersion)) {
		return array(GalleryCoreApi::error(
				 ERROR_BAD_PARAMETER, __FILE__, __LINE__,
				 "[$coreApiVersion] [$themeApiVersion] [$moduleApiVersion]"),
			     null);
	    }
	    $coreApis['core'] = explode('.', $coreApiVersion);
	    $coreApis['module'] = explode('.', $moduleApiVersion);
	    $coreApis['theme'] = explode('.', $themeApiVersion);
	    $AdminRepository['showIncompatible'] = 1;
	}

	$AdminRepository['isCoreUpgradeAvailable'] = false;
	$AdminRepository['upgradesAvailable'] = false;
	foreach ($repositories as $source => $repository) {
	    if (!$repository->localIndexExists()) {
		break;
	    }

	    /*
	     * Note: this merges modules and themes instead of treating themes separately.
	     * TODO: separate themes into their own group, like we do elsewhere
	     */
	    foreach (array('module', 'theme') as $pluginType) {
		/* Get list of plugins to show in the repository browser. */
		list ($ret, $browseData[$pluginType]) = $repository->getRepositoryPluginList(
		    $pluginType, $showIncompatible, $coreApis);
		if ($ret) {
		    return array($ret, null);
		}

		foreach ($browseData[$pluginType] as $pluginId => $browseInfo) {
		    if (empty($AdminRepository['browseData'][$pluginId]) ||
			(empty($AdminRepository['browseData'][$pluginId]['action']) &&
			 !empty($browseInfo['action'])) ||
			version_compare(
			    $browseInfo['repositoryVersion'],
			    $AdminRepository['browseData'][$pluginId]['repositoryVersion'],
			    '>')) {
			$AdminRepository['browseData'][$pluginId] = $browseInfo;
		    }
		}
	    }

	    /* Get local index meta data. */
	    list ($ret, $indexMetaData) = $repository->getIndexMetaData();
	    if ($ret) {
		return array($ret, null);
	    }

	    list ($ret, $message) = GalleryRepository::translateRepositoryName($source);
	    if ($ret) {
		return array($ret, null);
	    }

	    $AdminRepository['indexMetaData'][$source] =
		array_merge(array('name' => $message), $indexMetaData);

	    list ($ret, $tmp) = $repository->getAllUpgradeablePackages();
	    if ($ret) {
		return array($ret, null);
	    }

	    if (!empty($tmp)) {
		$AdminRepository['upgradesAvailable'] = true;
	    }

	    /* Get core upgrade info. */
	    if ($source == 'released') {
		list ($ret, $AdminRepository['isCoreUpgradeAvailable'], $apiVersions) =
		    $repository->isCoreUpgradeAvailable();
		if ($ret) {
		    return array($ret, null);
		}
		if ($AdminRepository['isCoreUpgradeAvailable']) {
		    $AdminRepository['latestCoreApiVersion'] = $apiVersions['core'];
		    $AdminRepository['latestThemeApiVersion'] = $apiVersions['theme'];
		    $AdminRepository['latestModuleApiVersion'] = $apiVersions['module'];
		}
	    }
	}

	if (!empty($AdminRepository['browseData'])) {
	    GalleryCoreApi::requireOnce('modules/core/AdminPlugins.inc');
	    uasort($AdminRepository['browseData'], array($this, 'pluginSort'));
	}

	$baseName = dirname(dirname(dirname(__FILE__)));
	$AdminRepository['writeable'] = array(
	    'modules' => $platform->is_writeable("$baseName/modules"),
	    'themes' => $platform->is_writeable("$baseName/themes"));
	$AdminRepository['authToken'] = $session->getAuthToken();

	/* Render the HTML body */
	$template->setVariable('AdminRepository', $AdminRepository);
	$template->setVariable('controller', 'core.AdminRepository');

	if ($repositoryInitErrorCount) {
	    $status =& $template->getVariableByReference('status');
	    $status['error']['repositoryInitErrorCount'] = $repositoryInitErrorCount;
	}

	$template->javascript('lib/yui/yahoo-min.js');
	$template->javascript('lib/yui/dom-min.js');
	$template->javascript('lib/yui/event-min.js');
	$template->javascript('lib/yui/connection-min.js');
	$template->javascript('lib/yui/animation-min.js');
	$template->javascript('lib/yui/container-min.js');
	$template->javascript('modules/core/templates/AdminRepository.js');
	return array(null, array('body' => 'modules/core/templates/AdminRepository.tpl'));
    }

    /**
     * TODO: combine this with AdminPluginsView::pluginSort
     */
    function pluginSort($a, $b) {
	if (($cmp = strcmp($a['groupLabel'], $b['groupLabel'])) == 0) {
	    $cmp = strcmp($a['name'], $b['name']);
	}
	return $cmp;
    }
}
?>
