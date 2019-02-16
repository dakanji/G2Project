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
 * Access point for external application in which Gallery is embedded.
 * See modules/core/classes/GalleryEmbed.class  and
 * http://codex.gallery2.org/index.php/Gallery2:Embedding  for more details.
 *
 * @package Gallery
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 15513 $
 */

/* Define G2_EMBED = 1 to remember to generate correct URLs and return the HTML, etc. */
require_once(dirname(__FILE__) . '/modules/core/classes/GalleryDataCache.class');
GalleryDataCache::put('G2_EMBED', 1, true);
require(dirname(__FILE__) . '/main.php');
require(dirname(__FILE__) . '/modules/core/classes/GalleryEmbed.class');

/*
 * Simplify finding the path to embed.php by sending it as a HTTP header
 * Idea:
 *   In your integration setup you need to find out
 *     - the filesystem path for embed.php
 *     - the g2Uri and the embedUri.
 * You can get the embed.php path with your g2Uri by fetching
 * http://example.com/gallery2/embed.php?getEmbedPath=1 via fsockopen.
 */
$getEmbedPath = GalleryUtilities::getRequestVariablesNoPrefix('getEmbedPath');
if (!empty($getEmbedPath)){
    if (!headers_sent()) {
	/*
	 * Don't use GalleryUtilities::getRemoteHostAddress()
	 * since it checks headers that can be forged easily too
	 */
	$remotehost = GalleryUtilities::getServerVar('REMOTE_ADDR');
	$remotehost = !empty($remotehost) ? gethostbyname($remotehost) : '';
	$localhost = GalleryUtilities::getServerVar('HTTP_HOST');
	$localhost = !empty($localhost) ? gethostbyname($localhost) : '127.0.0.1';
	if (!empty($remotehost) && $remotehost == $localhost) {
	    if (defined('GALLERY_CONFIG_DIR')) {
		/* GALLERY_CONFIG_DIR is multisite-aware */
		header('X-G2-EMBED-PATH: ' . GALLERY_CONFIG_DIR . '/embed.php');
	    } else {
		/* Fallback if G2 isn't installed yet */
		header('X-G2-EMBED-PATH: ' . __FILE__ );
	    }
	}
    }
}
?>
