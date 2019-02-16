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
 * This controller will handle the deletion of an item
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 15513 $
 */
class ItemDeleteSingleController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	global $gallery;
	$session =& $gallery->getSession();
	$urlGenerator =& $gallery->getUrlGenerator();

	list ($itemId, $navId) = GalleryUtilities::getRequestVariables('itemId', 'navId');

	$status = $error = array();
	if (isset($form['action']['delete'])) {
	    /* Get the root album id, so we don't try to delete it */
	    list ($ret, $rootId) =
		GalleryCoreApi::getPluginParameter('module', 'core', 'id.rootAlbum');
	    if ($ret) {
		return array($ret, null);
	    }

	    /* The view shouldn't let us get this far if we don't have delete permission */
	    $ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.delete');
	    if ($ret) {
		return array($ret, null);
	    }

	    list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	    if ($ret) {
		return array($ret, null);
	    }

	    /* The view shouldn't let us try to delete the root album, either */
	    if ($itemId == $rootId) {
		return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__,
						   "Can't delete the root album"), null);
	    }

	    /* Save URL for trimming from navigation links */
	    $itemUrl = $urlGenerator->generateUrl(
		    array('view' => 'core.ShowItem', 'itemId' => $itemId),
		    array('forceSessionId' => false));

	    /* Delete */
	    $ret = GalleryCoreApi::deleteEntityById($itemId);
	    if ($ret) {
		return array($ret, null);
	    }

	    list ($ret, $success) =
		GalleryCoreApi::guaranteeAlbumHasThumbnail($item->getParentId());
	    if ($ret) {
		return array($ret, null);
	    }
	    /* What do we do if we weren't successful?  No thumbnail, I guess */

	    /* Trim navigation if previous URL is ShowItem for this item */
	    if (!empty($navId)) {
		$navigation = $session->getNavigation($navId);
		if ($i = strpos($itemUrl, '?')) {
		    $itemUrl = substr($itemUrl, 0, $i);
		}
		if (strpos($navigation[0]['returnUrl'], $itemUrl) !== false) {
		    $session->jumpNavigation($navId);
		}
	    }

	    $status['deleted'] = 1;

	    /* Figure out where to redirect upon success */
	    $redirect['view'] = 'core.ItemAdmin';
	    $redirect['subView'] = 'core.ItemDeleteConfirmation';
	    $redirect['itemId'] = $item->getParentId();
	}

	if (!empty($redirect)) {
	    $results['redirect'] = $redirect;
	} else {
	    $results['delegate']['view'] = 'core.ItemAdmin';
	    $results['delegate']['subView'] = 'core.ItemDeleteSingle';
	}
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * This view will prompt for confirmation on the deletion of an item
 */
class ItemDeleteSingleView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	$itemId = GalleryUtilities::getRequestVariables('itemId');

	if ($form['formName'] != 'ItemDeleteSingle') {
	    $form['destination'] = '';
	    $form['formName'] = 'ItemDeleteSingle';
	}

	list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	if ($ret) {
	    return array($ret, null);
	}

	/* Get child counts */
	list ($ret, $childCountTable) = GalleryCoreApi::fetchDescendentCounts(array($itemId));
	if ($ret) {
	    return array($ret, null);
	}
	$childCount = isset($childCountTable[$itemId]) ? $childCountTable[$itemId] : 0;

	$ItemDeleteSingle = array();
	$ItemDeleteSingle['itemTypeNames'] = $item->itemTypeName();
	$ItemDeleteSingle['childCount'] = $childCount;

	$template->setVariable('ItemDeleteSingle', $ItemDeleteSingle);
	$template->setVariable('controller', 'core.ItemDeleteSingle');
	return array(null, array('body' => 'modules/core/templates/ItemDeleteSingle.tpl'));

    }

    /**
     * @see GalleryView::getViewDescription
     */
    function getViewDescription() {
	list ($ret, $core) = GalleryCoreApi::loadPlugin('module', 'core');
	if ($ret) {
	    return array($ret, null);
	}

	list ($ret, $item) = $this->getItem();
	if ($ret) {
	    return array($ret, null);
	}

	$itemTypeNames = $item->itemTypeName(true);

	return array(null, $core->translate(array('text' => 'delete %s',
						  'arg1' => $itemTypeNames[1])));
    }
}
?>
