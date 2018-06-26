<?php
lt_include(PLOG_CLASS_PATH . 'class/data/utf8/utf8_funcs.php');

/**
 * Smarty truncate modifier plugin
 *
 * Type:     modifier<br>
 * Name:     utf8_truncate<br>
 * @param string
 * @param integer
 * @param string
 * @param boolean
 * @return string
 */
function smarty_modifier_utf8_truncate(
	$string,
	$length = 80,
	$etc = '...',
	$break_words = false
) {
	if ($length == 0) {
		return '';
	}

	if (utf8_strlen($string) > $length) {
		$length -= strlen($etc);

		if (!$break_words) {
			$string = preg_replace('/\s+?(\w+)?$/', '', utf8_substr($string, 0, $length + 1));
		}

		return utf8_substr($string, 0, $length) . $etc;
	}

	return $string;
}

// vim: set expandtab:
