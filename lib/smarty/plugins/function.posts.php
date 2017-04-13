<?php
	function smarty_function_posts($params, &$smarty)
	{
    	// we can do this since we loaded this value from within
        // the action
    	$articles = $smarty->get_template_vars("articles");

        // go through all the posts
        foreach( $articles as $article ) {
			$smarty->assign( "post", $article );
            $smarty->display( "default/post.template" );
        }
	}

