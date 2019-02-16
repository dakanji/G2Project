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
 * Settings for comments
 * @package Comment
 * @subpackage UserInterface
 * @author Johannes Schmittat <Johannes@Schmittat.com>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 15513 $
 */
class CommentSiteAdminController extends GalleryController {

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
	   if (!isset($form['show']) || !is_numeric($form['show'])) {
		$error[] = 'form[error][show]';
	    }

	    if (empty($error)) {
		$ret = GalleryCoreApi::setPluginParameter('module', 'comment',
							  'comments.show', $form['show']);
		if ($ret) {
		    return array($ret, null);
		}
		$ret = GalleryCoreApi::setPluginParameter('module', 'comment',
			'comments.latest', (isset($form['latest']) && $form['latest']) ? 1 : 0);
		if ($ret) {
		    return array($ret, null);
		}
		$status['saved'] = 1;
	    }
	} /* else $form['action']['reset'] */

	/* Figure out where to redirect upon success */
	$method = empty($error) ? 'redirect' : 'delegate';
	$results[$method]['view'] = 'core.SiteAdmin';
	$results[$method]['subView'] = 'comment.CommentSiteAdmin';
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * Settings for comments
 */
class CommentSiteAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret, null);
	}

	if ($form['formName'] != 'CommentSiteAdmin') {
	    $form['formName'] = 'CommentSiteAdmin';
	    foreach (array('latest', 'show') as $key) {
		list ($ret, $value) =
		    GalleryCoreApi::getPluginParameter('module', 'comment', 'comments.' . $key);
		if ($ret) {
		    return array($ret, null);
		}
		$form[$key] = $value;
	    }
	} else if (!isset($form['latest'])) {
	    $form['latest'] = false;
	}

	list ($ret, $pluginInstances) =
	    GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');
	if ($ret) {
	    return array($ret, null);
	}
	$form['canValidate'] = !empty($pluginInstances);

	$template->setVariable('controller', 'comment.CommentSiteAdmin');
	return array(null,
		     array('body' => 'modules/comment/templates/CommentSiteAdmin.tpl'));
    }
}
?>
