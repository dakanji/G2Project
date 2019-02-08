<?php
/**
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You can receive a copy of the GNU General Public License by writing to
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA   02110-1301, USA.
 *
 * You can also find a copy of the GNU General Public License on the
 * website of the Free Software Foundation at http://www.gnu.org
 **/

//----------------------------------------------------------------------
// *Verification*
//
//   Confirm client is verified and has called this page properly.
//   Redirect otherwise.
//
//----------------------------------------------------------------------
if (!defined('G2_SUPPORT')) {
	define('G2_SUPPORT_FILE', true);

	include_once __DIR__ . '/defaultloc.inc';
}

//----------------------------------------------------------------------
// *Main Procedural Code*
//
//   Code entry point.
//   Prepares HTML code to pass to HTML page section
//
//----------------------------------------------------------------------
$HTMLhead = $HTMLbody = null;
$HTMLbody = g2Connect();

if (!$HTMLbody) {
	define('THIS_SCRIPT', 'index.php?missingObjectFix');
	$vw       = (isset($_REQUEST['vw']) && $_REQUEST['vw'] != '') ? trim($_REQUEST['vw']) : 'opt';
	$adv      = (isset($_REQUEST['adv'])) ? true : false;
	$show     = $hide     = false;
	$tmpArray = $args = array();
	$tmpData  = (isset($_REQUEST['args']) && $_REQUEST['args'] != '') ? trim($_REQUEST['args']) : '';

	if (strlen($tmpData)) {
		$tmpArray = explode('|', $tmpData);

		foreach ($tmpArray as $pair) {
			$parts           = explode(':', $pair);
			$args[$parts[0]] = $parts[1];
		}
	}
	list($HTMLhead, $HTMLForm, $HTMLbody) = process($vw, $args);
}
GalleryEmbed::done();

//----------------------------------------------------------------------
// *Script Functions*
//
//   Functions used by this script
//
//----------------------------------------------------------------------
function g2Connect() {
	include_once '../../embed.php';
	$ret = GalleryEmbed::init(
		array(
			'fullInit' => true,
		)
	);

	if ($ret) {
		return '<div class="error center"><h2>Could not activate Gallery2 API.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';
	}

	return null;
}

function process($renderType, $args = array()) {
	global $gallery, $adv, $hide, $show, $reset;
	$storage =& $gallery->getStorage();

	$tables   = $ids   = $missingIds   = array();
	$gID      = $display      = $status      = '';
	$headData = $bodyForm = $bodyMain = null;
	$adv      = (!empty($args['adv'])) ? true : false;
	$reset    = (isset($_REQUEST['reset']) && $_REQUEST['reset'] != '') ? true : false;

	if (!$reset) {
		list($err, $rootID) = getRoot();

		if ($err) {
			$bodyMain = $err;
		} else {
			if (!empty($args['msg'])) {
				$bodyMain = statusMsg(base64_decode($args['msg']));
			}

			if (!empty($_REQUEST['gID']) && is_numeric($_REQUEST['gID'])) {
				$gID       = $_REQUEST['gID'];
				$bodyMain .= jsessAdd('pick', $gID);
			}

			if ($renderType == 'req') {
				$body     = '';
				$hide     = true;
				$pass     = (isset($args['pass'])) ? '&args=pass:' . $args['pass'] : '';
				$bodyMain = '<span class="center"><h3>Processing Request...</h3></span>' . $bodyMain;
				$url      = THIS_SCRIPT . '&vw=' . $args['mode'] . $pass . '&gID=' . $gID;
				$headData = '<meta http-equiv="refresh" content="0; URL=' . $url . '">' . "\n";
			} elseif ($renderType == 'entChk') {
				$bodyMain = statusMsg();
				$adv      = true;

				if (!$err) {
					list($err, $ids) = getItemIdsRecursive($gID);

					if (!$err) {
						$tables  = array(
							'AccessSubscriberMap' => 'itemId',
							'AlbumItem'           => 'id',
							'AnimationItem'       => 'id',
							'ChildEntity'         => 'id',
							'ChildEntity'         => 'parentId',
							'DataItem'            => 'id',
							'Derivative'          => 'derivativeSourceId',
							'DerivativePrefsMap'  => 'itemId',
							'DescendentCountsMap' => 'itemId',
							'Entity'              => 'id',
							'FileSystemEntity'    => 'id',
							'ImageBlockCacheMap'  => 'itemId',
							'Item'                => 'id',
							'ItemAttributesMap'   => 'itemId',
							'LinkItem'            => 'id',
							'MovieItem'           => 'id',
							'PhotoItem'           => 'id',
							'UnknownItem'         => 'id',
							'User'                => 'id',
						);
						$display = (count($ids)) ? '<p>Total Missing entities: <strong>' . count($ids) . '</strong></p>' : '';

						foreach ($ids as $id) {
							foreach ($tables as $table => $field) {
								$sql = 'DELETE FROM [' . $table . '] WHERE [::' . $field . '] = ?';
								$ret = $storage->execute($sql, array($id));

								if (!$ret) {
									list($ret, $rows) = $storage->getAffectedRows();
									$display         .= '<p>Fixed: ' . $rows . 'object(s)</p>';
								}
							}
						}

						if ($display != '') {
							$bodyMain .= statusMsg($display);
							$url       = THIS_SCRIPT . '&args=msg:' . urlencode(base64_encode('<div class="warning center">Some missing entities were found and fixed.</div>')) . '|adv:true';
							$headData  = '<meta http-equiv="refresh" content="0; URL=' . $url . '">' . "\n";
						} else {
							$url      = THIS_SCRIPT . '&args=msg:' . urlencode(base64_encode('<div class="success center">No missing entities were found.</div>')) . '|adv:true';
							$headData = '<meta http-equiv="refresh" content="0; URL=' . $url . '">' . "\n";
						}
					} else {
						$adv      = false;
						$bodyMain = $err;
					}
				} else {
					$adv      = false;
					$bodyMain = $err;
				}
			} elseif ($renderType == 'resChk') {
				// Get Ids of all missing derivatives under the album tree
				list($err, $DerivativeIds) = getMissingDerivatives($gID);

				if ($err) {
					$bodyMain = $err;
				} else {
					// Break this up into batches
					$batch  = (isset($_REQUEST['batchSize']) && $_REQUEST['batchSize'] != '') ? $_REQUEST['batchSize'] : 500;
					$total  = count($DerivativeIds);
					$passes = ($total / $batch > 1) ? ceil($total / $batch) : 0;

					if ($passes) {
						$pass  = (isset($args['pass'])) ? (int)$args['pass'] : $passes;
						$start = (($pass * $batch) - $batch > 0) ? ($pass * $batch) - $batch : 1;
						$end   = ($pass * $batch < $total) ? $pass * $batch : $total;

						if ($pass == $passes) {
							$end = $end + 1;
						} else {
							$end = $end - 1;
						}

						for ($i = $start; $i < $end; $i++) {
							$missingIds[] = $DerivativeIds[$i];
						}
					}

					// Delete database references to missing derivatives in this batch
					$tables   = array(
						'ChildEntity'     => 'id',
						'Derivative'      => 'id',
						'DerivativeImage' => 'id',
						'Entity'          => 'id',
					);
					$display .= (count($missingIds)) ? '<p>Total Missing Derivatives: <strong>' . count($missingIds) . '</strong></p>' : '';
					$status   = ($pass) ? 'Working on derivatives ' . $end . ' to ' . $start : '';

					foreach ($missingIds as $id) {
						foreach ($tables as $table => $field) {
							$sql = 'DELETE FROM [' . $table . '] WHERE [::' . $field . '] = ?';
							$ret = $storage->execute($sql, array($id));

							if (!$ret) {
								list($ret, $rows) = $storage->getAffectedRows();
								$display         .= '<p>Fixed: ' . $rows . 'derivatives(s)</p>';
							}
						}
					}

					// Handle page display and looping
					if (($display != '' || $status != '') && $pass > 0) {
						$bodyMain .= ($display != '') ? statusMsg($display) : '';
						$pass--;
						$url      = THIS_SCRIPT . '&args=msg:' . urlencode(base64_encode('<div class="info center">' . $status . '</div>')) . '|mode:resChk|pass:' . $pass . '&gID=' . $gID . '&vw=req';
						$headData = '<meta http-equiv="refresh" content="0; URL=' . $url . '">' . "\n";
					} elseif ($display != '') {
						$bodyMain .= statusMsg($display);
						$url       = THIS_SCRIPT . '&args=msg:' . urlencode(base64_encode('<div class="warning center">Some missing derivatives were found and fixed.</div>')) . '&gID=' . $gID;
						$headData  = '<meta http-equiv="refresh" content="0; URL=' . $url . '">' . "\n";
					} else {
						$url      = THIS_SCRIPT . '&args=msg:' . urlencode(base64_encode('<div class="success center">No missing derivatives were found.</div>')) . '&gID=' . $gID;
						$headData = '<meta http-equiv="refresh" content="0; URL=' . $url . '">' . "\n";
					}
				}
			}
		}
	}
	// Build form to show on page
	$bodyForm = "\n" . '<fieldset><br>' . "\n";

	if (!$err) {
		list($err, $albumList) = getAlbumSelector($gID);

		if (!$adv) {
			if (!$err) {
				$bodyForm .= $albumList;
				$bodyForm .= '<p class="description">STEP 1: Choose entity search start album</p>' . "\n";
				$bodyForm .= '<form action="' . THIS_SCRIPT . '" id="entitiesForm" method="post">' . "\n";
				$bodyForm .= '	<select id="gID" name="gID"></select>' . "\n";
				$bodyForm .= '	<input type="hidden" id="vw" name="vw" value="req">' . "\n";
				$bodyForm .= '	<input type="hidden" id="args" name="args" value="mode:entChk">' . "\n";
				$bodyForm .= '	<input type="hidden" id="adv" name="adv" value="true"><br>' . "\n";
				$bodyForm .= '	<input class="neutralbtn continue" type="submit" value="Check Entities" onclick="this.disabled=true;this.form.submit();">' . "\n";
				$bodyForm .= '</form>' . "\n";
			} else {
				$bodyMain = $err;
			}
		} elseif (!$hide) {
			$show      = true;
			$bodyForm .= '<p class="description">STEP 2: Choose derivative search batch size</p>' . "\n";
			$bodyForm .= '<form action="' . THIS_SCRIPT . '" id="derivativesForm" method="post">' . "\n";
			$bodyForm .= '</form>' . "\n";
		}
	}
	$bodyForm .= '<form action="' . THIS_SCRIPT . '" id="resetForm" method="post">' . "\n";
	$bodyForm .= '	<input type="hidden" id="reset" name="reset" value="true">' . "\n";
	$bodyForm .= '	<input class="neutralbtn continue" type="submit" value="Reset" onclick="this.disabled=true;this.form.submit();">' . "\n";
	$bodyForm .= '</form>' . "\n";
	$bodyForm .= '</fieldset>' . "\n";

	if (isset($bodyMain)) {
		$bodyForm = $bodyForm . '<br><hr class="faint">' . "\n";
	} else {
		if ($reset) {
			$bodyMain = statusMsg();
		} else {
			$bodyForm = $bodyForm . '<br>' . "\n";
		}
	}

	return array($headData, $bodyForm, $bodyMain);
}

function getAlbumIdsRecursive($id) {
	$err = $albumIds = null;
	// Get ids of all albums from starting point $id.
	list($ret, $tree) = GalleryCoreApi::fetchAlbumTree($id, null, null, true);

	if ($ret) {
		$err = '<div class="error center"><h2>Could not load tree.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';
	} else {
		// Load all the albumIds
		$albumIds = GalleryUtilities::arrayKeysRecursive($tree);
		// Add our starting point
		array_push($albumIds, $id);
	}

	return array($err, $albumIds);
}

function getItemIdsRecursive($id) {
	$err                  = null;
	$albums               = array();
	$itemIds              = array();
	$missingIds           = array();
	list($err, $albumIds) = getAlbumIdsRecursive($id);

	if (!$err) {
		foreach ($albumIds as $albumId) {
			list($ret, $album) = GalleryCoreApi::loadEntitiesById($albumId, 'GalleryAlbumItem');

			if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
				$missingIds[] = $albumId;
			}
			list($ret, $childIds) = GalleryCoreApi::fetchChildItemIdsIgnorePermissions($album);

			if ($ret) {
				$err = '<div class="error center"><h2>Could not fetch child item ids for Album: ' . $albumId . '.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';

				break;
			}
			$itemIds = array_merge($itemIds, $childIds);
		}

		if (!$err) {
			foreach ($itemIds as $id) {
				list($ret, $item) = GalleryCoreApi::loadEntitiesById($id, 'GalleryItem');

				if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
					$missingIds[] = $id;

					continue;
				}
				list($ret, $path) = $item->fetchPath();

				if ($ret || (!file_exists($path) && !is_dir($path))) {
					$missingIds[] = $id;
				}
			}
		}
	}

	return array($err, $missingIds);
}

function getMissingDerivatives($id) {
	$err                  = null;
	$itemIds              = $missingIds              = $albums              = array();
	list($err, $albumIds) = getAlbumIdsRecursive($id);

	if (!$err) {
		foreach ($albumIds as $albumId) {
			list($ret, $album) = GalleryCoreApi::loadEntitiesById($albumId, 'GalleryAlbumItem');

			if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
				$missingIds[] = $albumId;
			}
			list($ret, $childIds) = GalleryCoreApi::fetchChildItemIdsIgnorePermissions($album);

			if ($ret) {
				$err = '<div class="error center"><h2>Could not fetch child item ids for Album: ' . $albumId . '.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';

				break;
			}
			$itemIds = array_merge($itemIds, $childIds);
		}

		if (!$err) {
			foreach ($itemIds as $id) {
				list($ret, $item) = GalleryCoreApi::loadEntitiesById($id, 'GalleryDerivative');

				if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
					$missingIds[] = $id;

					continue;
				}
				list($ret, $path) = $item->fetchPath();

				if ($ret || (!file_exists($path) && !is_dir($path))) {
					$missingIds[] = $id;
				}
			}
		}
	}

	return array($err, $missingIds);
}

function getRoot() {
	$err = $defaultId = null;

	if (GalleryUtilities::isCompatibleWithApi(array(7, 5), GalleryCoreApi::getApiVersion())) {
		list($ret, $defaultId) = GalleryCoreApi::getDefaultAlbumId();

		if ($ret) {
			$err = '<div class="error center"><h2>Could not locate gallery root album.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';
		}
	} else {
		list($ret, $defaultId) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.rootAlbum');

		if ($ret) {
			$err = '<div class="error center"><h2>Could not locate gallery root album.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';
		}
	}

	return array($err, $defaultId);
}

function getAlbumSelector($gID) {
	$err                = $albumSelectorCode                = $albumSelector                = null;
	$albums             = array();
	list($err, $rootID) = getRoot();

	if (!$err) {
		$gID                = (isset($gID) && $gID != '') ? $gID : $rootID;
		list($err, $albums) = getAlbumTree($gID);

		if (!$err) {
			$albumSelector = '<option value=' . $gID . '>Gallery Root</option>';

			foreach ($albums as $album) {
				$albumSelector .= '<option value=' . $album->getId() . '>' . $album->getTitle() . '</option>';
			}
		}
		$albumSelectorCode = "\n" . jsessAdd('select', $albumSelector) . "\n" . '<script type="text/javascript">window.onload=jsAlbumSelect;</script>' . "\n";
	}

	return array($err, $albumSelectorCode);
}

function getAlbumTree($gID) {
	$err              = $ret              = $album              = null;
	$albumIds         = $albums         = array();
	list($ret, $tree) = GalleryCoreApi::fetchAlbumTree($gID, null, null, true);

	if ($ret) {
		$err = '<div class="error center"><h2>Could not create album selector.</h2></div><div class="error left">' . $ret->getAsHtml() . '</div>';
	} else {
		$albumIds = GalleryUtilities::arrayKeysRecursive($tree);

		foreach ($albumIds as $albumId) {
			list($ret, $album) = GalleryCoreApi::loadEntitiesById($albumId, 'GalleryAlbumItem');

			if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
				continue;
			}
			$albums[] = $album;
		}
	}

	return array($err, $albums);
}

function statusMsg($display = false) {
	if ($display !== false) {
		return "\n" . '
			<script type="text/javascript">
				if (typeof(jsess.missingObjectFix_tgtHTML) == "undefined") {
					jsess.missingObjectFix_tgtHTML = \'' . $display . '\';
				} else {
					jsess.missingObjectFix_tgtHTML = \'' . $display . '\' + jsess.missingObjectFix_tgtHTML;
    			}
    		</script>
    	' . "\n";
	}

	return "\n" . '<script type="text/javascript">jsess.missingObjectFix_tgtHTML="";</script>' . "\n";
}

function jsessAdd($tag = 'dummy', $msg = 'dummy') {
	return '<script type="text/javascript">jsess.missingObjectFix_' . $tag . '="' . $msg . '";</script>';
}

//----------------------------------------------------------------------
// *Page HTML*
//
//   Actual HTML to insert into index.php for display
//
//----------------------------------------------------------------------
?>
<html lang="en">
	<head>
		<title>Gallery Support | Missing Object Error Fix</title>
		<link rel="stylesheet" type="text/css" href="support.css">
		<script type="text/javascript" src="../javascript/jsess.min.js"></script>
		<script type="text/javascript">
			function changeContent(id,content) {
				if (document.getElementById(id) != null) {
					var node = document.getElementById(id);
					node.innerHTML = content;
				}
			}
			function jsAlbumSelect() {
				if (typeof(jsess.missingObjectFix_select) !== "undefined") {
					var selectStr = jsess.missingObjectFix_select;
				}

				if (typeof(jsess.missingObjectFix_pick) !== "undefined") {
				<?php
				if ($reset) {
					?>
					var oldStr = 'selected value=' + jsess.missingObjectFix_pick + '>';
					var newStr = 'value=' + jsess.missingObjectFix_pick + '>';
					<?php
				} else {
					?>
					var oldStr = 'value=' + jsess.missingObjectFix_pick + '>';
					var newStr = 'selected value=' + jsess.missingObjectFix_pick + '>';
					<?php
				}
				?>

					var selectStr = selectStr.replace(oldStr, newStr);
				}
				changeContent("gID", selectStr);
			}
		</script>
		<?php
		if ($HTMLhead) {
			echo $HTMLhead;
		}
		?>

	</head>
	<body>
		<div id="content">
		<?php
		if ($HTMLbody || $HTMLForm) {
			?>
			<div id="title">
				<a href="../../">Gallery</a> &raquo;
				<a href="index.php">Support</a> &raquo; Missing Object Error Fix
			</div>
			<div class="center">
				<p class="description">Run to recover from "ERROR_MISSING_OBJECT" error messages.</p>
				<noscript>
					<h2>This Tool Requires Javascript</h2>
					<p class="description">Please enable javascript to use this tool.</p>
				</noscript>
				<p><?php echo $HTMLForm; ?></p>
				<p id="tPara"></p>
				<p><?php echo $HTMLbody; ?></p>
			</div>
			<?php
		} else {
			?>
			<div class="center">
				<fieldset>
					<br><p class="description">
						No Output = Script Failure
					</p><br>
				</fieldset>
			</div>
			<?php
		}
		?>

		</div>
		<?php
		if ($show) {
			?>
		<script type="text/javascript">
			var formData = '';
			if (typeof(jsess.missingObjectFix_pick) !== "undefined") {
				formData = formData + '	<input type="hidden" id="gID" name="gID" value="' + jsess.missingObjectFix_pick + '">';
			}
			formData = formData + '	<input type="hidden" id="vw" name="vw" value="req">';
			formData = formData + '	<input type="hidden" id="args" name="args" value="mode:resChk">';
			formData = formData + '	<select id="batchSize" name="batchSize">';
			formData = formData + '		<option value=125>125 Items</option>';
			formData = formData + '		<option value=250>250 Items</option>';
			formData = formData + '		<option selected value=500>500 Items</option>';
			formData = formData + '		<option value=1000>1000 Items</option>';
			formData = formData + '		<option value=2000>2000 Items</option>';
			formData = formData + '	</select><br>';
			formData = formData + '	<input class="neutralbtn continue" type="submit" value="Check Derivatives" onclick="this.disabled=true;this.form.submit();">';
			changeContent("derivativesForm", formData);
		</script>
			<?php
		}
		?>

		<script type="text/javascript">
			if (typeof(jsess.missingObjectFix_tgtHTML) !== "undefined") {
				changeContent('tPara', jsess.missingObjectFix_tgtHTML);
			}
		</script>
	</body>
</html>
