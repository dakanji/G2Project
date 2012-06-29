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
 * The implementation of the Image Frame module
 *
 * @package ImageFrame
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 17580 $
 */
class ImageFrameModule extends GalleryModule {

    function ImageFrameModule() {
	global $gallery;
	$this->setId('imageframe');
	$this->setName($gallery->i18n('ImageFrame'));
	$this->setDescription($gallery->i18n('Render frames around images'));
	/** 
	 * Version = MajorModuleAPI.MinorModuleAPI.SerialNumberMajor.SerialNumberMinor
	 * Update upgrade() function below too
	**/
	$this->setVersion('4.0.0.0');
	$this->setRequiredModuleApi(array(4, 0));
	$this->setRequiredCoreApi(array(8, 0));
	$this->setTemplateVersion(1);
	$this->setGroup('display', $gallery->i18n('Display'));
	$this->setCallbacks('');
    }

    /**
     * @see GalleryModule::performFactoryRegistrations
     */
    function performFactoryRegistrations() {
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ImageFrameInterface_1_1', 'ImageFrameImpl', 'ImageFrame',
	    'modules/imageframe/classes/ImageFrameImpl.class', 'imageframe', null);
	if ($ret) {
	    return $ret;
	}

	return null;
    }
}
?>
