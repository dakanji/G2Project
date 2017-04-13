<?php
/**
 * Smarty substr modifier plugin
 *
 * Type:     modifier<br>
 * Name:     smarty<br>
 * Purpose:  make php's substr available to templates.
 */
function smarty_modifier_substr($string, $start = -1, $length = -1){
   if($start == -1 && $length == -1)
     return substr($string);
   else if($length == -1)
     return substr($string, $start);
   else
     return substr($string, $start, $length);
}


