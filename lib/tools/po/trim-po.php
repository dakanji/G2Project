<?php
/**
 * Usage: php trim-po.php xx_YY.po
 * Prints a copy of xx_YY.po, omitting all translations that match xx.po.
 * If not of form xx_YY.po or xx.po does not exist then trim any translations
 * where msgid == msgstr (applies mainly to en_*).
 * Both cases also print warnings for any translation hints that are not
 * handled in this translation (and will appear in the application).
 */
$path = $argv[1];
if (preg_match('#^/cygdrive/(\w+)/(.*)$#', trim($path), $matches)) {
    /* Cygwin and Window PHP filesystem function don't play nice together. */
    $path = $matches[1] . ':\\' . str_replace('/', '\\', $matches[2]);
}

$langpath = preg_replace('{(..)_..\.po$}', '$1.po', $path);

if ($langpath == $path || !file_exists($langpath)) {
    if ($langpath != $path && !in_array(basename($langpath), array('en.po', 'zh.po'))) {
	fwrite(stdErr(), "\nWarning: $path without $langpath\n");
    }
    list ($po, $header) = readPo($path);
    print $header;
    foreach ($po as $id => $data) {
	checkHint($id, $data['msgstr'], $path);
	if (substr($id, 5) != substr($data['msgstr'], 6)) {
	    print $data['before'] . $id . $data['msgstr'] . "\n";
	}
    }
    exit;
}

list ($po, $header) = readPo($path);
list ($langpo) = readPo($langpath);

print $header;
foreach ($po as $id => $data) {
    checkHint($id, $data['msgstr'], $path);
    if (!isset($langpo[$id]) || $langpo[$id]['msgstr'] != $data['msgstr']) {
	print $data['before'] . $id . $data['msgstr'] . "\n";
    }
}

function checkHint($msgid, $msgstr, $path) {
    if (strpos($msgid, '<!--') !== false && $msgstr == "msgstr \"\"\n") {
	fwrite(stdErr(), "\nWarning: Unhandled translator hint in $path\n");
    }
    if (strpos($msgstr, '<!--') !== false) {
	fwrite(stdErr(), "\nWarning: Translation contains hint in $path\n");
    }
}

function readPo($path) {
    $header = $data = array();
    $lines = file($path);
    for ($line = 'a'; $lines && trim($line); $header[] = $line) {
	$line = array_shift($lines);
    }
    $id = $str = false;
    $key = $value = $before = '';
    while ($lines) {
	$line = array_shift($lines);
	if (!$id && substr($line, 0, 5) == 'msgid') {
	    $id = true;
	} else if ($id && substr($line, 0, 6) == 'msgstr') {
	    $str = true;
	} else if ($id && $str && !trim($line)) {
	    $data[$key] = array('msgstr' => $value, 'before' => $before);
	    $id = $str = false;
	    $key = $value = $before = '';
	    continue;
	}
	if ($str) {
	    $value .= $line;
	} else if ($id) {
	    $key .= $line;
	} else {
	    $before .= $line;
	}
    }
    if ($key && $value) {
	$data[$key] = array('msgstr' => $value, 'before' => $before);
    }
    return array($data, implode('', $header));
}

function stdErr() {
    static $stdErr;
    if (!defined('STDERR')) {
	/* Already defined for CLI but not for CGI */
	$stdErr = fopen('php://stderr', 'w');
	define('STDERR', $stdErr);
    }
    return STDERR;
}
?>
