<?php
	/**
	 * Smarty plugin
	 * @package Smarty
	 * @subpackage plugins
	 */


	/**
	 * {assignvar var=key value=$template_var}
	 *
	 * Smarty's equivalent to PHP's $key=$$template_var
	 * 
	 * @param array
	 * @param Smarty
	 */
	function smarty_function_assignvar($params, &$smarty)
	{
	    if (!isset($params['var'])) {
	        $smarty->trigger_error("assign: missing 'var' parameter");
	        return;
	    }

	    if (!isset( $params['value'])) {
	        $smarty->trigger_error("assign: missing 'value' parameter");
	        return;
	    }
	
	    $smarty->assign($params['var'], $smarty->_tpl_vars[$params["value"]] );
	}		

