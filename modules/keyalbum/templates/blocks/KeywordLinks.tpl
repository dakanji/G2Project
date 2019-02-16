{*
 * $Revision: 16474 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if $forItem|default:true} {* Links for keywords of current item *}
{if empty($item)} {assign var=item value=$theme.item} {/if}
{assign var=showCloud value=$showCloud|default:false}

{if !empty($item.keywords)}
{g->callback type="keyalbum.SplitKeywords" keywords=$item.keywords}
<div class="{$class}">
  {g->text text="Keywords:"}
  {foreach from=$block.keyalbum.keywords key=rawKeyword item=keyword}
    <a href="{g->url arg1="view=keyalbum.KeywordAlbum" arg2="keyword=$rawKeyword"
		     arg3="highlightId=`$item.id`"}">{$keyword}</a>
  {/foreach}
</div>
{/if}

{else} {* Select box or cloud for all available keywords *}
{g->callback type="keyalbum.LoadKeywords"
	     onlyPublic=$onlyPublic|default:true sizeLimit=$sizeLimit|default:0
	     maxCloudFontEnlargement=$maxCloudFontEnlargement|default:3
	     includeFrequency=$showCloud}

{if !empty($block.keyalbum.keywords)}
<div class="{$class}">
  {if $showCloud}
    {foreach from=$block.keyalbum.keywords item=keyword}
      &nbsp;<a href="{g->url arg1="view=keyalbum.KeywordAlbum" arg2="keyword=`$keyword.raw`"}"{if
	     !empty($keyword.weight)} style="font-size: {$keyword.weight}em;"{/if}>
	  {$keyword.name}
      </a>&nbsp;
    {/foreach}
  {else}
  <select onchange="if (this.value) {ldelim} var newLocation = this.value; this.options[0].selected = true; location.href = newLocation; {rdelim}">
    <option value="">
      {g->text text="&laquo; Keyword Album &raquo;"}
    </option>
    {foreach from=$block.keyalbum.keywords item=keyword}
      <option value="{g->url arg1="view=keyalbum.KeywordAlbum" arg2="keyword=`$keyword.name`"}">
	{$keyword.name}
      </option>
    {/foreach}
  </select>
  {/if}
</div>
{/if}
{/if}
