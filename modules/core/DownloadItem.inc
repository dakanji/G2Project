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
 * Provides downloading of binary items
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 15513 $
 */
class DownloadItemView extends GalleryView {

    /**
     * @see GalleryView::isImmediate
     */
    function isImmediate() {
	return true;
    }

    /**
     * @see GalleryView::isAllowedInEmbedOnly
     */
    function isAllowedInEmbedOnly() {
	return true;
    }

    /**
     * @see GalleryView::shouldSaveSession
     */
    function shouldSaveSession() {
	return false;
    }

    /**
     * @see GalleryView::renderImmediate
     */
    function renderImmediate($status, $error) {
	/* Figure out which item we're talking about */
	$itemId = (int) GalleryUtilities::getRequestVariables('itemId');
	if (empty($itemId)) {
	    return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}

	/* Load the item */
	list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	if ($ret) {
	    return $ret;
	}

	/* Figure out the filename */
	list ($ret, $pseudoFileName) = GalleryUtilities::getPseudoFileName($item);
	if ($ret) {
	    return $ret;
	}

	/* Don't allow malicious URLs */
	$fileName = GalleryUtilities::getRequestVariables('fileName');
	if (!empty($fileName) && $fileName != $pseudoFileName) {
	    return GalleryCoreApi::error(GALLERY_ERROR, __FILE__, __LINE__, 'malicious url');
	}

	/* Get the path to the file */
	if (!method_exists($item, 'fetchPath')) {
	    return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
	list ($ret, $path) = $item->fetchPath();
	if ($ret) {
	    return $ret;
	}

	/* Rebuild derivative cache, if necessary */
	$itemForPermission = $itemId;
	$derivativeType = null;
	if (GalleryUtilities::isA($item, 'GalleryDerivative')) {
	    list ($ret, $item) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($itemId);
	    if ($ret) {
		return $ret;
	    }

	    $itemForPermission = $item->getParentId();
	    $derivativeType = $item->getDerivativeType();
	}

	$ret = $this->_sendFile(array('derivativePath' => $path,
				      'derivativeType' => $derivativeType,
				      'mimeType' => $item->getMimeType(),
				      'pseudoFileName' => $pseudoFileName,
				      'parentId' => $itemForPermission,
				      'id' => $itemId));
	if ($ret) {
	    return $ret;
	}

	$ret = GalleryCoreApi::createFastDownloadFile($item);
	    /* Ignore failures since the file has already been sent */

	return null;
    }

    function _sendFile($data) {
	global $gallery;
	$platform =& $gallery->getPlatform();
	$session =& $gallery->getSession();
	$phpVm = $gallery->getPhpVm();

	if (function_exists('apache_setenv')) {
	    @apache_setenv('no-gzip', '1');
	}

	/* Make sure we have permission */
	if (($ids = $session->get('core.isPrintService')) && in_array($data['id'], $ids)) {
	    /* Print services only need core.view to get access to full size version of photos */
	    $permission = 'core.view';
	} else {
	    $permission = 'core.viewSource';
	    switch ($data['derivativeType']) {
	    case DERIVATIVE_TYPE_IMAGE_THUMBNAIL:
		$permission = 'core.view';
		break;

	    case DERIVATIVE_TYPE_IMAGE_RESIZE:
		$permission = 'core.viewResizes';
		break;

		/* DERIVATIVE_TYPE_IMAGE_PREFERRED uses core.viewSource */
	    }
	}
	$ret = GalleryCoreApi::assertHasItemPermission($data['parentId'], $permission);
	if ($ret) {
	    return $ret;
	}

	$requestMethod = strtolower(GalleryUtilities::getServerVar('REQUEST_METHOD'));

	$phpVm->header('Content-type: ' . $data['mimeType']);
	$phpVm->header('Content-Disposition: inline; filename="' . $data['pseudoFileName'] . '"');
	$stats = $platform->stat($data['derivativePath']);
	$phpVm->header('Last-Modified: ' . GalleryUtilities::getHttpDate($stats[9]));
	$phpVm->header('Expires: ' . GalleryUtilities::getHttpDate(time() + 31536000));

	/* If the request method is HEAD, don't send back the body */
	if ($requestMethod == 'head') {
	    $phpVm->header('Content-length: 0');
	} else {
	    if ($stats[7] > 0) {
		$phpVm->header('Content-length: ' . $stats[7]);
	    }
	    /*
	     * Don't use readfile() because it buffers the entire file in memory.  Profiling shows
	     * that this approach is as efficient as fpassthru() but we get to call
	     * guaranteeTimeLimit which prevents it from failing on very large files
	     */
	    if ($fd = $platform->fopen($data['derivativePath'], 'rb')) {
		while (true) {
		    $bits = $platform->fread($fd, 65535);
		    if (strlen($bits) == 0) {
			break;
		    }
		    print $bits;
		    $gallery->guaranteeTimeLimit(30);
		}
		$platform->fclose($fd);
	    }
	}

	return null;
    }
}
?>
