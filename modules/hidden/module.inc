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
 * Hidden Items
 *
 * This module provides the ability to hide items and allow them to be viewed only
 * by directly accessing the correct URL.
 *
 * @package Hidden
 * @author Alan Harder <alan.harder@sun.com>
 * @author Jess Martin <jmartin@cs.unc.edu>
 * @version $Revision: 16034 $
 */
class HiddenModule extends GalleryModule /* and GalleryEventListener */ {

    function HiddenModule() {
	global $gallery;

	$this->setId('hidden');
	$this->setName($gallery->i18n('Hidden Items'));
	$this->setDescription($gallery->i18n('Hide items/albums from guests until directly ' .
					     'accessed with the right URL'));
	$this->setVersion('1.0.4');
	$this->setGroup('display', $this->translate('Display'));
	$this->setCallbacks('registerEventListeners');
	$this->setRequiredCoreApi(array(7, 4));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::registerEventListeners
     */
    function registerEventListeners() {
	GalleryCoreApi::registerEventListener('GalleryEntity::save', new HiddenModule());
    }

    /**
     * @see GalleryModule::performFactoryRegistrations
     */
    function performFactoryRegistrations() {
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'GalleryOnLoadHandler', 'HiddenModule', 'Hidden',
	    'modules/hidden/module.inc', 'hidden', null);
	if ($ret) {
	    return $ret;
	}

	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ItemEditOption', 'HiddenItemOption', 'HiddenItemOption',
	    'modules/hidden/HiddenItemOption.inc', 'hidden', array('ItemEditItem'));
	if ($ret) {
	    return $ret;
	}

	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'HiddenInterface_1_0', 'HiddenHelper', 'Hidden',
	    'modules/hidden/classes/HiddenHelper.class', 'hidden', null);
	if ($ret) {
	    return $ret;
	}

	return null;
    }

    /**
     * @see GalleryModule::getOnLoadHandlerIds
     */
    function getOnLoadHandlerIds() {
	return array('Hidden');
    }

    /**
     * Unhide all hidden items.
     * @see GalleryModule::uninstall
     */
    function uninstall() {
	global $gallery;
	GalleryCoreApi::requireOnce('modules/hidden/classes/HiddenHelper.class');

	$query = '
	    SELECT [GalleryEntity::id]
	    FROM [GalleryEntity]
	    WHERE [GalleryEntity::onLoadHandlers] LIKE \'%|Hidden|%\'
	    ';
	list ($ret, $searchResults) = $gallery->search($query);
	if ($ret) {
	    return $ret;
	}
	while ($result = $searchResults->nextResult()) {
	    $itemIds[] = (int)$result[0];
	}

	while (!empty($itemIds)) {
	    list ($ret, $items) = GalleryCoreApi::loadEntitiesById(array_splice($itemIds, 0, 100));
	    if ($ret) {
		return $ret;
	    }
	    foreach ($items as $item) {
		$ret = HiddenHelper::unHideItem($item);
		if ($ret) {
		    return $ret;
		}
	    }
	}

	$ret = parent::uninstall();
	if ($ret) {
	    return $ret;
	}
	return null;
    }

    /**
     * Add session based permission when core.ShowItem accessed directly for this item
     */
    function onLoad(&$item, $duringUpgrade) {
	list ($view, $itemId) = GalleryUtilities::getRequestVariables('view', 'itemId');
	if ($view == 'core.ShowItem' && $itemId == $item->getId()) {
	    list ($ret, $permissions) = GalleryCoreApi::getPermissions($itemId);
	    if ($ret) {
		return $ret;
	    }
	    if (!isset($permissions['core.view']) && (isset($permissions['core.viewResizes'])
		    || isset($permissions['core.viewSource']))) {
		global $gallery;
		$session =& $gallery->getSession();
		if (!$session->exists(GALLERY_PERMISSION_SESSION_KEY)) {
		    $session->put(GALLERY_PERMISSION_SESSION_KEY, array());
		}
		$sessionPerms =& $session->get(GALLERY_PERMISSION_SESSION_KEY);
		if (!in_array($itemId, $sessionPerms)) {
		    $sessionPerms[] = (int)$itemId;
		}

		/* Use a blunt instrument to clear the permission cache. */
		GalleryDataCache::reset();
	    }
	}
	return null;
    }

    /**
     * Update permissions when items move in/out of hidden albums
     * @see GalleryEventListener::handleEvent
     */
    function handleEvent($event) {
	$item = $event->getEntity();
	if (GalleryUtilities::isA($item, 'GalleryItem')
		&& !$item->testPersistentFlag(STORAGE_FLAG_NEWLY_CREATED)
		&& $item->isModified('parentId')
		&& !$item->hasOnLoadHandler('Hidden')) {
	    GalleryCoreApi::requireOnce('modules/hidden/classes/HiddenHelper.class');
	    $ret = HiddenHelper::handleMoveItem($item);
	    if ($ret) {
		return array($ret, null);
	    }
	}
	return array(null, null);
    }
}
?>
