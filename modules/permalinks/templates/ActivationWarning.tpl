{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock">
  <h3> {g->text text="Permalink post activation"} </h3>

{capture assign="link"}
  <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=rewrite.AdminRewrite"}">
    {g->text text="URL Rewrite Module"}</a>
{/capture}

  <p>{g->text text="Now that you have activated the Permalinks module, you have to activate Permalinks rule in the %s." arg1="`$link`"} </p>
</div>
