{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="rss.FeedList"}
{if !empty($block.rss.feeds)}
  {foreach from=$block.rss.feeds item=feed}
    <link rel="alternate" type="application/rss+xml" title="{$feed}"
      href="{g->url arg1="view=rss.Render" arg2="name=`$feed`"}" />
  {/foreach}
{/if}

