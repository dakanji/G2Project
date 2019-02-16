{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if empty($theme.params.sidebarBlocks)}
  {include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
{else}
<table width="100%" cellspacing="0" cellpadding="0">
  <tr valign="top">
    <td id="gsSidebarCol">
      {g->theme include="sidebar.tpl"}
    </td>
    <td>
      {include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
    </td>
  </tr>
</table>
{/if}
