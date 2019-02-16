{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="View Comments"} </h2>
</div>

{if !empty($status)}
<div class="gbBlock">
  <h2 class="giSuccess">
    {if isset($status.changed)}
    {g->text text="Comment changed successfully"}
    {/if}
  </h2>
</div>
{/if}

{if empty($ShowComments.comments)}
<div class="gbBlock">
  <h3> {g->text text="There are no comments for this item"} </h3>
</div>
{else}
<div class="gbBlock">
{foreach from=$ShowComments.comments item=comment}
  <div class="one-comment gcBorder2">
  {include file="gallery:modules/comment/templates/Comment.tpl"
	   comment=$comment item=$ShowComments.item can=$ShowComments.can
	   user=$ShowComments.commenters[$comment.commenterId]}
  </div>
{/foreach}
</div>
{/if}
