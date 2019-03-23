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
 * The implementation of the Captcha module
 *
 * @package Captcha
 * @author Stefan Ioachim <stefanioachim@gmail.com>
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 18172 $
 */
class CaptchaModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('captcha');
		$this->setName($gallery->i18n('Captcha'));
		$this->setDescription(
			$gallery->i18n(
				'Prevents abuse by deterring automated bots with ' .
				'input that requires visual comprehension'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new CaptchaModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryValidationPlugin',
			'CaptchaValidationPlugin',
			'CaptchaValidationPlugin',
			'modules/captcha/classes/CaptchaValidationPlugin.inc',
			'captcha',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'ResetFailureCountsTask',
			'ResetFailureCountsTask',
			'modules/captcha/classes/ResetFailureCountsTask.class',
			'captcha',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion, $x1 = null) {
		switch ($currentVersion) {
			case '':
				// Set some reasonable defaults
				$ret = $this->setParameter('failedAttemptThreshold', 3);

				if ($ret) {
					return $ret;
				}

				break;

			case '0.9.0':
				// Captcha changed from LoginPlugin to ValidationPlugin to add registration support
			case '0.9.1':
				// Updated GalleryCoreApi requirement to 5.3 to use Gallery::getPhpVm()
			case '0.9.2':
				// Updated GalleryCoreApi requirement to 6.0
			case '0.9.3':
			case '0.9.4':
				// Upgraded GalleryModule requirement to 1.0
			case '0.9.5':
				// Upgraded GalleryModule requirement to 2.0
			case '0.9.6':
			case '1.0.0':
				// Renamed ValidationPlugin to GalleryValidationPlugin
			case '1.0.1':
				// GalleryCoreApi 7.0 and GalleryModule 3.0
			case '1.0.2':
				// Updated GalleryValidationPlugin API
			case '1.1.0':
			case '1.1.1':
			case '1.1.2':
			case '1.1.3':
			case '1.1.4':
			case '1.1.4.1':
				// .mo file migration
			case '1.1.5':
			case '1.1.6':
			case '1.1.7':
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

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We need to perform a test before activating
		list($ret, $needsConfiguration) = $this->needsConfiguration();

		if ($ret) {
			return array($ret, null);
		}

		return array(null, !$needsConfiguration);
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		GalleryCoreApi::requireOnce('modules/captcha/classes/CaptchaHelper.class');

		$gdReport = CaptchaHelper::testRequiredGdFunctions();

		return array(null, count($gdReport['fail']) > 0);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Captcha'),
					'view' => 'captcha.CaptchaSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'captcha.CaptchaConfigAdmin';
	}
}
