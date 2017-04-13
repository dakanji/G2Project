<?php

/**
 * Shamelessly ripped off from the phpbb pager
 */
function smarty_function_adminpager($params, &$smarty)	
{
	lt_include( PLOG_CLASS_PATH."class/template/smarty/plugins/function.pager.php" );
	// fetch the parameters
	if( isset( $params["data"]))
		$pager = $params["data"];
	else {
		// see if we can load the pager from the smarty context
		if( isset( $smarty->_tpl_vars["pager"] ))
			$pager = $smarty->_tpl_vars["pager"];
		else
			$smarty->trigger_error( "'data' parameter missing for pager!" );			
	}
	
	// Style parameter. It can either "links", "list", "forwardonly" and "backonly"
	isset( $params["style"] ) ? $style = $params["style"] : $style = "links";
	
	$base_url = $pager->getBaseUrl();
	$total_pages = $pager->getTotalPages();
	$per_page = $pager->getRegsForPage();
	$start_item = 1;
	$add_prevnext_text = true;
	$on_page = $pager->getCurrentPage();	

	if( $style == "list" ) {
		$page_string .= "<span class=\"pager\">";
		if( !$pager->isFirstPage() && !$pager->isEmpty()) {
			$page_string .= "<span class=\"list_action_button\">
			   <a href=\"".$pager->getPrevPageLink()."\"><img src=\"imgs/admin/icon_left-16.png\" /></a>
			 </span>";
		}
		$page_string .= "<select name=\"plogPager\" id=\"plogPager\" onChange=\"location.href=this.options[this.selectedIndex].value\""; 
		if( $pager->isEmpty()) {
			$page_string .= "disabled=\"disabled\"";
		}
		$page_string .= ">";
		foreach( $pager->getPageLinks() as $pageId => $pageLink ) {
		    $page_string .= "<option value=\"$pageLink\"";
			if( $pageId == $pager->getCurrentPage())
				$page_string .= "selected=\"selected\"";
			$page_string .= ">$pageId</option>";
		}

		$page_string .= "</select>";
		
		if( !$pager->isLastPage() && !$pager->isEmpty()) {
		 	$page_string .= "<span class=\"list_action_button\">";
		    $page_string .= "<a href=\"".$pager->getNextPageLink()."\"><img src=\"imgs/admin/icon_right-16.png\" /></a>";
		 	$page_string .= "</span>";
		}
		$page_string .= "</span>";
	}
	else {
		smarty_function_pager( $params, $smarty );
	}

	return $page_string;
}

