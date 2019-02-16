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
 * Display the slideshow
 * @package Slideshow
 * @subpackage UserInterface
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 17265 $
 */
class SlideshowView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	global $gallery;
	$SlideShow = array();
	$startId = null;

	$itemListString = GalleryUtilities::getRequestVariables('itemList');
	if (!empty($itemListString)) {
	    /* Build slideshow using specified items */
	    $item = explode('|', $itemListString);
	} else {
	    /* Build slideshow for an album */
	    list ($ret, $item) = $this->getItem();
	    if ($ret) {
		return array($ret, null);
	    }
	    list ($ret, $canView) = GalleryCoreApi::hasItemPermission($item->getId(), 'core.view');
	    if ($ret) {
		return array($ret, null);
	    }
	    if (!$canView) {
		/* Avoid information disclosure, act as if the item didn't exist. */
		return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
	    }
	    if (!$item->getCanContainChildren()) {
		/* Photo: do slideshow of parent album, start at this photo */
		$startId = $item->getId();
		list ($ret, $item) = GalleryCoreApi::loadEntitiesById($item->getParentId());
		if ($ret) {
		    return array($ret, null);
		}
		list ($ret, $canView) =
		    GalleryCoreApi::hasItemPermission($item->getId(), 'core.view');
		if ($ret) {
		    return array($ret, null);
		}
		if (!$canView) {
		    /* Avoid information disclosure, act as if the item didn't exist. */
		    return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}
	    }
	    $SlideShow['item'] = (array)$item;
	}

	list ($ret, $itemList, $start) =
	    $this->buildSlideshow($item, $startId);
	if ($ret) {
	    return array($ret, null);
	}

	$SlideShow['itemList'] = $itemList;
	$SlideShow['count'] = count($itemList);
	$SlideShow['start'] = $start;
	$template->setVariable('SlideShow', $SlideShow);
	$template->head('modules/slideshow/templates/Header.tpl');

	list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'slideshow');
	if ($ret) {
	    return array($ret, null);
	}
	if (!empty($SlideShow['item']['title'])) {
	    $template->title($module->translate(array('text' => 'Slideshow for %s',
						      'arg1' => $SlideShow['item']['title'])));
	} else {
	    $template->title($module->translate('Slideshow'));
	}

	return array(null, array('body' => 'modules/slideshow/templates/Slideshow.tpl',
				 'useFullScreen' => true));
    }

    /**
     * Build data set to be used by template to render slideshow.
     * If $item parameter is a single object then build using all children
     * of that item; if an array then build using the ids in the array.
     */
    function buildSlideshow($item, $startId = null, $recursive = false) {
	global $gallery;
	if (!isset($this->init)) {
	    $this->sizeClasses = array(
		array(320,320),     /* 0:   <320x240  */
		array(640,640),     /* 1:  ..640x480  */
		array(800,800),     /* 2:  ..800x600  */
		array(1024,1024),   /* 3: ..1024x768  */
		array(1280,1280),   /* 4: ..1280x1024 */
	    );                      /* 5:  >1280x1024 */
	    $this->init = 1;
	}
	$checkViewPermission = true;
	if (is_array($item)) {
	    $childIds = $item;
	} else {
	    list ($ret, $childIds) = GalleryCoreApi::fetchChildItemIds($item);
	    if ($ret) {
		return array($ret, null, null);
	    }
	    $checkViewPermission = false;
	}
	$itemList = array();
	$childItems = array();
	if (!empty($childIds)) {
	    $gallery->guaranteeTimeLimit(30);
	    list ($ret, $permissions) = GalleryCoreApi::fetchPermissionsForItems($childIds);
	    if ($ret) {
		return array($ret, null, null);
	    }
	    if ($checkViewPermission) {
		foreach ($childIds as $i => $id) {
		    if (!isset($permissions[$id]['core.view'])) {
			/* Avoid information disclosure, act as if the item didn't exist. */
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null);
		    }
		}
	    }
	    list ($ret, $childItems) = GalleryCoreApi::loadEntitiesById($childIds);
	    if ($ret) {
		return array($ret, null, null);
	    }
	    list ($ret, $preferredFullImages) =
		GalleryCoreApi::fetchPreferredsByItemIds($childIds);
	    if ($ret) {
		return array($ret, null, null);
	    }
	    list ($ret, $resizedImages) =
		GalleryCoreApi::fetchResizesByItemIds($childIds);
	    if ($ret) {
		return array($ret, null, null);
	    }
	    list ($ret, $thumbnails) =
		GalleryCoreApi::fetchThumbnailsByItemIds($childIds);
	    if ($ret) {
		return array($ret, null, null);
	    }
	}
	if (!isset($startId)) {
	    $start = 0;
	}
	foreach ($childItems as $child) {
	    if (count($itemList) % 100 == 0) {
		$gallery->guaranteeTimeLimit(30);
	    }

	    $childId = $child->getId();
	    if (!isset($start) && $childId == $startId) {
		$start = count($itemList);
	    }

	    if ($child->getCanContainChildren()) {

		if ($recursive) {
		    list ($ret, $sublist) =
			$this->buildSlideshow($child, null, $recursive);
		    if ($ret) {
			return array($ret, null, null);
		    }
		    $itemList = array_merge($itemList, $sublist);
		}

	    } else if (GalleryUtilities::isA($child, 'GalleryPhotoItem')) {
		$images = array();
		if (isset($permissions[$childId]['core.viewSource'])) {
		    /* Full size; check for preferred copy */
		    if (isset($preferredFullImages[$childId])) {
			$images[] = $preferredFullImages[$childId];
		    } else if ($child->canBeViewedInline()) {
			$images[] = $child;
		    }
		}
		if (isset($permissions[$childId]['core.viewResizes']) &&
			isset($resizedImages[$childId])) {
		    foreach ($resizedImages[$childId] as $resize) {
			$images[] = $resize;
		    }
		}
		if (isset($thumbnails[$childId])) {
		    $images[] = $thumbnails[$childId];
		}

		/* build table of {sizeClass}=>{sources_index} */
		$sizeClassMap = $sizeClassArea = array();
		for ($i = 0; $i <= count($this->sizeClasses); $i++) {
		    $sizeClassMap[] = $sizeClassArea[] = -1;
		}
		for ($i = 0; $i < sizeof($images); $i++) {
		    $image = $images[$i];
		    if (GalleryUtilities::isA($image, 'GalleryDerivativeImage')) {
			if (!($image->getWidth() && $image->getHeight())) {
			    list ($ret, $image) =
				GalleryCoreApi::rebuildDerivativeCache($image->getId());
			    if ($ret) {
				return array($ret, null, null);
			    }
			}
		    }
		    $image = $images[$i] = (array)$image;
		    if (!isset($image['width']) || !isset($image['height'])) {
			continue; /* Should not happen */
		    }
		    $area = $image['width'] * $image['height'];
		    for ($j = count($this->sizeClasses); $j >= 0; $j--) {
			if ($j < count($this->sizeClasses) &&
			      ($image['width'] > $this->sizeClasses[$j][0] ||
			       $image['height'] > $this->sizeClasses[$j][1])) {
			    break;
			}
			if ($area > $sizeClassArea[$j]) {
			    $sizeClassMap[$j] = $i;
			    $sizeClassArea[$j] = $area;
			}
		    }
		}

		for ($j = 0, $i = count($this->sizeClasses); $i >= 0; $i--) {
		    if ($sizeClassMap[$i] < 0) {
			$sizeClassMap[$i] = $j;
		    } else {
			$j = $sizeClassMap[$i];
		    }
		}

		if (!empty($images)) {
		    $itemList[] = array(
			'id' => $childId,
			'data' => (array)$child,
			'sources' => $images,
			'sizeClassMap' => $sizeClassMap);
		}
	    }
	}
	return array(null, $itemList, isset($start) ? $start : 0);
    }
}
?>
