{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{* Set defaults *}
{if empty($item)} {assign var=item value=$theme.item} {/if}

{g->callback type="rating.LoadRating" itemId=$item.id}

{if !empty($block.rating.RatingData)}
<div class="{$class}">
{include file="gallery:modules/rating/templates/RatingInterface.tpl" 
	RatingData=$block.rating.RatingData
	RatingSummary=$block.rating.RatingSummary}
</div>
{/if}
