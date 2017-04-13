<?php 
/* 
Smarty Modifier url_parse 
$string - The string to parse 
$what  - One of the following: 
- scheme 
- host 
- port 
- users 
- pass 
- path 
- query 
- fragment 
*/ 

function smarty_modifier_url_parse($string, $what) 
{ 
  $url=@parse_url($string); 
  return($url[$what]); 
} 


