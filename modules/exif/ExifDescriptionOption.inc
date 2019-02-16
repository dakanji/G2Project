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
 * This ItemAddOption uses the EXIF description value for the gallery item summary/description
 * and the IPTC keywords for the gallery item keywords when the image is uploaded.
 *
 * @package Exif
 * @subpackage UserInterface
 * @author Elliot Shepherd <elliot@jarofworms.com>
 * @author Georg Rehfeld <rehfeld@georg-rehfeld.de>
 * @author Jozsef R.Nagy <jozsef.rnagy@site.hu>
 * @version $Revision: 15926 $
 */
class ExifDescriptionOption extends ItemAddOption {

    /**
     * @see ItemAddOption::isAppropriate
     */
    function isAppropriate() {
	list ($ret, $addOption) = GalleryCoreApi::getPluginParameter('module', 'exif', 'addOption');
	if ($ret) {
	    return array($ret, null);
	}
	return array(null, $addOption > 0);
    }

    /**
     * @see ItemAddOption::handleRequestAfterAdd
     */
    function handleRequestAfterAdd($form, $items) {
	GalleryCoreApi::requireOnce('modules/exif/classes/ExifExtractor.class');
	GalleryCoreApi::requireOnce('modules/exif/classes/ExifHelper.class');
	$errors = $warnings = $needed = array();

	list ($ret, $addOption) =
	    GalleryCoreApi::getPluginParameter('module', 'exif', 'addOption');
	if ($ret) {
	    return array($ret, null, null);
	}

	/* Check if we need to get/process any exifdata */
	if (($addOption & EXIF_ITEM_SUMMARY) || ($addOption & EXIF_ITEM_DESCRIPTION)) {
	    $needed[] = 'IPTC/Caption';
	    $needed[] = 'ImageDescription';
	    $needed[] = 'UserComment';
	}
	if ($addOption & IPTC_ITEM_KEYWORDS) {
	    $needed[] = 'IPTC/Keywords';
	}
	if ($addOption & IPTC_ITEM_TITLE) {
	    $needed[] = 'IPTC/ObjectName';
	}
	if ($addOption & EXIF_ITEM_ROTATE) {
	    $needed[] = 'Orientation';
	}

	if (!empty($needed)) {
	    /* Copy the array because we will change it with do / while / array_splice */
	    $itemsInBatches = $items;
	    /*
	     * Batch size should be <= ulimit max open files, as long as we don't query this value,
	     * assume a value of 100 which is fairly low
	     */
	    $batchSize = 100;
	    do {
		$currentItems = array_splice($itemsInBatches, 0, $batchSize);
		for ($i = 0; $i < count($currentItems); $i++) {
		    $itemId = $currentItems[$i]->getId();

		    list ($ret, $exifData) = ExifExtractor::getMetaData(array($itemId), $needed);
		    if ($ret) {
			return array($ret, null, null);
		    }

		    $mustSave = false;

		    /*
		     * TODO(xlerb) reconsider, if ExifHelper should be changed to do the
		     * preferences.
		     */
		    $itemDescription = '';
		    if (!empty($exifData[$itemId]['IPTC/Caption']['value'])) {
			$itemDescription = $exifData[$itemId]['IPTC/Caption']['value'];
		    }
		    else if (!empty($exifData[$itemId]['ImageDescription']['value'])) {
			$itemDescription = $exifData[$itemId]['ImageDescription']['value'];
		    }
		    else if (!empty($exifData[$itemId]['UserComment']['value'])) {
			$itemDescription = $exifData[$itemId]['UserComment']['value'];
		    }

		    if (!empty($itemDescription)) {
			if ($addOption & EXIF_ITEM_SUMMARY) {
			    $currentItems[$i]->setSummary($itemDescription);
			    $mustSave = true;
			}
			if ($addOption & EXIF_ITEM_DESCRIPTION) {
			    $currentItems[$i]->setDescription($itemDescription);
			    $mustSave = true;
			}
		    }

		    if ($addOption & IPTC_ITEM_KEYWORDS
			    && !empty($exifData[$itemId]['IPTC/Keywords']['value'])) {
			$currentItems[$i]->setKeywords(
				$exifData[$itemId]['IPTC/Keywords']['value']);
			$mustSave = true;
		    }

		    if ($addOption & IPTC_ITEM_TITLE
			    && !empty($exifData[$itemId]['IPTC/ObjectName']['value'])) {
			$currentItems[$i]->setTitle($exifData[$itemId]['IPTC/ObjectName']['value']);
			$mustSave = true;
		    }

		    /* rotate item based on exifdata */
		    if ($addOption & EXIF_ITEM_ROTATE
			    && !empty($exifData[$itemId]['Orientation']['value'])) {

			$orientation = $exifData[$itemId]['Orientation']['value'];
			$args = array();

			/*
			 *  Note: Not handling the Mirrored cases this time.
			 *  Additional operation needed.
			 */

			 /*
			  //elseif ($orientation == 'Normal (O deg)') { leave $args empty }
			  else if ($orientation == 'Mirrored') {}
			  else if ($orientation == 'Upsidedown Mirrored') {}
			  else if ($orientation == '90 deg CW Mirrored') {}
			  else if ($orientation == '90 deg CCW Mirrored') {}
			 */

			if ($orientation == 'Upsidedown') {
			    $args = array(180);
			}
			else if ($orientation == '90 deg CW') {
			    $args = array(-90);
			}
			else if ($orientation == '90 deg CCW') {
			    $args = array(90);
			}

			$operation = 'rotate';

			$preserveOriginal = ($addOption & EXIF_ITEM_ROTATE_PRESERVE) ? 1 : 0;

			if (!empty($args)) {
			    list ($ret, $preferred) =
				GalleryCoreApi::fetchPreferredsByItemIds(array($itemId));
			    if ($ret) {
				return array($ret, null, null);
			    }
			    $ret = GalleryCoreApi::applyToolkitOperation(
				    $operation, $args, $preserveOriginal, $currentItems[$i],
				    empty($preferred) ? null : $preferred[$itemId]);
			    if ($ret) {
				return array($ret, null, null);
			    }
			}
		    }

		    if ($mustSave && empty($args)) {
			/**
			 * @TODO This is going to generate errors in a few cases because we're
			 * not refreshing the item after we lock it.  We really should be locking
			 * all the items at the top of this loop and releasing them at the bottom,
			 * but that will conflict with GalleryCoreApi::applyToolkitOperation above
			 * which also wants to lock the item.  Alternatively, we can buffer up all
			 * the changes we want to make to the item and then lock it here,
			 * refresh the item after locking it and then replay the changes here
			 * before saving.
			 */
			list ($ret, $lockId) = GalleryCoreApi::acquireWriteLock($itemId);
			if ($ret) {
			    return array($ret, null, null);
			}

			$ret = $currentItems[$i]->save();
			GalleryCoreApi::releaseLocks($lockId);
			if ($ret) {
			    return array($ret, null, null);
			}
		    }
		}

	    } while (!empty($itemsInBatches));
	}
	return array(null, $errors, $warnings);
    }
}
?>
