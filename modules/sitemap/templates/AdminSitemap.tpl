{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Sitemap settings"} </h2>
</div>

<div class="gbBlock">
  {capture assign="googleLink"}
   <a href="http://google.com/webmasters/sitemaps/siteoverview">{g->text text="Google Sitemaps"}</a>
  {/capture}
  {capture assign="sitemapLink"}
   <b> {g->url arg1="view=sitemap.Sitemap" forceFullUrl=1} </b>
  {/capture}
  <p class="giDescription">
    {g->text text="To use the Google Sitemap, you must now go to the %s page and submit this url: %s" arg1=$googleLink arg2=$sitemapLink}
  </p>

  {capture assign="link"}{strip}
    {if $AdminSitemap.canRewrite}
      {assign var=subView value="rewrite.AdminRewrite"}
    {else}
      {assign var=subView value="core.AdminPlugins"}
    {/if}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=`$subView`"}">
      {g->text text="URL Rewrite Module"}</a>
  {/strip}{/capture}
  <p class="giDescription">
    {g->text text="You can change the Sitemap url using the %s." arg1=$link} </p>
  </p>
</div>
