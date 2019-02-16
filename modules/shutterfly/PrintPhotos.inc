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
 * This view creates a form to send data to shutterfly.com
 * @package Shutterfly
 * @subpackage UserInterface
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 17265 $
 */
class PrintPhotosView extends GalleryView {

    /**
     * @see GalleryView::isImmediate
     */
    function isImmediate() {
	return true;
    }

    /**
     * @see GalleryView::isControllerLike
     */
    function isControllerLike() {
	return true;
    }

    /**
     * @see GalleryView::renderImmedate
     */
    function renderImmediate($status, $error) {
	global $gallery;
	$urlGenerator =& $gallery->getUrlGenerator();

	list ($itemId, $returnUrl) = GalleryUtilities::getRequestVariables('itemId', 'returnUrl');
	if (!empty($itemId)) {
	    $cartItemIds = array($itemId => 1);
	} else {
	    $session =& $gallery->getSession();
	    $cartItemIds = $session->get('shutterfly.cart');
	    $session->remove('shutterfly.cart');
	}
	if (empty($cartItemIds) || empty($returnUrl)) {
	    return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
	$ret = $this->assertIsInternalUrl($returnUrl);
	if ($ret) {
	    return $ret;
	}

	/* Load the necessary item data */
	$itemIds = array_keys($cartItemIds);
	list ($ret, $items) = GalleryCoreApi::loadEntitiesById($itemIds);
	if ($ret) {
	    return $ret;
	}

	list ($ret, $thumbnails) = GalleryCoreApi::fetchThumbnailsByItemIds($itemIds);
	if ($ret) {
	    return $ret;
	}

	list ($ret, $resizes) = GalleryCoreApi::fetchResizesByItemIds($itemIds);
	if ($ret) {
	    return $ret;
	}

	list ($ret, $preferreds) = GalleryCoreApi::fetchPreferredsByItemIds($itemIds);
	if ($ret) {
	    return $ret;
	}

	$ret = GalleryCoreApi::studyPermissions($itemIds);
	if ($ret) {
	    return $ret;
	}

	/* We want to know which are viewable to guests */
	list ($ret, $anonymousUserId) =
	    GalleryCoreApi::getPluginParameter('module', 'core', 'id.anonymousUser');
	if ($ret) {
	    return $ret;
	}
	$ret = GalleryCoreApi::studyPermissions($itemIds, $anonymousUserId, false);
	if ($ret) {
	    return $ret;
	}

	/* Assemble all our data */
	$i = 1;
	$entries = $protectedIds = array();
	foreach ($items as $item) {
	    $itemId = $item->getId();
	    list ($ret, $permissions) = GalleryCoreApi::getPermissions($itemId);
	    if ($ret) {
		return $ret;
	    }
	    list ($ret, $publicPermissions) =
		GalleryCoreApi::getPermissions($itemId, $anonymousUserId, false);
	    if ($ret) {
		return $ret;
	    }

	    if (!isset($permissions['shutterfly.print']) || !isset($permissions['core.view'])) {
		/* Skip any cart items for which we don't have permission */
		continue;
	    }

	    $source = isset($preferreds[$itemId]) ? $preferreds[$itemId] : $item;
	    $needSession = !isset($publicPermissions['core.viewSource']);

	    if ($needSession && !isset($sfSession)) {
		/*
		 * Get G2 session for shutterfly to access non-public images.
		 * We can't use this session because hijack protection will prevent access
		 * plus the current user could logout before shutterfly retrieves the images.
		 * Create a new session with the rights of current user for shutterfly to use.
		 */
		$sfSession = new GallerySession();
		$ret = $sfSession->initEmpty(true, $gallery->getActiveUserId());
		if ($ret) {
		    return $ret;
		}
	    }
	    if ($needSession) {
		$sessionParam = array($sfSession->getKey() => $sfSession->getId());
		$protectedIds[] = $source->getId();
	    } else {
		$sessionParam = array();
	    }

	    $entry = array('item' => (array)$item);
	    $entry['imageUrl'] = $urlGenerator->generateUrl(
		array_merge(array('view' => 'core.DownloadItem', 'itemId' => $source->getId()),
			    $sessionParam),
		array('forceSessionId' => false, 'forceFullUrl' => true));
	    $entry['imageWidth'] = $source->getWidth();
	    $entry['imageHeight'] = $source->getHeight();

	    if (!isset($thumbnails[$itemId]) || $thumbnails[$itemId]->getPostFilterOperations()) {
		/* Use the source if the thumbnail has a postfilter (like a watermark) */
		$thumbSource = $source;
	    } else {
		$thumbSource = $thumbnails[$itemId];
	    }

	    $entry['thumbUrl'] = $urlGenerator->generateUrl(
		array('view' => 'core.DownloadItem', 'itemId' => $thumbSource->getId()),
		array('forceSessionId' => $needSession, 'forceFullUrl' => true));
	    $entry['thumbWidth'] = $thumbSource->getWidth();
	    $entry['thumbHeight'] = $thumbSource->getHeight();

	    /*
	     * Ugh, the Shutterfly api doesn't have a parameter for quantity.
	     * Repeat the same entry multiple times to get desired quantity!
	     */
	    for ($j = 0; $j < $cartItemIds[$itemId]; $j++) {
		$entries[$i++] = $entry;
	    }
	}

	if (isset($sfSession)) {
	    /* Mark this session so that it can be treated specially */
	    $sfSession->put('core.isPrintService', $protectedIds);

	    /*
	     * TODO: Would like to enforce a particular session timeout to ensure this session
	     * lasts long enough for shutterfly to retrieve the images.  Maybe also store the
	     * sessionid in this session so we can reuse it for multiple print requests (and
	     * just bump timeout each time).
	     */
	    $ret = $sfSession->save();
	    if ($ret) {
		return $ret;
	    }
	}

	/*
	 * Ugh, the Shutterfly api can only track its session data via some cookies
	 * (redirecting to a url with embedded session id won't work) so we must
	 * render a form and submit it.. here we set our own cookie that will be
	 * checked to ensure we submit our form only once.
	 */
	if (!headers_sent() /* Should only skip cookie in unit test */) {
	    setcookie('G2_shutterfly', '1');
	    header("Content-type: text/html; charset=UTF-8");
	}

	GalleryCoreApi::requireOnce('modules/core/classes/GalleryTemplate.class');
	$template = new GalleryTemplate(dirname(dirname(dirname(__FILE__))));
	$template->setVariable('l10Domain', 'modules_shutterfly');
	$template->setVariable('PrintPhotos', array('returnUrl' => $returnUrl,
						    'count' => count($entries),
						    'entries' => $entries));
	$template->display('gallery:modules/shutterfly/templates/PrintPhotos.tpl');

	return null;
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
