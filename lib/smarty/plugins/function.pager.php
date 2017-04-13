<?php

/**
 * Shamelessly ripped off from the phpbb pager
 *
 * Supported parameters:
 * @param data The Pager object containing the information that needs to be displayed 
 * @param style Style, one of the following values: 'links', 'list', 'prevonly', 'forwardonly'
 * @param separator The character to use as a separator between pages, defaults to an empty space
 * @param next Text to be used to the link to the next page in the sequence. If not present, it defaults to
 * whatever the current locale has defined for 'next_post'
 * @param previous Text to be used to the link to the previous page in the sequence. If not present, it defaults to
 * whatever the current locale has defined for 'prev_post'
 * @param disablediv Whether not to enclose the pager data in a <div class="pager">...</pager> or not. Defaults to 'false'
 * @param anchor Specify the name of an HTML anchor that will be appended to each one of the links in pages, so that
 * we can get the browser to jump to the right section of the page (in case the paged content doesn't start at the
 * top of the page)
 */
function smarty_function_pager($params, &$smarty)	
{
	// fetch the parameters
	if( isset( $params["data"]))
		$pager = $params["data"];
	else {
		// see if we can load the pager from the smarty context
		if( isset( $smarty->_tpl_vars["pager"] ))
			$pager = $smarty->_tpl_vars["pager"];
		else
			return( "" );
	}
	
	// Style parameter. It can either "links", "list", "forwardonly" and "backonly"
	isset( $params["style"] ) ? $style = $params["style"] : $style = "links";
	
	// Separator parameter, defaults to blank spaces
	isset( $params["separator"] ) ? $separator = $params["separator"] : $separator = " ";
	
	// whether to use a <div> area or not
	$useDiv = !isset( $params["disablediv" ]);
	
	// Text used for the "next" link. If not present, this function will look for an object called
	// $locale in the template context. If not available, the default locale will be used
	if( isset( $params["next"])) {
		$nextText = $params["next"];
	}
	else {
		if( isset( $smarty->_tpl_vars["locale"] )) {
			$locale = $smarty->_tpl_vars["locale"];			
		}
		else {
			lt_include( PLOG_CLASS_PATH."class/locale/ltlocales.class.php" );
			$locale =& LTLocales::getLocale();
		}
		$nextText = $locale->tr( "next_post" )."&raquo;";
	}
	
	// Text used for the "previous" link. If not present, this function will look for an object called
	// $locale in the template context. If not available, the default locale will be used
	if( isset( $params["previous"])) {
		$prevText = $params["previous"];
	}
	else {
		if( isset( $smarty->_tpl_vars["locale"] )) {
			$locale = $smarty->_tpl_vars["locale"];			
		}
		else {
			lt_include( PLOG_CLASS_PATH."class/locale/ltlocales.class.php" );
			$locale =& LTLocales::getLocale();
		}
		$prevText = "&laquo;".$locale->tr( "previous_post" );
	}	
	
    // number of pages shown in the beginning
	isset( $params["beginning"] ) ? $beginning = $params["beginning"] : $beginning = 3;
	// number of pages shown in the middle
	isset( $params["middle"] ) ? $middle = $params["middle"] : $middle = 5;
	// Number of pages shown in the end
	isset( $params["end"] ) ? $end = $params["end"] : $end = 3;
	// whether we need to append an anchor reference to all links (in case the paged
	// doesn't start immediately at the top of the page)
	isset( $params["anchor"]) ? $anchor = $params["anchor"] : $anchor = "";
	
		
	$base_url = $pager->getBaseUrl();
	$total_pages = $pager->getTotalPages();
	$per_page = $pager->getRegsForPage();
	$start_item = 1;
	$add_prevnext_text = true;
	$on_page = $pager->getCurrentPage();

	$page_string = '';
	
	$pageLinks = $pager->getPageLinks();
	
	if( $style == "links" ) {
		if ( $total_pages == 1 )
			return '';

		if ( $total_pages > ($beginning + 1 + $middle + 1 + $middle + 1 + $end ))
		{
			$init_page_max = ( $total_pages > $beginning ) ? $beginning : $total_pages;

			for($i = 1; $i < $init_page_max + 1; $i++) {
				$page_string .= ( $i == $on_page ) ? " <span class=\"pagerCurrent\">$i</span>" : " <a class=\"pagerLink\" href=\"".$pageLinks[$i].$anchor."\">$i</a>";
				if ( $i <  $init_page_max ) {
					$page_string .= $separator;
				}
			}

			if ( $on_page > 1  && $on_page < $total_pages ) {
				$page_string .= ( $on_page > ($beginning + $middle + 1) ) ? ' ... ' : $separator;

				$init_page_min = ( $on_page > ($beginning + $middle) ) ? $on_page : ($beginning + $middle + 1 );
				$init_page_max = ( $on_page < $total_pages - ($end + 1) ) ? $on_page : $total_pages - ($end + $middle);


				for($i = $init_page_min - $middle; $i < $init_page_max + ($middle + 1); $i++) {
					$page_string .= ( $i == $on_page ) ? " <span class=\"pagerCurrent\">$i</span>" : " <a class=\"pagerLink\" href=\"".$pageLinks[$i].$anchor."\">$i</a>";
					if ( $i <  $init_page_max + 1 ) {
						$page_string .= $separator;
					}
				}

				$page_string .= ( $on_page < $total_pages - ($end + $middle) ) ? ' ... ' : $separator;
			}
			else {
				$page_string .= ' ... ';
			}
			
			for($i = $total_pages - ($end - 1); $i < $total_pages + 1; $i++) {
				$page_string .= ( $i == $on_page ) ? " <span class=\"pagerCurrent\">$i</span>" : " <a class=\"pagerLink\" href=\"".$pageLinks[$i].$anchor."\">$i</a>";
				if( $i <  $total_pages ) {
					$page_string .= $separator;
				}
			}
		}
		else {
			for($i = 1; $i < $total_pages + 1; $i++) {
				$page_string .= ( $i == $on_page ) ? " <span class=\"pagerCurrent\">$i</span>" : " <a class=\"pagerLink\" href=\"".$pageLinks[$i].$anchor."\">$i</a>";
				if ( $i <  $total_pages ) {
					$page_string .= $separator;
				}
			}
		}

		if ( $add_prevnext_text ) {
			if ( $on_page > 1 ) {
				$page_string = ' <a class="pagerLinkPrevPage" href="'.$pageLinks[$on_page - 1 ].$anchor.'">'.$prevText.'</a>&nbsp;&nbsp;'.$page_string;
			}

			if ( $on_page < $total_pages ) {
				$page_string .= '&nbsp;&nbsp;<a class="pagerLinkNextPage" href="'.$pageLinks[$on_page + 1].$anchor.'">'.$nextText.'</a>';
			}			
		}
	}
	elseif( $style == "list" ) {
		$page_string .= "
		<script type=\"text/javascript\">
			function onPagerListChange(list) 
			{
				var index = list.selectedIndex;
				var value = list.options[index].value;
				location.href = value;
				return true;
			}
		</script>
		<span class=\"pager\">";
		if( !$pager->isFirstPage() && !$pager->isEmpty()) {
			$page_string .= "<span class=\"list_action_button\">
			   <a href=\"".$pager->getPrevPageLink().$anchor."\"><img src=\"imgs/admin/icon_left-16.png\" alt=\"$prevText\" /></a>
			 </span>";
		}
		$page_string .= "<select name=\"plogPager\" id=\"plogPager\" onChange=\"onPagerListChange(this)\""; 
		if( $pager->isEmpty()) {
			$page_string .= "disabled=\"disabled\"";
		}
		$page_string .= ">";
		foreach( $pager->getPageLinks() as $pageId => $pageLink ) {
		    $page_string .= "<option value=\"{$pageLink}{$anchor}\"";
			if( $pageId == $pager->getCurrentPage())
				$page_string .= "selected=\"selected\"";
			$page_string .= ">$pageId</option>";
		}

		$page_string .= "</select>";
		
		if( !$pager->isLastPage() && !$pager->isEmpty()) {
		 	$page_string .= "<span class=\"list_action_button\">";
		    $page_string .= "<a href=\"".$pager->getNextPageLink().$anchor."\"><img src=\"imgs/admin/icon_right-16.png\" alt=\"$nextText\" /></a>";
		 	$page_string .= "</span>";
		}
		$page_string .= "</span>";
	}
	elseif( $style == "prevonly" ) {
		if (!$pager->isFirstPage() && !$pager->isEmpty()) {
		   $page_string .= "<a class=\"pagerLinkPrevPage\" href=\"".$pager->getPrevPageLink().$anchor."\">$prevText</a>&nbsp;";
		}
	}
	elseif( $style == "nextonly" ) {
		if (!$pager->isLastPage() && !$pager->isEmpty()) {
		   $page_string .= "<a class=\"pagerLinkNextPage\" href=\"".$pager->getNextPageLink().$anchor."\">$nextText</a>&nbsp;";
		}		
	}
	else {
		$smarty->trigger_error( "Unrecognized 'style' parameter for the pager. Valid values are: 'links', 'prevonly', 'nextonly'" );
	}
	
	if( $useDiv )
		$page_string = '<div class="pager">'.$page_string.'</div>';

	return $page_string;
}

