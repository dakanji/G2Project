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
 * This view will show the comments stored for an item
 * @package Comment
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 15513 $
 */
class ShowCommentsView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	list ($ret, $item) = $this->getItem();
	if ($ret) {
	    return array($ret, null);
	}

	GalleryCoreApi::requireOnce('modules/comment/classes/GalleryCommentHelper.class');
	list ($ret, $comments) =
	    GalleryCommentHelper::fetchComments($item->getId(), null, ORDER_ASCENDING);
	if ($ret) {
	    return array($ret, null);
	}

	$commenters = array();
	if (!empty($comments)) {
	    for ($i = 0; $i < sizeof($comments); $i++) {
		/* Get the commenter ids */
		$commenters[$comments[$i]->getCommenterId()] = 1;

		/* Extract the member data */
		$comments[$i] = (array)$comments[$i];
	    }

	    /* Load all the commenters */
	    if (sizeof($commenters) > 0) {
		list ($ret, $commentersList) =
		    GalleryCoreApi::loadEntitiesById(array_keys($commenters));
		if ($ret) {
		    return array($ret, null);
		}
	    }

	    foreach ($commentersList as $commenter) {
		$commenters[$commenter->getId()] = (array)$commenter;
	    }
	}

	list ($ret, $permissions) = GalleryCoreApi::getPermissions($item->getId());
	if ($ret) {
	    return array($ret, null);
	}

	foreach (array('delete' => 'comment.delete',
		       'edit' => 'comment.edit',
		       'view' => 'comment.view')
		 as $canFlag => $permission) {
	    $can[$canFlag] = isset($permissions[$permission]);
	}

	$template->setVariable('ShowComments', array(
	    'comments' => $comments,
	    'commenters' => $commenters,
	    'can' => $can,
	    'item' => (array)$item));

	return array(null,
		     array('body' => 'modules/comment/templates/ShowComments.tpl'));
    }

    /**
     * @see GalleryView::getViewDescription
     */
    function getViewDescription() {
	list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'comment');
	if ($ret) {
	    return array($ret, null);
	}

	return array(null, $module->translate('show comments'));
    }
}
?>
