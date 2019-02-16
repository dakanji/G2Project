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
ini_set('error_reporting', 2047);
if (!empty($_SERVER['SERVER_NAME'])) {
    print "You must run this from the command line\n";
    exit(1);
}

if (!file_exists('modules')) {
    print "You must run this from the gallery2 directory\n";
    exit(1);
}

$author = '';
$authorFullName = '';
if (function_exists('posix_getlogin')) {
    $author = posix_getlogin();
    $tmp = posix_getpwnam($author);
    if (!empty($tmp['gecos'])) {
	$authorFullName = $tmp['gecos'];
    }
}

require_once(dirname(__FILE__) . '/../../../lib/smarty/Smarty.class.php');
if (!empty($_ENV['TMP'])) {
    $tmpdir = $_ENV['TMP'];
} else {
    $tmpdir = '/tmp';
}
$tmpdir .= "/g2_" . rand(1, 30000);
if (file_exists($tmpdir)) {
    print "Tmp dir already exists: $tmpdir\n";
    exit(1);
}

if (!mkdir($tmpdir)) {
    print "Unable to make tmp dir: $tmpdir\n";
    exit(1);
}

$smarty = new Smarty();
$smarty->compile_dir = $tmpdir;
$smarty->error_reporting = error_reporting();
$smarty->debugging = true;
$smarty->use_sub_dirs = false;
$smarty->template_dir = dirname(__FILE__);

/*
 * Gather any info we need from the user
 */
if (!empty($author)) {
    $defaultModuleName = 'Hello ' . ucfirst($author);
} else {
    $defaultModuleName = 'Hello World';
}

while (empty($moduleName)) {
    $moduleName = ask('What is the name of your module?', $defaultModuleName);
}

while (empty($moduleId)) {
    $moduleId = ask('What is the id of your module?',
		    strtolower(preg_replace('/ /', '', $moduleName)));
}
$moduleId = preg_replace('/\W/', '', $moduleId);
$ucModuleId = ucfirst($moduleId);

$smarty->assign('moduleId', $moduleId);
$smarty->assign('ucModuleId', $ucModuleId);
$smarty->assign('moduleName', $moduleName);
$smarty->assign('author', $author);
$smarty->assign('authorFullName', $authorFullName);
$smarty->assign('viewName', $ucModuleId);
$smarty->assign('mapName', $ucModuleId . "Map");

/*
 * Start building things!
 */

/* Make the module directory */
$modulePath = 'modules/' . $moduleId;
if (file_exists($modulePath)) {
    error("$modulePath already exists!");
} else {
    mkdir($modulePath) || error("Can't mkdir($modulePath)");
}

/* Create module.inc */
$fd = safe_fopen("$modulePath/module.inc");
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/module.inc.tpl'));
fclose($fd);

/* Create our sample view and template */
$fd = safe_fopen("$modulePath/$ucModuleId.inc");
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/MyPage.inc.tpl'));
fclose($fd);

mkdir("$modulePath/templates");
$fd = safe_fopen("$modulePath/templates/$ucModuleId.tpl");
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/MyPage.tpl.tpl'));
fclose($fd);

/*
 * Create our map
 */
mkdir($modulePath . '/classes');
mkdir($modulePath . '/classes/GalleryStorage');

$smarty->assign('makefileType', 'classes');
$fd = safe_fopen("$modulePath/classes/GNUmakefile");
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/GNUmakefile.tpl'));
fclose($fd);

$smarty->assign('makefileType', 'GalleryStorage');
$fd = safe_fopen("$modulePath/classes/GalleryStorage/GNUmakefile");
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/GNUmakefile.tpl'));
fclose($fd);

$fd = safe_fopen("$modulePath/classes/Maps.xml");
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/map.tpl'));
fclose($fd);

$fd = safe_fopen($modulePath . '/classes/' . $ucModuleId . 'Helper.class');
fwrite($fd, $smarty->fetch(dirname(__FILE__) . '/MyPageHelper.class.tpl'));
fclose($fd);

print "* * * * * * * * * * * * * * * * * * * * * * * * * *\n";
print "Your module is ready!  You must build it by doing: \n";
print "\n";
print "  cd modules/$moduleId/classes \n";
print "  make && make clean\n";
print "\n";
print "Then you can go to the Site Admin -> Modules \n";
print "page and install and activate your module!\n";
print "* * * * * * * * * * * * * * * * * * * * * * * * * *\n";

function ask($prompt, $default='') {
    print $prompt;
    if (!empty($default)) {
	print " [$default]";
    }
    print ' ';
    $line = trim(fgets(STDIN));
    if (empty($line)) {
	return $default;
    }
    return $line;
}

function error($message) {
    fwrite(STDERR, "$message\n");
    fwrite(STDERR, "*** Exiting!\n");
    cleanup();
    exit(1);
}

function cleanup() {
    global $tmpdir;
    if (file_exists($tmpdir)) {
	system("rm -rf $tmpdir");
    }
}

function safe_fopen($path) {
    ($fd = fopen($path, 'wb')) || error("Can't write to $path");
    return $fd;
}