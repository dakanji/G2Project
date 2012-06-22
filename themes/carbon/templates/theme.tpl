{*
 * $Revision: 17592 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<!DOCTYPE html>
<html lang="{g->language}">
  <head>
    {* Let Gallery print out anything it wants to put into the <head> element *}
    {g->head}

    {if $theme.pageType == 'album' || $theme.pageType == 'photo'}
    <meta name="keywords" content="{$theme.item.keywords}" >
    <meta name="description" content="{$theme.item.description|markup:strip}" >
    {/if}
    {if $theme.pageType != 'admin'}
    <script type="text/javascript" src="{g->url href='themes/carbon/theme.js'}"></script>
    {/if}

    {* If Gallery doesn't provide a header, we use the album/photo title (or filename) *}
    {if empty($head.title)}
      <title>{$theme.item.title|default:$theme.item.pathComponent|markup:strip}</title>
    {/if}
  </head>
  <body class="gallery">
    {if !empty($jsWarning)}
      {include file="gallery:modules/core/templates/JavaScriptWarning.tpl" l10Domain="modules_core"}
    {/if}

    <div {g->mainDivAttributes}>
      {*
       * Some module views (eg slideshow) want the full screen.  So for those, we don't draw
       * a header, footer, navbar, etc.  Those views are responsible for drawing everything.
       *}
      {if $theme.useFullScreen}
	{include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
      {elseif $theme.pageType == 'progressbar'}
	<div id="gsHeader">
          {if !empty($theme.params.logoImageLocation)}
	  <img src="{g->url href=$theme.params.logoImageLocation}" alt="">
          {else}
	  <img src="{g->url href="images/galleryLogo_sm.gif"}" class="g2logo" alt="">
          {/if}
	</div>
	{g->theme include="progressbar.tpl"}
      {else}
      <div id="gsHeader">
	<table class="width100pc noSpacing noPadding">
	  <tr>
	    <td class="width50pc alignLeft alignTop">
	      <a href="{g->url}">
		{if !empty($theme.params.logoImageLocation)}
		<img src="{g->url href=$theme.params.logoImageLocation}" alt="">
		{else}
		<img src="{g->url href="images/galleryLogo_sm.gif"}" class="g2logo" alt="">
		{/if}
              </a>
	    </td>
	    <td class="alignRight alignTop">
	      {g->theme include="ads.tpl"}
	    </td>
	  </tr>
	</table>
      </div>

      <div id="gsNavBar" class="gcBorder1">
	<div class="gbSystemLinks">
	  {if !empty($theme.params.extraLink) && !empty($theme.params.extraLinkUrl)}
	  <span class="block-core-SystemLink">
	    <a href="{$theme.params.extraLinkUrl}">{$theme.params.extraLink}</a>
	  </span>
	  &laquo;
	  {/if}
	  {g->block type="core.SystemLinks"
		    order="core.SiteAdmin core.YourAccount core.Login core.Logout"
		    separator="&laquo;"
		    othersAt=4}
	  {if $theme.pageType != 'admin'}
	  <span class="block-core-SystemLink">
	    <a href="{g->url params=$theme.pageUrl arg1="jsWarning=true"}" 
	      onclick="toggleSidebar('sidebar'); return false;">{g->text text="Sidebar"}</a>
	  </span>
	  {/if}
	</div>

	<div class="gbBreadCrumb">
	  {g->block type="core.BreadCrumb" separator="&raquo;"}
	</div>
      </div>

      {* Include the appropriate content type for the page we want to draw. *}
      {if $theme.pageType == 'album'}
	{g->theme include="album.tpl"}
      {elseif $theme.pageType == 'photo'}
	{g->theme include="photo.tpl"}
      {elseif $theme.pageType == 'admin'}
	{g->theme include="admin.tpl"}
      {elseif $theme.pageType == 'module'}
	{g->theme include="module.tpl"}
      {/if}

      <div id="gsFooter" class="gcBorder1">
	<table class="gcBackground1 width100pc noSpacing noPadding">
	  <tr>
	    <td class="width50pc alignLeft">
	      {g->logoButton type="validation"}
	      {g->logoButton type="gallery2"}
	      {g->logoButton type="gallery2-version"}
	      {g->logoButton type="donate"}
	    </td>
	    <td class="alignRight">
	      {strip}
	      {if !empty($theme.params.copyright)}
		{$theme.params.copyright}
	      {/if}
	      {/strip}
	      {g->block type="core.GuestPreview"}
	    </td>
	  </tr>
	</table>
      </div>
      {/if}  {* end of full screen check *}
    </div>

    {*
     * Give Gallery a chance to output any cleanup code, like javascript that needs to be run
     * at the end of the <body> tag.  If you take this out, some code won't work properly.
     *}
    {g->trailer}

    {* Put any debugging output here, if debugging is enabled *}
    {g->debug}
  </body>
</html>
