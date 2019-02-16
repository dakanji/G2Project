{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<table class="gcBackground1" width="100%" cellspacing="0" cellpadding="0">
  <tr valign="top">
    <td>
      {include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
    </td>
  </tr>
</table>
{if !empty($theme.params.sidebarBlocks)}
  {g->theme include="sidebar.tpl"}
{/if}
