{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Delete %s" arg1=$ItemDeleteSingle.itemTypeNames.0} </h2>
</div>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Are you sure you want to delete this %s?"
	     arg1=$ItemDeleteSingle.itemTypeNames.1}
    {if $ItemDeleteSingle.childCount > 0}
      {g->text one="It contains %d item." many="It contains %d items."
	       count=$ItemDeleteSingle.childCount arg1=$ItemDeleteSingle.childCount}
    {/if}

    <strong>
      {g->text text="There is no undo!"}
    </strong>
  </p>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][delete]"}" value="{g->text text="Delete"}"/>
</div>
