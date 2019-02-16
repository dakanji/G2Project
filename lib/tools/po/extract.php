#!/usr/bin/php -f
<?php
/*
 * PHP script to extract strings from all the files and print
 * to stdout for use with xgettext.
 *
 * This script is based on the perl script provided with the Horde project
 * http://www.horde.org/.  As such, it inherits the license from the
 * original version.  You can find that license here:
 *
 * http://cvs.horde.org/co.php/horde/COPYING?r=2.1
 *
 * I'm not exactly sure what the license restrictions are in this case,
 * but I want to give full credit to the original authors:
 *
 * Copyright 2000-2002 Joris Braakman <jbraakman@yahoo.com>
 * Copyright 2001-2002 Chuck Hagenbuch <chuck@horde.org>
 * Copyright 2001-2002 Jan Schneider <jan@horde.org>
 *
 * We've modified the script somewhat to make it work cleanly with the
 * way that Gallery embeds internationalized text, so let's tack on our
 * own copyrights.
 *
 * Copyright 2002-2007 Bharat Mediratta <bharat@menalto.com>
 *
 * $Id: extract.php 15513 2006-12-20 00:24:57Z mindless $
 */

if (!empty($_SERVER['SERVER_NAME'])) {
    print "You must run this from the command line\n";
    exit(1);
}

$exts = '(class|php|inc|tpl|css|html)';
/* These are in phpdoc and don't really need translations: */
$skip = array('TEST to be displayed in different languages' => true,
	      'TT <!-- abbreviation for Translation Test -->' => true);
$idEmitted = false;
$strings = array();
array_shift($_SERVER['argv']);
foreach ($_SERVER['argv'] as $moduleDir) {
    if (preg_match('#^/cygdrive/(\w+)/(.*)$#', trim($moduleDir), $matches)) {
	/* Cygwin and Window PHP filesystem function don't play nice together. */
	$moduleDir = $matches[1] . ':\\' . str_replace('/', '\\', $matches[2]);
    }
    if (!is_dir($moduleDir)) {
	continue;
    }
    chdir($moduleDir);
    find('.');

    $oldStringsRaw = "$moduleDir/po/strings.raw";
    if (file_exists($oldStringsRaw)) {
	$lines = file($oldStringsRaw);
	if (preg_match('/^#.*Id/', $lines[0])) {
	    print $lines[0];
	    $idEmitted = true;
	}
    }
}

if (!$idEmitted) {
    print '# $' . 'Id$' . "\n";
}
foreach ($strings as $string => $otherFiles) {
    print $string;
    if (!empty($otherFiles)) {
	print ' /* also in: ' . implode(' ', $otherFiles) . ' */';
    }
    print "\n";
}

/**
 * Recursive go through subdirectories
 */
function find($dir) {
    if ($dh = opendir($dir)) {
	$listing = $subdirs = array();
	while (($file = readdir($dh)) !== false) {
	    if ($file == '.' || $file == '..') {
		continue;
	    }
	    $listing[] = $file;
	}
	closedir($dh);
	sort($listing);
	global $exts;
	$dir = ($dir == '.') ? '' : ($dir . '/');
	foreach ($listing as $file) {
	    $filename = $dir . $file;
	    if (is_dir($filename)) {
		/* Don't parse unit tests */
		if ($file != 'test') {
		    $subdirs[] = $filename;
		}
	    } else if (preg_match("/\." . $exts . "$/", $file)) {
		extractStrings($filename);
	    }
	}
	foreach ($subdirs as $dir) {
	    find($dir);
	}
    }
}

/**
 * Grab all translatable strings in a file into $strings array
 */
function extractStrings($filename) {
    global $strings, $skip;
    $strings["\n/* $filename */"] = array();
    $startSize = count($strings);
    $localStrings = array();
    $data = file_get_contents($filename);

    /*
     * grab phrases for translate( or i18n( or _( calls; capture string parameter enclosed
     * in single or double quotes including concatenated strings like 'one' . "two"
     */
    if (preg_match_all("/(translate|i18n|_)\(\s*((?:(?:\s*\.\s*)?(?:'(?:(?:\\')?[^']*?)*[^\\\\]'|\"(?:(?:\")?[^\"]*?)*[^\\\\]\"))+)\s*(?:,\s*(true|false)\s*)?\)/s",
		       $data, $matches, PREG_SET_ORDER)) {
	foreach ($matches as $match) {
	    $text = eval('return ' . $match[2] . ';');
	    $text = str_replace('"', '\\"', $text);    /* escape double-quotes */
	    if (isset($skip[$text])) {
		continue;
	    }
	    if (!empty($match[3])) {
		$hint = '/* xgettext:' . ($match[3] == 'false' ? 'no-' : '') . "c-format */\n";
	    } else {
		$hint = '';
	    }
	    $string = $hint . sprintf('gettext("%s")', $text);
	    if (!isset($strings[$string])) {
		$strings[$string] = array();
	    } else if (!isset($localStrings[$string])) {
		$strings[$string][] = $filename;
	    }
	    $localStrings[$string] = true;
	}
    }

    /* grab phrases of this format: translate(array('one' => '...', 'many' => '...')) */
    if (preg_match_all("/translate\(\s*array\(\s*'one'\s*=>\s*((?:(?:\s*\.\s*)?(?:'(?:(?:\\')?[^']*?)*[^\\\\]'|\"(?:(?:\")?[^\"]*?)*[^\\\\]\"))+).*?'many'\s*=>\s*((?:(?:\s*\.\s*)?(?:'(?:(?:\\')?[^']*?)*[^\\\\]'|\"(?:(?:\")?[^\"]*?)*[^\\\\]\"))+)\s*[,)]/s",
		       $data, $matches, PREG_SET_ORDER)) {
	foreach ($matches as $match) {
	    $one = eval('return ' . $match[1] . ';');
	    $many = eval('return ' . $match[2] . ';');
	    $one = str_replace('"', '\\"', $one);      /* escape double-quotes */
	    $many = str_replace('"', '\\"', $many);    /* escape double-quotes */
	    $string = sprintf('ngettext("%s", "%s")', $one, $many);
	    if (!isset($strings[$string])) {
		$strings[$string] = array();
	    } else if (!isset($localStrings[$string])) {
		$strings[$string][] = $filename;
	    }
	    $localStrings[$string] = true;
	}
    }

    /* grab phrases of this format: translate(array('text' => '...', ...)) */
    if (preg_match_all("/translate\(\s*array\(\s*'text'\s*=>\s*((?:(?:\s*\.\s*)?(?:'(?:(?:\\')?[^']*?)*[^\\\\]'|\"(?:(?:\")?[^\"]*?)*[^\\\\]\"))+)\s*[,)]/s",
		       $data, $matches, PREG_SET_ORDER)) {
	foreach ($matches as $match) {
	    $text = eval('return ' . $match[1] . ';');
	    $text = str_replace('"', '\\"', $text);    /* escape double-quotes */
	    $string = sprintf('gettext("%s")', $text);
	    if (!isset($strings[$string])) {
		$strings[$string] = array();
	    } else if (!isset($localStrings[$string])) {
		$strings[$string][] = $filename;
	    }
	    $localStrings[$string] = true;
	}
    }

    /* grab phrases of this format: {g->text ..... } or {g->changeInDescendents ... } */
    if (preg_match_all("/(\{\s*g->(?:text|changeInDescendents)\s+.*?[^\\\]\})/s",
		       $data, $matches, PREG_SET_ORDER)) {
	foreach ($matches as $match) {
	    $string = $match[1];
	    $text = $one = $many = null;

	    /*
	     * Ignore translations of the form:
	     *   text=$foo
	     * as we expect those to be variables containing values that
	     * have been marked elsewhere with the i18n() function
	     */
	    if (preg_match("/text=\\$/", $string)) {
		continue;
	    }

	    /* text=..... */
	    if (preg_match("/text=\"(.*?[^\\\])\"/s", $string, $matches)) {
		$text = $matches[1];
	    } elseif (preg_match("/text='(.*?)'/s", $string, $matches)) {
		$text = $matches[1];
		$text = str_replace('"', '\\"', $text);    /* escape double-quotes */
	    }

	    /* one = ..... */
	    if (preg_match("/\s+one=\"(.*?[^\\\])\"/s", $string, $matches)) {
		$one = $matches[1];
	    } elseif (preg_match("/\s+one='(.*?)'/s", $string, $matches)) {
		$one = $matches[1];
		$one = str_replace('"', '\\"', $one);    /* escape double-quotes */
	    }

	    /* many = ..... */
	    if (preg_match("/\s+many=\"(.*?[^\\\])\"/s", $string, $matches)) {
		$many = $matches[1];
	    } elseif (preg_match("/\s+many='(.*?)'/s", $string, $matches)) {
		$many = $matches[1];
		$many = str_replace('"', '\\"', $many);    /* escape double-quotes */
	    }

	    /* c-format hint for xgettext */
	    if (preg_match('/c[Ff]ormat=(true|false)/s', $string, $matches)) {
		$hint = '/* xgettext:' . ($matches[1] == 'false' ? 'no-' : '') . "c-format */\n";
	    } else {
		$hint = '';
	    }

	    /* pick gettext() or ngettext() */
	    if ($text != null) {
		$string = $hint . sprintf('gettext("%s")', $text);
	    } else if ($one != null && $many != null) {
		$string = $hint . sprintf('ngettext("%s", "%s")', $one, $many);
	    } else {
		/* parse error */
		$stderr = fopen('php://stderr', 'w');
		$text = preg_replace("/\n/s", '\n>', $text);
		fwrite($stderr, "extract.php parse error: $filename:\n");
		fwrite($stderr, "> $text\n");
		exit(1);
	    }

	    $string = str_replace('\\}', '}', $string);    /* unescape right-curly-braces */
	    if (!isset($strings[$string])) {
		$strings[$string] = array();
	    } else if (!isset($localStrings[$string])) {
		$strings[$string][] = $filename;
	    }
	    $localStrings[$string] = true;
	}
    }
    if (count($strings) == $startSize) {
	unset($strings["\n/* $filename */"]);
    }
}
?>
