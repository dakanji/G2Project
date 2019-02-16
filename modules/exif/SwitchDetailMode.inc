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
 * This controller will switch the detail mode in the session and return to the previous page
 * @package Exif
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 15513 $
 */
class SwitchDetailModeController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest() {
	global $gallery;

	$mode = GalleryUtilities::getRequestVariables('mode');
	if ($mode != 'summary' && $mode != 'detailed') {
	    $mode = 'summary';
	}

	/* Store the new mode in the session */
	$session =& $gallery->getSession();
	$session->put('exif.module.LoadExifInfo.mode', $mode);

	$results['return'] = 1;
	$results['redirect']['view'] = 'core.ShowItem';
	$results['status'] = array();
	$results['error'] = array();

	return array(null, $results);
    }

}
?>
