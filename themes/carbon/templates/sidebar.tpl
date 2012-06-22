{*
 * $Revision: 17075 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div id="sidebar" class="gcPopupBackground">
  <table class="noSpacing noPadding">
    <tr>
      <td align="left" class="padLeft5">
	<h2>{g->text text="Actions"}</h2>
      </td>
      <td align="right" class="padRight2">
	<div class="buttonHideSidebar"><a href="javascript: slideOut('sidebar')"
	 title="{g->text text="Close"}"></a></div>
      </td>
    </tr>
    <tr>
      <td colspan="2" class="gcBackground2 padLeft5">
	<div id="gsSidebar" class="gcBorder1">
	  {* Show the sidebar blocks chosen for this theme *}
	  {foreach from=$theme.params.sidebarBlocks item=block}
	    {g->block type=$block.0 params=$block.1 class="gbBlock"}
	  {/foreach}
	</div>
      </td>
    </tr>
  </table>
</div>
 {* shim in the style for html 5 validity *}
 <script type="text/javascript">
 document.getElementById("sidebar").style.position = 'absolute';
 document.getElementById("sidebar").style.left = '-190px';
 document.getElementById("sidebar").style.top = '{$theme.params.sidebarTop}px';
 document.getElementById("sidebar").style.padding = '1px';
 </script>
