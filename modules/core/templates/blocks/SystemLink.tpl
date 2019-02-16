{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if isset($theme.systemLinks[$linkId])}
<span class="{$class}">
  <a href="{g->url
     params=$theme.systemLinks[$linkId].params}">{$theme.systemLinks[$linkId].text}</a>
</span>
{/if}
