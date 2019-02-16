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
 * This controller handles printing a single photo
 * @package PhotoAccess
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 17265 $
 */
class PrintPhotoController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	list ($itemId, $returnUrl) = GalleryUtilities::getRequestVariables('itemId', 'returnUrl');
	$ret = $this->assertIsInternalUrl($returnUrl);
	if ($ret) {
	    return array($ret, null);
	}

	GalleryCoreApi::requireOnce('modules/photoaccess/classes/PhotoAccessHelper.class');
	list ($ret, $cartUrl) = PhotoAccessHelper::printPhotos(array($itemId => 1), $returnUrl);
	if ($ret) {
	    return array($ret, null);
	}

	/* Prepare our results */
	$results['redirect']['href'] = $cartUrl;
	$results['status'] = array();
	$results['error'] = array();

	return array(null, $results);
    }

    /**
     * Assert that the given URL is internal to the application
     * @param string $url URL to check
     * @return GalleryStatus a status code
     * @deprecated This is a copy of GalleryUrlGenerator::assertIsInternalUrl from core API 7.43.
     */
    function assertIsInternalUrl($url) {
	global $gallery;
	$urlGenerator =& $gallery->getUrlGenerator();

	if (!empty($url)) {
	    /* Detect header injection attempts */
	    if (!GalleryUtilities::isSafeHttpHeader($url)) {
		$message = sprintf('Invalid URL! The requested URL %s contains malicious '
				 . 'characters.',
				   $urlGenerator->makeUrl($urlGenerator->getCurrentRequestUri()));
		return GalleryCoreApi::error(ERROR_PERMISSION_DENIED, __FILE__, __LINE__, $message);
	    }

	    /*
	     * Check for phishing attacks, don't allow return URLs to other sites or to other paths.
	     * Therefore first get the validPath, e.g. '/gallery2/' Do not allow ../ to break out of
	     * the path Allow all URLs that don't start with a protocol and neither with '/', eg.
	     * v/albumname but also www.EVIL.com is fine, since it's interpreted as a relative URL
	     */
	    $validPath = '/'
		. str_replace($urlGenerator->makeUrl(''), '', $urlGenerator->getCurrentUrlDir());
	    /*
	     * We check for ../ and /../ patterns and on windows \../ would also break out,
	     * normalize to URL / *nix style paths to check fewer cases
	     */
	    $normUrl = str_replace("\\", '/', $url);
	    if (((empty($urlGenerator->_file[0]) || strpos($url, $urlGenerator->_file[0]) !== 0)
			&& strpos($normUrl, $validPath) !== 0
			&& strpos($url, $urlGenerator->getCurrentUrlDir()) !== 0
			&& !( !preg_match('{^\s*\w*://}i', $normUrl)
			    && preg_match('{^\s*[^/\s]}i', $normUrl)))
		    || preg_match('{^\s*\.\./}', $normUrl)
		    || strpos($normUrl, '/../') !== false) {
		$message = sprintf('Invalid URL! The requested URL %s tried to insert a '
				 . 'redirection to %s which is not a part of this Gallery.',
				   $urlGenerator->makeUrl($urlGenerator->getCurrentRequestUri()),
				   $url);
		return GalleryCoreApi::error(ERROR_PERMISSION_DENIED, __FILE__, __LINE__, $message);
	    }
	}
	return null;
    }
}
?>
