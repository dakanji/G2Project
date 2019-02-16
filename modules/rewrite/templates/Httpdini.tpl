{*
 * $Revision: 15865 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
# BEGIN Gallery 2 Url Rewrite section (GalleryID: {$Httpdini.galleryId})
# Do not edit this section manually. Gallery will overwrite it automatically.

RewriteCond Host: {$Httpdini.host}
RewriteRule {$Httpdini.galleryDirectory}modules/rewrite/data/isapi_rewrite/Rewrite.txt {$Httpdini.galleryDirectory}modules/rewrite/data/isapi_rewrite/Works.txt [O]

{foreach from=$Httpdini.rules item=rule}
{if !empty($rule.conditions)}
{foreach from=$rule.conditions item="condition"}
RewriteCond {$condition.test} {$condition.pattern}{if !empty($condition.flags)}   [{$condition.flags|@implode:","}]{/if}

{/foreach}
{/if}
RewriteRule ([^?]*)(?:\?(.*))? {$rule.substitution}{if !empty($rule.flags)}   [{$rule.flags|@implode:","}]{/if}

{/foreach}

# END Url Rewrite section
