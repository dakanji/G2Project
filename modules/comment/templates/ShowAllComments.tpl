{*
 * $Revision: 15949 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Latest Comments"} </h2>
</div>

{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
  {if isset($status.changed)}
    {g->text text="Comment changed successfully"}
  {/if}
</h2></div>
{/if}

{if empty($ShowAllComments.comments)}
<div class="gbBlock">
  <h3> {g->text text="There are no comments for this item"} </h3>
</div>
{else}
{if $ShowAllComments.navigator.pageCount > 1}
  {g->block type="core.Navigator" class="commentNavigator"
	    navigator=$ShowAllComments.navigator
	    currentPage=$ShowAllComments.navigator.page
	    totalPages=$ShowAllComments.navigator.pageCount}
{/if}
<table>
{foreach from=$ShowAllComments.comments item=comment}
<tr><td style="text-align: center; padding: 0 4px">
  {assign var="item" value=$ShowAllComments.itemData[$comment.parentId]}
  <a id="CommentThumb-{$item.id}" href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.id`"}">
    {if isset($item.thumb)}
      {g->image item=$item image=$item.thumb maxSize=120}
    {else}
      {$item.title|default:$item.pathComponent|markup}
    {/if}
  </a>
  <script type="text/javascript">
    //<![CDATA[
    {* force and alt/longdesc parameter here so that we avoid issues with single quotes in the title/description *}
    {if isset($item.resize)}
    new YAHOO.widget.Tooltip("gTooltip", {ldelim}
        context: "CommentThumb-{$item.id}", text: '{g->image item=$item image=$item.resize maxSize=500 alt="" longdesc=""}', showDelay: 250 {rdelim});
    {elseif isset($item.thumb)}
    new YAHOO.widget.Tooltip("gTooltip", {ldelim}
        context: "CommentThumb-{$item.id}", text: '{g->image item=$item image=$item.thumb alt="" longdesc=""}', showDelay: 250 {rdelim});
    {/if}
    //]]>
  </script>
</td><td>
  <div class="one-comment gcBorder2">
  {include file="gallery:modules/comment/templates/Comment.tpl"
	   comment=$comment can=$ShowAllComments.can[$comment.id]
	   item=$item user=$ShowAllComments.commenters[$comment.commenterId]}
  </div>
</td></tr>
{/foreach}
</table>
{/if}
