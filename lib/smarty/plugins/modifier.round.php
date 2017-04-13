<?php 
/** 
 * Smarty Modifier round 
 * $round The number we're trying to round (don't try this with strings!!)
 * $locale An optional LTLocale object that will be used to localize the strings to be displayed.
 * If none is specified, the default locale will be used.
 */ 
function smarty_modifier_round($size,$locale = null) 
{
	// load the default locale if none provided
	if( $locale == null ) {
		lt_include( PLOG_CLASS_PATH."class/locale/ltlocales.class.php" );
		$locale =& LTLocales::getLocale();
	}
	
    if ($size < pow(2,10)) return( $size." ".$locale->tr("bytes"));
	if ($size >= pow(2,10) && $size < pow(2,20)) return( round($size / pow(2,10), 0)." ".$locale->tr("kb"));
	if ($size >= pow(2,20) && $size < pow(2,30)) return( round($size /pow(2,20), 1)." ".$locale->tr("mb"));
	if ($size > pow(2,30)) return( round($size / pow(2,30), 2)." ".$locale->tr("gb"));
} 

