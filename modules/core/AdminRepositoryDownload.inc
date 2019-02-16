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
GalleryCoreApi::requireOnce('modules/core/classes/GalleryRepositoryUtilities.class');

/**
 * This controller will handle an administration request for a module
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Jozef Selesi <selesi at gmail dot com>
 * @version $Revision: 16127 $
 */
class AdminRepositoryDownloadController extends GalleryController {

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

	if (!isset($form['pluginType']) || !isset($form['pluginId'])) {
	    return array(GalleryCoreApi::error(
			     ERROR_BAD_PARAMETER, __FILE__, __LINE__,
			     "Plugin type or ID not set [$pluginType] [$pluginId]"),
			 null);
	}
	$pluginType = $form['pluginType'];
	$pluginId = $form['pluginId'];

	if (!preg_match('/theme|module/', $pluginType)) {
	    return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__,
					       "Invalid plugin type [$pluginType]"),
			 null);
	}

	/* Handle cancel action. */
	if (isset($form['action']['cancel'])) {
	    $redirect['view'] = 'core.SiteAdmin';
	    $redirect['subView'] = 'core.AdminRepository';
	} else if (isset($form['action']['download'])) {
	    /* Create package list. */
	    $installPackages = $deleteLanguages = array();

	    $utils = new GalleryRepositoryUtilities();
	    list ($ret, $pluginPackages) = $utils->getPluginPackages($pluginType, $pluginId);
	    if ($ret) {
		return array($ret, null);
	    }

	    $baseSource = '';
	    if (!empty($form['base'])) {
		list($baseSource, $baseNewBuild) = explode(':', $form['base']);
		if (empty($pluginPackages['base']['build']) ||
		    $pluginPackages['base']['build'] != $baseNewBuild) {
		    $installPackages[$baseSource][$pluginType][$pluginId]['base'] = 1;
		}
	    }

	    $selected = array();
	    if (isset($form['languages'])) {
		foreach ($form['languages'] as $language) {
		    list ($langSource, $langCode, $langNewVersion) = explode(':', $language);
		    $langCode = 'lang-' . $langCode;
		    if ($langSource == $baseSource &&
			(empty($pluginPackages[$langCode]['build']) ||
			 $pluginPackages[$langCode]['build'] != $langNewVersion)) {
			$installPackages[$langSource][$pluginType][$pluginId][$langCode] = 1;
		    }
		    $selected[$pluginType][$pluginId][$langCode] = 1;
		}
	    }

	    if (isset($form['languagesAvailable'])) {
		foreach ($form['languagesAvailable'] as $language) {
		    list ($langSource, $langCode) = explode(':', $language);
		    if ($langSource == $baseSource &&
		        empty($selected[$pluginType][$pluginId]['lang-' . $langCode])) {
			$deleteLanguages[$pluginType][$pluginId][] = $langCode;
		    }
		}
	    }

	    /* Show error message if no packages have been selected for download. */
	    if (empty($installPackages) && empty($deleteLanguages)) {
		$delegate['view'] = 'core.SiteAdmin';
		$delegate['subView'] = 'core.AdminRepositoryDownload';
		$error[] = 'form[error][nothingSelected]';

		/* TODO: Do we need to put these vars back into the request? */
		GalleryUtilities::putRequestVariable('pluginId', $pluginId);
		GalleryUtilities::putRequestVariable('pluginType', $pluginType);
	    } else {
		list ($ret, $repositories) = GalleryRepository::getRepositories();
		if ($ret) {
		    return array(null, $ret);
		}

		$templateAdapter =& $gallery->getTemplateAdapter();
		$templateAdapter->registerTrailerCallback(
		    array($this, 'performDownloadAndInstallation'),
		    array($installPackages, $deleteLanguages, $repositories));
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
     * Download specified packages to the local repository cache and perform installation.
     *
     * TODO: Show a summary page (or at least a link to it) which contains details about
     * the exact tasks that were performed and any errors that were encountered.
     *
     * @param array $installPackages packages to install
     * @param array $deleteLanguages language packages to delete
     * @param array an array of object GalleryRepository
     * @return object GalleryStatus a status code
     */
    function performDownloadAndInstallation($installPackages, $deleteLanguages, $repositories) {
	global $gallery;
	$session =& $gallery->getSession();
	$platform =& $gallery->getPlatform();
	$phpVm = $gallery->getPhpVm();
	$templateAdapter =& $gallery->getTemplateAdapter();

	/*
	 * Get the original plugin status from before we start making changes so that we can
	 * try to counteract any activate/deactivate ripple effects.
	 */
	foreach (array('module', 'theme') as $pluginType) {
	    list ($ret, $pluginStatus[$pluginType]) = $this->_fetchPluginStatus($pluginType, false);
	    if ($ret) {
		return $ret;
	    }
	}

	list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'core');
	if ($ret) {
	    return $ret;
	}
	$titleText = $module->translate('Updating Packages');
	$templateAdapter->updateProgressBar($titleText, '', 0);

	/* Create download file list. */
	$sourcedFiles = array();
	foreach ($installPackages as $source => $packages) {
	    if (!isset($repositories[$source])) {
		continue;
	    }

	    list ($ret, $tmp) = $repositories[$source]->getDownloadFileList($packages);
	    if ($ret) {
		return $ret;
	    }
	    $sourcedFiles[$source] = empty($sourcedFiles[$source])
		? $tmp
		: array_merge($sourcedFiles[$source], $tmp);
	}

	$totalActions = 0;
	foreach ($sourcedFiles as $source => $files) {
	    foreach ($files as $pluginType => $plugins) {
		foreach ($plugins as $pluginId => $pluginDownloadData) {
		    /*
		     * 2 actions (preverify, then download) for all files except the descriptor
		     * which we just download.
		     */
		    $totalActions += 2 * count($pluginDownloadData['files']) - 1;
		}
	    }
	}
	foreach ($deleteLanguages as $pluginType => $plugins) {
	    foreach ($plugins as $pluginId => $languages) {
		$totalActions += count($languages);
	    }
	}

	$status = array();
	$status['error'] = array();

	/* Download files. */
	$currentAction = 0;

	foreach ($sourcedFiles as $source => $files) {
	    $repository = $repositories[$source];

	    foreach ($files as $pluginType => $plugins) {
		foreach ($plugins as $pluginId => $pluginDownloadData) {
		    $packageUrls = $pluginDownloadData['files'];
		    $pluginName = $pluginDownloadData['name'];

		    $templateAdapter->updateProgressBar(
			$titleText,
			sprintf($module->translate('Preparing %s'), $pluginName),
			++$currentAction / $totalActions);

		    /*
		     * Extract the descriptor and verify that all of our packages will unpack
		     * safely before starting.
		     */
		    $relativeDescriptorUrl = $packageUrls['descriptor'];
		    unset($packageUrls['descriptor']);

		    list ($ret, $descriptor) = $repository->downloadAndUnpack(
			$pluginType, $pluginId, 'descriptor', $relativeDescriptorUrl);
		    if ($ret) {
			if ($ret->getErrorCode() & ERROR_STORAGE_FAILURE) {
			    /* XXX: storage failure means we failed to download the file properly */
			    $status['error']['failedToDownload'][$pluginName] =
				$relativeDescriptorUrl;
			    continue;
			}
			return $ret;
		    }

		    $errors = array();
		    foreach (array_keys($packageUrls) as $packageName) {
			$gallery->guaranteeTimeLimit(30);

			if (++$currentAction % 5 == 0) {
			    $templateAdapter->updateProgressBar(
				$titleText,
				sprintf($module->translate('Preparing %s'), $pluginName),
				$currentAction / $totalActions);
			}

			/* pre-verify here */
			$errors = array_merge(
			    $errors, $repository->preVerifyPackage($packageName, $descriptor));
		    }
		    $errors = array_unique($errors);
		    if ($errors) {
			$status['error']['failedToInstall'][$pluginName] = $errors;
			continue;
		    }

		    $templateAdapter->updateProgressBar(
			$titleText,
			sprintf($module->translate('Downloading %s'), $pluginName),
			$currentAction / $totalActions);


		    $dontReactivate = array();
		    foreach ($packageUrls as $packageName => $relativePackageUrl) {
			$gallery->guaranteeTimeLimit(30);

			if (++$currentAction % 5 == 0) {
			    $templateAdapter->updateProgressBar(
				$titleText,
				sprintf($module->translate('Downloading %s'), $pluginName),
				$currentAction / $totalActions);
			}

			/* Download and unpack package. */
			list ($ret, $ignored) = $repository->downloadAndUnpack(
			    $pluginType, $pluginId, $packageName, $relativePackageUrl);
			if ($ret) {
			    if ($ret->getErrorCode() & ERROR_STORAGE_FAILURE) {
				/* XXX: storage failure means we failed to download the file */
				$status['error']['failedToDownload'][$pluginName][] =
				    $relativePackageUrl;
				if ($packageName == 'base') {
				    $dontReactivate[$pluginType][$pluginId] = 1;
				    break;
				} else {
				    continue;
				}
			    }
			    return $ret;
			}

			/* Check the unpacked files' integrity. */
			$ret = $repository->verifyPackageIntegrity($packageName, $descriptor);
			if ($ret) {
			    return $ret;
			}

			/* Update plugin package map. */
			list ($ret, $version, $build) = $repository->getPackageVersionAndBuild(
			    $pluginType, $pluginId, $packageName);
			if ($ret) {
			    return $ret;
			}
			$ret = $repository->updatePackageMetaData(
			    $pluginType, $pluginId, $packageName, $version, $build, 0);
			if ($ret) {
			    return $ret;
			}
		    }

		    /*
		     * Our plugin status caches are no longer valid because we've just installed
		     * some new code.  Load the status and ignore the cache which forces a cache
		     * update.  @todo: add a real cache flushing method in the api instead
		     */
		    list ($ret, $ignored) = GalleryCoreApi::fetchPluginStatus($pluginType, true);
		    if ($ret) {
			return $ret;
		    }

		    /*********************************************************
		     * This next block is duplicated in PluginCallback.
		     * @todo: Refactor this code duplication away
		     */

		    $templateAdapter->updateProgressBar(
			$titleText, sprintf($module->translate('Activating %s'), $pluginName),
			$currentAction / $totalActions);

		    if (empty($dontReactivate[$pluginType][$pluginId])) {
			/**
			 * Some plugins may already be loaded (e.g. the default theme, or
			 * authentication modules) so PHP will not let us evaluate the newly
			 * downloaded code.  This means that we can't upgrade those modules
			 * via DP; the site admin must do it by hand on the AdminPlugins page.
			 * For now, just notify the user.
			 *
			 * @todo convert this to a two-phase approach so that we can reactivate
			 *       plugins that are already loaded at this point.
			 */
			if ($phpVm->class_exists($pluginId . $pluginType)) {
			    $status['error']['cantUpgradeInUse'][] = $pluginName;
			} else {
			    list ($ret, $plugin) =
				GalleryCoreApi::loadPlugin($pluginType, $pluginId, true);
			    if ($ret) {
				return $ret;
			    }

			    $ret = $plugin->installOrUpgrade();
			    if ($ret) {
				return $ret;
			    }

			    if ($pluginType == 'module') {
				list ($ret, $autoConfigured) = $plugin->autoConfigure();
				if ($ret) {
				    return $ret;
				}
			    } else {
				/* Themes don't need this step */
				$autoConfigured = true;
			    }

			    $isActive = !empty($pluginStatus[$pluginType][$pluginId]['active']);
			    $notInstalled = empty($pluginStatus[$pluginType][$pluginId]['version']);

			    if ($autoConfigured && ($isActive || $notInstalled)) {
				list ($ret, $redirect) = $plugin->activate();
				if ($ret) {
				    return $ret;
				}
				/* Ignore the redirect */
			    }

			    $status['updated'][] = $pluginName;
			}
		    }

		    /*********************************************************/
		}
	    }
	}

	/* Delete old language packs */
	$deleteText = $module->translate('Deleting Language Packs');
	$g2base = dirname(dirname(dirname(__FILE__)));
	$status['languagePacksDeleted'] = 0;
	foreach ($deleteLanguages as $pluginType => $plugins) {
	    foreach ($plugins as $pluginId => $languages) {
		foreach ($languages as $language) {
		    $currentAction++;
		    $templateAdapter->updateProgressBar(
			$titleText, $deleteText, $currentAction / $totalActions);

		    $actual = 0;
		    $dir = "$g2base/${pluginType}s/$pluginId/locale/$language";
		    if ($platform->is_dir($dir) && $platform->is_writeable($dir)) {
			$platform->recursiveRmdir($dir);
			$actual++;
		    }
		    $file = "$g2base/${pluginType}s/$pluginId/po/$language.po";
		    if ($platform->is_file($file) && $platform->is_writeable($file)) {
			$platform->unlink($file);
			$actual++;
		    }

		    $ret = GalleryCoreApi::removeMapEntry(
			'GalleryPluginPackageMap',
			array('pluginType' => $pluginType,
			      'pluginId' => $pluginId,
			      'packageName' => 'lang-' . $language));
		    if ($ret) {
			return $ret;
		    }

		    if ($actual) {
			$status['languagePacksDeleted']++;
		    }
		}
	    }
	}

	/* Update progress bar. */
	if (!empty($status['error'])) {
	    $message = $module->translate('Update completed with errors.');
	} else {
	    $message = $module->translate('Update complete.');
	}
	$templateAdapter->updateProgressBar($titleText, $message, 1);

	/* Show link to return to the previously selected tab. */
	$redirect['view'] = 'core.SiteAdmin';
	$redirect['subView'] = 'core.AdminRepository';
	$redirect['statusId'] = $session->putStatus($status);

	$urlGenerator =& $gallery->getUrlGenerator();
	$templateAdapter->completeProgressBar($urlGenerator->generateUrl($redirect));
	return null;
    }

    /**
     * Passthrough to GalleryCoreApi::fetchPluginStatus, used by test code to
     * allow us to inject mock plugins.
     * @see GalleryCoreApi::fetchPluginStatus
     * @private
     */
    function _fetchPluginStatus($pluginType, $ignoreCache) {
	return GalleryCoreApi::fetchPluginStatus($pluginType, $ignoreCache);
    }
}

/**
 * This view will show all repository-related features.
 */
class AdminRepositoryDownloadView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	global $gallery;

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret, null);
	}

	if ($form['formName'] != 'AdminRepositoryDownload') {
	    $form['formName'] = 'AdminRepositoryDownload';
	}

	/* Init repository. */
	list ($ret, $repositories) = GalleryRepository::getRepositories();
	if ($ret) {
	    return array($ret, null);
	}

	list ($pluginType, $pluginId) =
	    GalleryUtilities::getRequestVariables('pluginType', 'pluginId');

	$AdminRepositoryDownload = array(
	    'pluginId' => $pluginId,
	    'pluginType' => $pluginType);

	$utils = new GalleryRepositoryUtilities();
	list ($ret, $pluginPackages) = $utils->getPluginPackages($pluginType, $pluginId);
	if ($ret) {
	    return array($ret, null);
	}

	foreach (array_keys($pluginPackages) as $code) {
	    if (strpos($code, 'lang-') !== false) {
		$AdminRepositoryDownload['installedLanguages'][substr($code, 5)] = 1;
	    }
	}

	foreach ($repositories as $source => $repository) {
	    if (!$repository->pluginExistsInIndex($pluginType, $pluginId)) {
		continue;
	    }

	    /* Downloading and upgrading plugins are only different in the UI */
	    list ($ret, $upgradeData) = $repository->getPluginUpgradeInfo($pluginType, $pluginId);
	    if ($ret) {
		return array($ret, null);
	    }

            if (!$upgradeData['base']['isCompatible']) {
		continue;
	    }

	    list ($ret, $upgradeData['pluginName']) =
		$repository->getPluginName($pluginType, $pluginId);
	    if ($ret) {
		return array($ret, null);
	    }

	    $upgradeData['languageCount'] = count($upgradeData['languages']);
	    $upgradeData['repository'] = $source;
	    list ($ret, $upgradeData['repositoryName']) =
		GalleryRepository::translateRepositoryName($source);
	    if ($ret) {
		return array($ret, null);
	    }

	    $AdminRepositoryDownload['upgradeData'][] = $upgradeData;

	    if (empty($AdminRepositoryDownload['pluginName'])) {
		$AdminRepositoryDownload['pluginName'] = $upgradeData['pluginName'];
	    }
	}
	usort($AdminRepositoryDownload['upgradeData'], array($this, 'versionComparator'));

	if (!empty($AdminRepositoryDownload['upgradeData'][0]['base']['currentVersion'])) {
	    for ($i = 0; $i < count($AdminRepositoryDownload['upgradeData']); $i++) {
		if ($AdminRepositoryDownload['upgradeData'][$i]['base']['relation'] == 'equal') {
		    break;
		}
		if ($AdminRepositoryDownload['upgradeData'][$i]['base']['relation'] == 'older') {
		    $newEntry['pluginName'] = $AdminRepositoryDownload['pluginName'];
		    $newEntry['base'] = array(
			'newVersion' => $pluginPackages['base']['version'],
			'newBuild' => $pluginPackages['base']['build'],
			'currentVersion' => $pluginPackages['base']['version'],
			'currentBuild' => $pluginPackages['base']['build'],
			'relation' => 'equal');
		    $newEntry['repository'] = 'installed';
		    $newEntry['languages'] = array();
		    $newEntry['languageCount'] = 0;
		    array_splice($AdminRepositoryDownload['upgradeData'], $i, 0, array($newEntry));
		    break;
		}
	    }
	}

	$template->setVariable('AdminRepositoryDownload', $AdminRepositoryDownload);
	$template->setVariable('controller', 'core.AdminRepositoryDownload');
	$template->javascript('modules/core/templates/AdminRepositoryDownload.js');

	return array(null, array('body' => 'modules/core/templates/AdminRepositoryDownload.tpl'));
    }

    function versionComparator($a, $b) {
	$result = version_compare($a['base']['newVersion'], $b['base']['newVersion']);
	if (!$result) {
	    $result = version_compare($a['base']['newBuild'], $b['base']['newBuild']);
	}
	return $result;
    }
}
?>
