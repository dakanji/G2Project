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
 * Thumbnail defaults
 *
 * This module provides default thumbnails for non-image mime types.
 *
 * @package Thumbnail
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 16635 $
 */
class ThumbnailModule extends GalleryModule {

    function ThumbnailModule() {
	global $gallery;

	$this->setId('thumbnail');
	$this->setName($gallery->i18n('Thumbnail Manager'));
	$this->setDescription($gallery->i18n('Set default thumbnails for non-image items'));
	$this->setVersion('1.0.9.1'); /* Update ThumbnailModuleExtras::upgrade also! */
	$this->setGroup('display', $gallery->i18n('Display'));
	$this->setCallbacks('getSiteAdminViews|registerEventListeners');
	$this->setRequiredCoreApi(array(7, 4));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::upgrade
     */
    function upgrade($currentVersion) {
	GalleryCoreApi::requireOnce('modules/thumbnail/ThumbnailModuleExtras.inc');
	$ret = ThumbnailModuleExtras::upgrade($this, $currentVersion);
	if ($ret) {
	    return $ret;
	}

	return null;
    }

    /**
     * @see GalleryModule::performFactoryRegistrations
     */
    function performFactoryRegistrations() {
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'GalleryEntity', 'ThumbnailImage', 'ThumbnailImage',
	    'modules/thumbnail/classes/ThumbnailImage.class', 'thumbnail', null);
	if ($ret) {
	    return $ret;
	}

	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'GalleryToolkit', 'ThumbnailToolkit', 'Thumbnail',
	    'modules/thumbnail/classes/ThumbnailToolkit.class', 'thumbnail', null);
	if ($ret) {
	    return $ret;
	}

	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ItemEditOption', 'CustomThumbnailOption', 'CustomThumbnailOption',
	    'modules/thumbnail/CustomThumbnailOption.inc', 'thumbnail', array('ItemEditItem'));
	if ($ret) {
	    return $ret;
	}

	return null;
    }

    /**
     * @see GalleryModule::getModuleEntityTypes
     */
    function getModuleEntityTypes() {
	return array('ThumbnailImage');
    }

    /**
     * @see GalleryModule::registerEventListeners
     */
    function registerEventListeners() {
	GalleryCoreApi::registerEventListener('GalleryEntity::delete', new ThumbnailModule());
    }

    /**
     * @see GalleryModule::activate
     */
    function activate($postActivationEvent=true) {
	GalleryCoreApi::requireOnce('modules/thumbnail/ThumbnailModuleExtras.inc');
	list ($ret, $ignored) = ThumbnailModuleExtras::activate($postActivationEvent);
	if ($ret) {
	    return array($ret, null);
	}

	list ($ret, $redirect) = parent::activate($postActivationEvent);
	if ($ret) {
	    return array($ret, null);
	}

	return array(null, $redirect);
    }

    /**
     * @see GalleryModule::uninstall
     */
    function uninstall() {
	GalleryCoreApi::requireOnce('modules/thumbnail/classes/ThumbnailHelper.class');
	global $gallery;

	list ($ret, $containerId) = $this->getParameter('id.container');
	if ($ret) {
	    return $ret;
	}
	$containerId = (int)$containerId;

	/* Also deletes ThumbnailImage entities and restores thumbnails for the affected items. */
	$ret = parent::uninstall();
	if ($ret) {
	    return $ret;
	}

	/* Remove the thumbnail container. */
	$ret = GalleryCoreApi::deleteEntityById($containerId);
	if ($ret) {
	    return $ret;
	}
	
	return null;
    }

    /**
     * @see GalleryModule::getSiteAdminViews
     */
    function getSiteAdminViews() {
	return array(null,
		     array(array('name' => $this->translate('Thumbnails'),
				 'view' => 'thumbnail.ThumbnailSiteAdmin')));
    }

    /**
     * Event handler for GalleryEntity::delete event.
     * Remove custom ThumbnailImage if the thumbnail itself is deleted.
     *
     * @see GalleryEventListener::handleEvent
     */
    function handleEvent($event) {
	$entity = $event->getEntity();
	if (GalleryUtilities::isA($entity, 'GalleryDerivativeImage') &&
		$entity->getDerivativeType() == DERIVATIVE_TYPE_IMAGE_THUMBNAIL &&
		$entity->getDerivativeSourceId() != $entity->getParentId()) {
	    list ($ret, $source) =
		GalleryCoreApi::loadEntitiesById($entity->getDerivativeSourceId());
	    if ($ret) {
		if ($ret->getErrorCode() & ERROR_MISSING_OBJECT) {
		    /* Already gone.. ok! */
		    return array(null, null);
		}
		return array($ret, null);
	    }
	    if (GalleryUtilities::isA($source, 'ThumbnailImage')) {
		/* Prevent that we try to restore the thumbnail during the deletion. */
		$key = 'ThumbnailImage::delete';
		GalleryDataCache::put($key, $entity->getId());
		$ret = GalleryCoreApi::deleteEntityById($source->getId());
		GalleryDataCache::remove($key);
		if ($ret) {
		    return array($ret, null);
		}
	    }
	}
	return array(null, null);
    }
}
?>
