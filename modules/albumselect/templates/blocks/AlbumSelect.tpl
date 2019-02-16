{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="albumselect.LoadAlbumData"
	     stripTitles=true truncateTitles="20" createTextTree=true}

{if isset($block.albumselect)}
{assign var="data" value=$block.albumselect.LoadAlbumData.albumSelect}
<div class="{$class}">
  <select onchange="if (this.value) {ldelim} var newLocation = '{$data.links.prefix}' + this.value; this.options[0].selected = true; location.href = newLocation; {rdelim}">
    <option value="">
      {g->text text="&laquo; Jump to Album &raquo;"}
    </option>
    {foreach from=$data.tree item=node}
      <option value="{$data.links[$node.id]}">
	{$data.titles[$node.id]}
      </option>
    {/foreach}
  </select>
</div>
{/if}
