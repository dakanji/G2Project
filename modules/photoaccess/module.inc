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
 * Photo printing at photoworks.com
 *
 * @package PhotoAccess
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 17265 $
 */
class PhotoAccessModule extends GalleryModule {

    function PhotoAccessModule() {
	global $gallery;

	$this->setId('photoaccess');
	$this->setName($gallery->i18n('PhotoWorks'));
	$this->setDescription($gallery->i18n('PhotoWorks Photo Printing Module'));
	$this->setVersion('1.0.7.1'); /* Update upgrade() too */
	$this->setGroup('commerce', $gallery->i18n('Commerce'));
	$this->setCallbacks('getItemLinks');
	$this->setRequiredCoreApi(array(7, 12));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::upgrade
     */
    function upgrade($currentVersion) {
	global $gallery;
	if (!isset($currentVersion)) {
	    $currentVersion = '0';
	} else if (version_compare($currentVersion, '1.0.0', '<')) {
	    /* Instead of enumerating all previous versions... */
	    $currentVersion = '1.0.0';
	}
	list ($ret, $coreParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');
	if ($ret) {
	    return $ret;
	}

	switch ($currentVersion) {
	case '0':
	    /* Register our permission */
	    $ret = GalleryCoreApi::registerPermission($this->getId(), 'photoaccess.print',
						      $gallery->i18n('[photoaccess] Print'));
	    if ($ret) {
		return $ret;
	    }
	    /* Give everybody print permission by default */
	    $ret = GalleryCoreApi::addGroupPermission(
		$coreParams['id.rootAlbum'], $coreParams['id.everybodyGroup'],
		'photoaccess.print', true);
	    if ($ret) {
		return $ret;
	    }
	    break;

	case '1.0.0':
	    $ret = GalleryCoreApi::registerPermission($this->getId(), 'photoaccess.print',
						      $gallery->i18n('[photoaccess] Print'));
	    if ($ret) {
		return $ret;
	    }
	    $ret = GalleryCoreApi::addGroupPermission(
		$coreParams['id.rootAlbum'], $coreParams['id.everybodyGroup'],
		'photoaccess.print', true);
	    if ($ret) {
		return $ret;
	    }

	case '1.0.1':
	case '1.0.2':
	case '1.0.3':
	case '1.0.4':
	case '1.0.5':
	case '1.0.6':
	case '1.0.7':

	case 'end of upgrade path':
	    break;

	default:
	    return GalleryCoreApi::error(ERROR_BAD_PLUGIN, __FILE__, __LINE__,
					 sprintf('Unknown module version %s', $currentVersion));
	}

	return null;
    }

    /**
     * @see GalleryModule::performFactoryRegistrations
     */
    function performFactoryRegistrations() {
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'CartPluginInterface_1_0', 'PhotoAccessCartPlugin', 'photoaccess',
	    'modules/photoaccess/classes/PhotoAccessCartPlugin.class', 'photoaccess', null);
	if ($ret) {
	    return $ret;
	}

	return null;
    }

    /**
     * @see GalleryModule::getItemLinks
     */
    function getItemLinks($items, $wantsDetailedLinks, $permissions) {
	$links = array();

	foreach ($items as $item) {
	    $itemId = $item->getId();
	    if (isset($wantsDetailedLinks[$itemId]) &&
		    isset($permissions[$itemId]['photoaccess.print']) &&
		    GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
		$links[$itemId][] =
		    array('text' => $this->translate('Print on PhotoWorks.com'),
			  'params' => array('controller' => 'photoaccess.PrintPhoto',
					    'itemId' => $itemId, 'returnUrl' => '%CURRENT_URL%'));
	    }
	}

	return array(null, $links);
    }
}
?>
