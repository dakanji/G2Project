<?php

/**
 * Function used in the admin interface as a shorthand to check whether the user has the given permission
 *
 * @param perm
 * @param user
 * @param blog
 */
function smarty_block_check_perms($params, $content, &$smarty) {
	if (isset($content)) {
		// fetch the user
		if (isset($params['user'])) {
			$user = $params['user'];
		} else {
			// see if we can load the user from the smarty context
			if (isset($smarty->_tpl_vars['user'])) {
				$user = $smarty->_tpl_vars['user'];
			} else {
				$smarty->trigger_error('Cannot load a user');
			}
		}

		// fetch the blog
		if (isset($params['blog'])) {
			$blog = $params['blog'];
		} else {
			// see if we can load the user from the smarty context
			if (isset($smarty->_tpl_vars['blog'])) {
				$blog = $smarty->_tpl_vars['blog'];
			} else {
				$smarty->trigger_error('Cannot load a blog');
			}
		}

		// fetch the permission name
		if (isset($params['perm'])) {
			$perm = $params['perm'];

			// if the user is an admin, he should be allowed
			if ($user->isSiteAdmin()) {
				return $content;
			}

			// if the user is the blog owner, he should be allowed
			if ($user->getId() == $blog->getOwnerId()) {
				return $content;
			}

			$blogId = $blog->getId();
		} elseif (isset($params['adminperm'])) {
			$perm   = $params['adminperm'];
			$blogId = 0;
		} else {
			$smarty->trigger_error("'perm' and 'adminperm' parameters are both missing!");
		}

		// check the permission
		if ($user->hasPermissionByName($perm, $blogId)) {
			return $content;
		}

		return '';
	}
}
