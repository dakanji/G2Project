{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{* Set defaults *}
{if empty($item)} {assign var=item value=$theme.item} {/if}
{if empty($show)} {assign var=show value=3} {/if}
{g->callback type="comment.LoadComments" itemId=$item.id show=$show}

{if !empty($block.comment.LoadComments.comments)}
<div class="{$class}">
  <h3> {g->text text="Recent comments"} </h3>

  {if sizeof($block.comment.LoadComments.comments) < $block.comment.LoadComments.totalComments }
  <a href="{g->url arg1="view=comment.ShowComments" arg2="itemId=`$block.comment.LoadComments.item.id`"}">
    {g->text text="(view all %d comments)" arg1=$block.comment.LoadComments.totalComments}
  </a>
  {/if}

  {foreach from=$block.comment.LoadComments.comments item=comment}
    <div class="one-comment gcBorder2">
      {include file="gallery:modules/comment/templates/Comment.tpl"
	       user=$block.comment.LoadComments.commenters[$comment.commenterId]
	       comment=$comment
	       can=$block.comment.LoadComments.can
	       item=$block.comment.LoadComments.item
	       truncate=256}
    </div>
  {/foreach}
</div>
{/if}
