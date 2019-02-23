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
 * Implementation of the Share module
 *
 * @package Share
 * @author Karl Newman
 * @version $Revision: 1910 $
 */
class ShareModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('share');
		$this->setName($gallery->i18n('Share'));
		$this->setDescription(
			$gallery->i18n(
				'Share link to Gallery pictures on social media sites'
			)
		);

		$this->setVersion('2.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('export', $gallery->i18n('Export'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new ShareModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Share'),
					'view' => 'share.ShareSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion, $x1 = null) {
		switch ($currentVersion) {
			case null:
				// Initial install

				foreach (array(
					'onlyWhenLoggedIn' => false,
					'addThisAccountId' => 'addThisAccountId',
				) as $key => $value) {
					$ret = $this->setParameter($key, $value);

					if ($ret) {
						return $ret;
					}
				}
				// Fall Through
			case '1.0.0':
			case '1.0.1':
				// Fixed typo in block template
			case '2.0.0':
				// Updated GalleryCoreApi requirement to 8.0
				// Upgraded GalleryModule requirement to 4.0
			case 'end of upgrade path':
				/*
				 * Leave this bogus case at the end of the legitimate case statements
				 * so that we always properly terminate our upgrade path with a break.
				 */
				break;

			default:
				return GalleryCoreApi::error(
					ERROR_BAD_PLUGIN,
					__FILE__,
					__LINE__,
					sprintf('Unknown module version %s', $currentVersion)
				);
		}

		return null;
	}
}
