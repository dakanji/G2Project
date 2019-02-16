{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div id="sidebar" class="gcPopupBackground"
 style="position:absolute; left:-190px; top:{$theme.params.sidebarTop}px; padding:1px;">
  <table cellspacing="0" cellpadding="0">
    <tr>
      <td align="left" style="padding-left:5px;">
	<h2>{g->text text="Actions"}</h2>
      </td>
      <td align="right" style="padding-right:2px;">
	<div class="buttonHideSidebar"><a href="javascript: slideOut('sidebar')"
	 title="{g->text text="Close"}"></a></div>
      </td>
    </tr>
    <tr>
      <td colspan="2" class="gcBackground2" style="padding-bottom:5px">
	<div id="gsSidebar" class="gcBorder1">
	  {* Show the sidebar blocks chosen for this theme *}
	  {foreach from=$theme.params.sidebarBlocks item=block}
	    {g->block type=$block.0 params=$block.1 class="gbBlock"}
	  {/foreach}
	  {g->block type="core.NavigationLinks" class="gbBlock"}
	</div>
      </td>
    </tr>
  </table>
</div>
