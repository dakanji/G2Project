<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     modifier
 * Name:     stars
 * Purpose:  convert chars to stars, excluding N suffix chars
 * Date:     April 15th, 2004
 * Version:  1.0
 * Author:   Monte Ohrt <monte at ohrt dot com>
 * -------------------------------------------------------------
 */
function smarty_modifier_stars($string, $suffix = 0, $char = '*')
{
    $_prefix_len = strlen($string) - $suffix;
    if($_prefix_len > 0) {
        return str_repeat($char, $_prefix_len) . substr($string, -$suffix, $suffix);
    } else {
        return $string;
    }
}


