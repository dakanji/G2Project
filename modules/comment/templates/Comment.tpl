{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if !empty($comment.subject)}
<h3>
  {$comment.subject|markup}
</h3>
{/if}

{if $can.edit}
<span class="edit">
  <a href="{g->url arg1="view=comment.EditComment" arg2="itemId=`$item.id`"
		   arg3="commentId=`$comment.id`" arg4="return=true"}">
    {g->text text="edit"}</a>
</span>
{/if}

{if $can.delete}
<span class="delete">
  <a href="{g->url arg1="view=comment.DeleteComment" arg2="itemId=`$item.id`"
		   arg3="commentId=`$comment.id`" arg4="return=true"}">
    {g->text text="delete"}</a>
</span>
{/if}

{assign var="commentText" value=$comment.comment|markup}
{if isset($truncate)}
  {assign var="truncated" value=$commentText|entitytruncate:$truncate}
{/if}

{if isset($truncate) && ($truncated != $commentText)}
  <a id="comment-more-toggle-{$comment.id}"
      onclick="document.getElementById('comment-truncated-{$comment.id}').style.display='none';
	       document.getElementById('comment-full-{$comment.id}').style.display='block';
	       document.getElementById('comment-more-toggle-{$comment.id}').style.display='none';
	       document.getElementById('comment-less-toggle-{$comment.id}').style.display='inline';"
      >{g->text text="show full"}</a>
  <a id="comment-less-toggle-{$comment.id}"
      onclick="document.getElementById('comment-truncated-{$comment.id}').style.display='block';
	       document.getElementById('comment-full-{$comment.id}').style.display='none';
	       document.getElementById('comment-more-toggle-{$comment.id}').style.display='inline';
	       document.getElementById('comment-less-toggle-{$comment.id}').style.display='none';"
      style="display: none">{g->text text="show summary"}</a>

  <p id="comment-truncated-{$comment.id}" class="comment">
    {$truncated}
  </p>
  <p id="comment-full-{$comment.id}" class="comment" style="display: none">
    {$commentText}
  </p>
{else}
  <p class="comment">
    {$commentText}
  </p>
{/if}

<p class="info">
  {capture name="date"}{g->date timestamp=$comment.date style="datetime"}{/capture}
  {if empty($comment.author)}
    {if $can.edit}
      {g->text text="Posted by %s on %s (%s)"
	       arg1=$user.fullName|default:$user.userName
	       arg2=$smarty.capture.date arg3=$comment.host}
    {else}
      {g->text text="Posted by %s on %s"
	       arg1=$user.fullName|default:$user.userName arg2=$smarty.capture.date}
    {/if}
  {else}
    {if $can.edit}
      {g->text text="Posted by %s (guest) on %s (%s)"
	       arg1=$comment.author|default:$user.userName
	       arg2=$smarty.capture.date arg3=$comment.host}
    {else}
      {g->text text="Posted by %s (guest) on %s"
	       arg1=$comment.author|default:$user.userName arg2=$smarty.capture.date}
    {/if}
  {/if}
</p>
