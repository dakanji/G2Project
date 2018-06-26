<?php

/**
 * Function used in the admin interface as a shorthand to check whether the user has the given permission
 *
 * @param key
 */
function smarty_block_user_cannot_override($params, $content, &$smarty) {
	if (isset($content)) {
		// fetch the user
		if (isset($params['key'])) {
			$key = $params['key'];
		} else {
			$smarty->trigger_error("user_cannot_override: missing 'key' parameter!");
		}

		if (GlobalPluginConfig::canOverride($key) == PLUGIN_SETTINGS_USER_CANNOT_OVERRIDE) {
			return $content;
		}

		return '';
	}
}
