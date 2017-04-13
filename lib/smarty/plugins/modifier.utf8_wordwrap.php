<?php
/**
 * Smarty plugin
 * @package	Smarty
 * @subpackage plugins
 */


/**
 * Smarty wordwrap modifier	plugin
 *
 * Type:	 modifier<br>
 * Name:	 wordwrap<br>
 * Purpose:	 wrap a	string of text at a	given length
 * @link http://smarty.php.net/manual/en/language.modifier.wordwrap.php
 *			wordwrap (Smarty online	manual)
 * @author	 Monte Ohrt	<monte at ohrt dot com>
 * @param string
 * @param integer
 * @param string
 * @param boolean
 * @return string
 */
function smarty_modifier_utf8_wordwrap($string,$length=80,$break="\n",$cut=false)
{
	return utf8_wordwrap($string, $length, $break, $cut);
}

/** 
 * wordwrap for utf8 encoded strings, http://milianw.de/section:Snippets/content:UTF-8-Wordwrap
 * 
 * @param string $str 
 * @param integer $len 
 * @param string $what 
 * @return string 
 */ 
 
function utf8_wordwrap($str, $width, $break,$cut = false){ 
    if(!$cut){ 
        $regexp = '#^(?:[\x00-\x7F]|[\xC0-\xFF][\x80-\xBF]+){'.$width.',}\b#U'; 
    } else { 
        $regexp = '#^(?:[\x00-\xFF]|[\xC0-\xFF][\x80-\xBF]+){'.$width.'}#'; 
    } 
    if(function_exists('mb_strlen')){ 
        $str_len = mb_strlen($str,'UTF-8'); 
    } else { 
        $str_len = preg_match_all('/[\x00-\x7F\xC0-\xFD]/', $str, $var_empty); 
    } 
    $while_what = ceil($str_len / $width); 
    $i = 1; 
    $return = ''; 
    while ($i < $while_what){ 
        preg_match($regexp, $str,$matches); 
        $string = $matches[0]; 
        $return .= $string . $break; 
        $str = substr($str,strlen($string)); 
        $i++; 
    } 
    return $return.$str; 
}


