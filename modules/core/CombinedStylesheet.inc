<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * Pack many Stylesheet files together and download them all at once.
 *
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Dayo Akanji <http://dakanji.com>
 * @version $Revision: 0 $
 */
class CombinedStylesheetView extends GalleryView {
    /**
     * @see GalleryView::isImmediate
     */
    function isImmediate() {
	return true;
    }

    /**
     * @see GalleryView::renderImmediate
     */
    function renderImmediate() {
	global $gallery;
	$platform =& $gallery->getPlatform();
	$phpVm = $gallery->getPhpVm();

	$ifModifiedSince = GalleryUtilities::getModifiedHeader();
	if (isset($ifModifiedSince)) {
	    $phpVm->header('HTTP/1.0 304 Not Modified');
	    return null;
	}

	$key = GalleryUtilities::getRequestVariables('key');
	if (preg_match('/[^0-9a-f]/', $key)) {
	    /* The key can't contain non-hex, so just terminate early */
	    return null;
	}

 	if (strpos(GalleryUtilities::getServerVar('HTTP_ACCEPT_ENCODING'), 'gzip') !== false) {
	    $cacheData =& GalleryDataCache::getFromDisk(
		array('type' => 'module',
		      'itemId' => 'CombinedStylesheet_gzip_' . $key,
		      'id' => '_all'));
	    if (isset($cacheData)) {
		/**
		 * Try to prevent Apache's mod_deflate from gzipping the output a 2nd time
		 * since broken versions of mod_deflate sometimes get the byte count wrong.
		 */
		if ($phpVm->function_exists('apache_setenv')
		        && !@$gallery->getConfig('apacheSetenvBroken')) {
		    @apache_setenv('no-gzip', '1');
		}

		$phpVm->header('Content-Encoding: gzip');
		/*
		 * Cache control is public for proxies that handle gzip properly.
		 * Private for the ones that don't.
		 * http://www.mnot.net/blog/2005/05/12/google_cache
		 */
		$phpVm->header('Cache-Control: private, x-gzip-ok="public"');
	    }
 	}

	if (!isset($cacheData)) {
	    $cacheData =& GalleryDataCache::getFromDisk(
		array('type' => 'module',
		      'itemId' => 'CombinedStylesheet_' . $key,
		      'id' => '_all'));

	    if (isset($cacheData)) {
			$phpVm->header('Cache-Control: public');
	    }
	}

	if (isset($cacheData)) {
	    $phpVm->header('Content-type: text/css; charset=UTF-8');
	    $phpVm->header('Last-Modified: ' . $gallery->getHttpDate(1));
	    $phpVm->header('Expires: ' . $gallery->getHttpDate($phpVm->time() + (3600 * 24 * 30)));
	    print $cacheData;
	} else {
	    return GalleryCoreApi::addEventLogEntry('core.CombinedStylesheet', 'Unable to read combined cache file', $key);
	}
	return null;
    }

    /**
     * @see GalleryView::autoCacheControl
     */
    function autoCacheControl() {
	return false;
    }

    /**
     * @see GalleryView::isAllowedInEmbedOnly
     */
    function isAllowedInEmbedOnly() {
	return true;
    }
}
?>