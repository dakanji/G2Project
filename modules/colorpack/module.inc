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
 * The implementation of the ColorPack module
 *
 * @package ColorPack
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 18172 $
 */
class ColorPackModule extends GalleryModule {

    function ColorPackModule() {
	global $gallery;
	$this->setId('colorpack');
	$this->setName($gallery->i18n('Color Packs'));
	$this->setDescription($gallery->i18n('Select different color palettes for themes'));
	/** 
	 * Version = MajorModuleAPI.MinorModuleAPI.SerialNumberMajor.SerialNumberMinor
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
	    'ColorPackInterface_1_0', 'ColorPackImpl', 'ColorPack',
	    'modules/colorpack/classes/ColorPackImpl.class', 'colorpack', null);
	if ($ret) {
	    return $ret;
	}

	return null;
    }
}
?>
