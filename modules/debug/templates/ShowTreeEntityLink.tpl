{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<a href="{g->url arg1="view=debug.ShowTree" arg2="entityId=$entityId"}">
  {g->text text="%d: (%s)" arg1=$entityId arg2=$ShowTree.entityTable.$entityId.entityType}
</a>

{if !empty($ShowTree.isItem.$entityId)}
  &nbsp;
  <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=$entityId"}">
    {g->text text="[browse]"}
  </a>
{/if}
