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
 * Settings for square thumbnails
 * @package SquareThumb
 * @subpackage UserInterface
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 15513 $
 */
class SquareThumbSiteAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret, null);
	}

	$status = $error = array();
	if (isset($form['action']['save'])) {
	    if (empty($form['color'])) {
		$error[] = 'form[error][color][missing]';
	    } else if (!preg_match('/^[0-9a-f]{6}$/i', $form['color'])) {
		$error[] = 'form[error][color][invalid]';
	    } else {
		foreach (array('mode', 'color') as $key) {
		    $ret = GalleryCoreApi::setPluginParameter('module', 'squarethumb',
							      $key, $form[$key]);
		    if ($ret) {
			return array($ret, null);
		    }
		}
		$status['saved'] = 1;
	    }
	} /* else $form['action']['reset'] */

	$method = empty($error) ? 'redirect' : 'delegate';
	$results = array($method => array('view' => 'core.SiteAdmin',
					  'subView' => 'squarethumb.SquareThumbSiteAdmin'),
			 'status' => $status,
			 'error' => $error);

	return array(null, $results);
    }
}

/**
 * Settings for square thumbnails
 */
class SquareThumbSiteAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret, null);
	}

	if ($form['formName'] != 'SquareThumbSiteAdmin') {
	    $form['formName'] = 'SquareThumbSiteAdmin';
	    foreach (array('mode', 'color') as $key) {
		list ($ret, $form[$key]) =
		    GalleryCoreApi::getPluginParameter('module', 'squarethumb', $key);
		if ($ret) {
		    return array($ret, null);
		}
	    }
	}

	$template->setVariable('controller', 'squarethumb.SquareThumbSiteAdmin');
	return array(null,
		     array('body' => 'modules/squarethumb/templates/SquareThumbSiteAdmin.tpl'));
    }
}
?>
