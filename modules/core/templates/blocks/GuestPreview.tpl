{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if $user.isRegisteredUser}
<div class="{$class}">
  {capture name=guestPreviewMode}
  {if ($theme.guestPreviewMode)}
    <a href="{g->url arg1="controller=core.ShowItem" arg2="guestPreviewMode=0" arg3="return=1"}">{$user.userName}</a> | <span class="active"> {g->text text="guest"} </span>
  {else}
  <span class="active"> {$user.userName} </span> | <a href="{g->url arg1="controller=core.ShowItem" arg2="guestPreviewMode=1" arg3="return=1"}">{g->text text="guest"}</a>
  {/if}
  {/capture}
  {g->text text="display mode: %s" arg1=$smarty.capture.guestPreviewMode}
</div>
{/if}
