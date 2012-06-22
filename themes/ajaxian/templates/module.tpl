{*
 * $Revision: 16235 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{if empty($theme.params.sidebarBlocks)}
  {include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
{else}
<table class="width100pc noSpacing noPadding">
  <tr class="alignTop">
    <td id="gsSidebarCol">
      {g->theme include="sidebar.tpl"}
    </td>
    <td>
      {include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
    </td>
  </tr>
</table>
{/if}
