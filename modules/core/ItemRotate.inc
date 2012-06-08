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
 /* You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * This controller will handle moving one or more items from one album to another.
 * @package GalleryCore
 * @subpackage UserInterface
 * @version $Revision: 15513 $
 */
class ItemRotateController extends GalleryController {

	/**
	 * @see GalleryController::handleRequest
	 */
	function handleRequest($form) {
		global $gallery;

		$itemId = GalleryUtilities::getRequestVariables('itemId');

		$status = $error = array();
		if ((isset($form['action']['flip'])) || 
				(isset($form['action']['cw'])) ||
				(isset($form['action']['ccw']))) {
			$operation = 'rotate';
			$preserveOriginal = true;
			if (isset($form['action']['flip'])) {
				$args = array(180);
			} else if (isset($form['action']['cw'])) {
				$args = array(90);
			} else if (isset($form['action']['ccw'])) {
				$args = array(-90);
			}

			// get list of selected ids
			if (empty($error) && !empty($form['selectedIds'])) {
				$selectedIds = array_keys($form['selectedIds']);
			
				// Load the source items
				list ($ret, $selectedItems) = GalleryCoreApi::loadEntitiesById($selectedIds);
				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::studyPermissions($selectedIds);
				if ($ret) {
					return array($ret, null);
				}

				foreach ($selectedItems as $selectedItem) {
					$selectedId = $selectedItem->getId();

					list ($ret, $permissions) = GalleryCoreApi::getPermissions($selectedId);
					if ($ret) {
						return array($ret, null);
					}

					// TODO put in real error checking
					if (!isset($permissions['core.edit'])) {
						$error[] = 'form[error][source][' . $selectedId . '][permission][edit]';
					}
				}
			}
				
			if (empty($error) && !empty($selectedIds) && !empty($args) && !empty($operation)) {
				$batchSize = 100;
				$status['rotated']['count'] = 0;
				do {
					$currentItems = array_splice($selectedItems, 0, $batchSize);
					$currentIds = array();
					foreach ($currentItems as $item) {
						$currentIds[] = $item->getId();
						$ret = null;
						$ret = GalleryCoreApi::applyToolkitOperation(
									$operation, $args, $preserveOriginal, $item);
						if ($ret) {
							return array($ret, null, null, null);
						}
						$status['rotated']['count']++;
					}
				} while (!empty($selectedItems));

				// Figure out where to redirect upon success 
				$redirect['view'] = 'core.ItemAdmin';
				$redirect['subView'] = 'core.ItemRotate';
				$redirect['itemId'] = $itemId;
			}
		} else if (isset($form['action']['next'])) {
			$page = GalleryUtilities::getRequestVariables('page');
			list ($ret, $peerIds) =
			GalleryCoreApi::fetchChildItemIdsWithPermission($itemId, 'core.edit');
			if ($ret) {
				return array($ret, null);
			}

			$numPages = ceil(sizeof($peerIds) / $form['numPerPage']);

			$results['delegate']['itemId'] = $itemId;
			$results['delegate']['page'] = min($page + 1, $numPages);
		} else if (isset($form['action']['previous'])) {
			$page = GalleryUtilities::getRequestVariables('page');
			$results['delegate']['itemId'] = $itemId;
			$results['delegate']['page'] = max($page - 1, 1);
		} else if (isset($form['action']['cancel'])) {
			$results['return'] = true;
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view'] = 'core.ItemAdmin';
			$results['delegate']['subView'] = 'core.ItemRotate';
		}
		$results['status'] = $status;
		$results['error'] = $error;
		
		return array(null, $results);
	}
}

/**
 * This view will prompt for which items to rotate and which direction
 */
class ItemRotateView extends GalleryView {

	/**
	 * @see GalleryView::loadTemplate
	 */
	function loadTemplate(&$template, &$form) {
	global $gallery;

	/* itemId is the album that we want to rotate items in */
	list ($itemId, $selectedId, $page) =
		GalleryUtilities::getRequestVariables('itemId', 'selectedId', 'page');
	if ($form['formName'] != 'ItemRotate') {
		/* First time around, load the form with item data */
		if ($selectedId) {
		$form['selectedIds'][$selectedId] = true;
		}
		$form['destination'] = '';
		$form['formName'] = 'ItemRotate';
		$form['numPerPage'] = 15;
	}

	list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	if ($ret) {
		return array($ret, null);
	}

	/* Get all peers that we can edit */
	list ($ret, $peerIds) =
		GalleryCoreApi::fetchChildItemIdsWithPermission($itemId, 'core.edit');
	if ($ret) {
		return array($ret, null);
	}

	$albumIds = $albumTree = $selectedIds = $peers = $peerDescendentCounts = array();
	$peerTypes = array('album' => array(), 'data' => array());
	$numPages = 1;
	if (!empty($peerIds)) {
		$numPages = ceil(sizeof($peerIds) / $form['numPerPage']);
		if (empty($page)) {
		/* determine which page we're on */
		$page = 1;
		for ($i = 0; $i < sizeof($peerIds); $i++) {
			if ($peerIds[$i] == $selectedId) {
			$page = ceil(($i + 1) / $form['numPerPage']);
			}
		}
		}

		$start = $form['numPerPage'] * ($page - 1);
		$peerIds = array_slice($peerIds, $start, $form['numPerPage']);
		if (isset($form['selectedIds'])) {
		$selectedIds = $form['selectedIds'];
		foreach ($peerIds as $peerId) {
			if (isset($selectedIds[$peerId])) {
			unset($selectedIds[$peerId]);
			}
		}
		}

		/* Add any items with error messages that would otherwise not be shown */
		if (!empty($form['error']['source'])) {
		foreach ($form['error']['source'] as $id => $tmp) {
			if (!in_array($id, $peerIds)) {
			array_unshift($peerIds, $id);
			unset($selectedIds[$id]);
			}
		}
		}

		/* Load all the peers */
		list ($ret, $peerItems) = GalleryCoreApi::loadEntitiesById($peerIds);
		if ($ret) {
		return array($ret, null);
		}

		/* get peer thumbnails and resizes */
		list ($ret, $derivatives) = GalleryCoreApi::fetchDerivativesByItemIds($peerIds);
		if ($ret) {
		return array($ret, null);
		}

		/* Build our peers table */
		$peers = array();
		foreach ($peerItems as $peerItem) {
		$peers[$peerItem->getId()] = (array)$peerItem;
		if (GalleryUtilities::isA($peerItem, 'GalleryAlbumItem')) {
			$peerTypes['album'][$peerItem->getId()] = 1;
		} else {
			$peerTypes['data'][$peerItem->getId()] = 1;
		}
		$peers[$peerItem->getId()]['selected'] =
			isset($form['selectedIds'][$peerItem->getId()]);

		/* While we're at it, attach thumbnails and resizes */
		if (isset($derivatives[$peerItem->getId()])) {
			foreach ($derivatives[$peerItem->getId()] as $derivative) {
			$type = $derivative->getDerivativeType();
			if (empty($peers[$peerItem->getId()]['resize']) &&
					$type == DERIVATIVE_TYPE_IMAGE_RESIZE) {
				$peers[$peerItem->getId()]['resize'] = (array)$derivative;
			} else if ($type == DERIVATIVE_TYPE_IMAGE_THUMBNAIL) {
				$peers[$peerItem->getId()]['thumbnail'] = (array)$derivative;
			}
			}
		}
		}

		/* Get child counts */
		if (!empty($peerTypes['album'])) {
		list ($ret, $peerDescendentCounts) =
			GalleryCoreApi::fetchDescendentCounts(array_keys($peerTypes['album']));
		if ($ret) {
			return array($ret, null);
		}
		}

		/* Get ids of all albums where we can add new data items */
		list ($ret, $albumIds['addDataItem']) =
		GalleryCoreApi::fetchAllItemIds('GalleryAlbumItem', 'core.addDataItem');
		if ($ret) {
		return array($ret, null);
		}

		/* Get ids of all all albums where we can add new album items */
		list ($ret, $albumIds['addAlbumItem']) =
		GalleryCoreApi::fetchAllItemIds('GalleryAlbumItem', 'core.addAlbumItem');
		if ($ret) {
		return array($ret, null);
		}

		/* Merge them together to get the master list of ids */
		$albumIds['allIds'] = array_unique(array_merge($albumIds['addDataItem'],
							$albumIds['addAlbumItem']));

		/* Load all the album entities */
		list ($ret, $albums) = GalleryCoreApi::loadEntitiesById($albumIds['allIds']);
		if ($ret) {
		return array($ret, null);
		}

		$albumTree = GalleryUtilities::createAlbumTree($albums);
	}

	$urlGenerator =& $gallery->getUrlGenerator();

	$ItemRotate = array();
	$ItemRotate['canCancel'] = $urlGenerator->isNavigationBackPossible();
	$ItemRotate['albumIds'] = $albumIds;
	$ItemRotate['peers'] = $peers;
	$ItemRotate['peerTypes'] = $peerTypes;
	$ItemRotate['peerDescendentCounts'] = $peerDescendentCounts;
	$ItemRotate['albumTree'] = $albumTree;
	$ItemRotate['page'] = $page;
	$ItemRotate['numPages'] = $numPages;
	$ItemRotate['numPerPage'] = $form['numPerPage'];
	$ItemRotate['selectedIds'] = array_keys($selectedIds);
	$ItemRotate['selectedIdCount'] = count($selectedIds);

	$template->setVariable('ItemRotate', $ItemRotate);
	$template->setVariable('controller', 'core.ItemRotate');
	$template->javascript('lib/yui/yahoo-min.js');
	$template->javascript('lib/yui/dom-min.js');
	$template->javascript('lib/yui/event-min.js');
	$template->javascript('lib/yui/container-min.js');
	return array(null, array('body' => 'modules/core/templates/ItemRotate.tpl'));
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	function getViewDescription() {
	list ($ret, $core) = GalleryCoreApi::loadPlugin('module', 'core');
	if ($ret) {
		return array($ret, null);
	}

	return array(null, $core->translate('Rotate item'));
	}
}
?>
