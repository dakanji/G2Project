<?php

/**
 * Smarty {break} compiler function plugin
 *
 * Type:     compiler function<br>
 * Name:     break<br>
 * Purpose:  break out of a foreach loop
 * @author Ferdinand Beyer: http://osdir.com/ml/php.smarty.general/2002-08/msg00058.html
 * @param string containing var-attribute and value-attribute
 * @param Smarty_Compiler
 */
function smarty_compiler_break($contents, &$smarty)
{
    return 'break;';
}
