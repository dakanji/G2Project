{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<script type="text/javascript">
{if $theme.imageCount==1}
var image_width = new Array(1); image_width[0] = {$theme.imageWidths};
var image_height = new Array(1); image_height[0] = {$theme.imageHeights};
{else}
var image_width = new Array({$theme.imageWidths});
var image_height = new Array({$theme.imageHeights});
{/if}
var view = {$theme.viewIndex|default:-1};
</script>
<script type="text/javascript" src="{$theme.themeUrl}/tile.js"></script>
<style type="text/css">
div.emptyTile {ldelim}
  width: {$theme.param.cellWidth}px;
  height: {$theme.param.cellHeight}px
{rdelim}
</style>
