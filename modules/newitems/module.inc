<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

/**
 * Add new/updated image in summary info for new/updated items.
 * New sort option to put new items in front, follwed by another sort.
 * Thanks to Felix Rabinovich (virshu) for the idea.
 *
 * @package NewItems
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 18172 $
 */
class NewItemsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('newitems');
		$this->setName($gallery->i18n('New Items'));
		$this->setDescription(
			$gallery->i18n(
				'Highlight new/updated Gallery items'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getSiteAdminViews|getItemSummaries');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new NewItemsModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion, $x1 = null) {
		if (!isset($currentVersion)) {
			foreach (array(
				'days.new'     => '7',
				'days.updated' => '7',
			) as $key => $value) {
				$ret = $this->setParameter($key, $value);

				if ($ret) {
					return $ret;
				}
			}
		}

		return null;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GallerySortInterface_1_2',
			'NewItemsSortOrder',
			'NewItems',
			'modules/newitems/classes/NewItemsSortOrder.class',
			'newitems',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('New Items'),
					'view' => 'newitems.NewItemsSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getItemSummaries
	 */
	public function getItemSummaries($items, $permissions, &$template) {
		list($ret, $param) = GalleryCoreApi::fetchAllPluginParameters('module', 'newitems');

		if ($ret) {
			return array($ret, null);
		}

		$time          = time();
		$newTime       = $time - $param['days.new']     * 86400;
		$updatedTime   = $time - $param['days.updated'] * 86400;
		$newString     = $this->translate('New');
		$updatedString = $this->translate('Updated');
		$summaries     = array();

		foreach ($items as $item) {
			if ($param['days.new'] > 0 && $item->getCreationTimestamp() > $newTime) {
				$summaries[$item->getId()] = '<span class="giNew">'
				. $newString . '</span>';
			} elseif ($param['days.updated'] > 0
				&& $item->getModificationTimestamp() > $updatedTime
			) {
				$summaries[$item->getId()] = '<span class="giUpdated">'
				. $updatedString . '</span>';
			}
		}

		return array(null, $summaries);
	}

	/**
	 * @see GalleryModule::uninstall
	 */
	public function uninstall() {
		$ret = GalleryCoreApi::deleteSortOrder('NewItems');

		if ($ret) {
			return $ret;
		}

		return parent::uninstall();
	}
}
