{*
 * $Revision: 15505 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Comment change confirmation"} </h2>
</div>

<div class="gbBlock">
{if !empty($status)}
<h2 class="giSuccess">
  {if isset($status.added)}
    {g->text text="Comment added successfully"}
  {/if}
  {if isset($status.deleted)}
    {g->text text="Comment deleted successfully"}
  {/if}
  {if isset($status.changed)}
    {g->text text="Comment modified successfully"}
  {/if}
</h2>
{/if}

  <p>
    <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$CommentChangeConfirmation.item.id`"}">
      {g->text text="Back to %s" arg1=$CommentChangeConfirmation.itemTypeName.1}
    </a>
  </p>
</div>
