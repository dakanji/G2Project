{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Move %s" arg1=$ItemMoveSingle.itemTypeNames.0} </h2>
</div>

{if isset($status.moved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Successfully moved"}
</h2></div>
{/if}

<div class="gbBlock">
  <h3> {g->text text="Destination"} </h3>

  <p class="giDescription">
    {g->text text="Choose a destination album"}
  </p>

  <select name="{g->formVar var="form[destination]"}" onchange="checkPermissions(this.form)">
    {foreach from=$ItemMoveSingle.albumTree item=album}
      <option value="{$album.data.id}" {if ($album.data.id == $form.destination)}selected="selected"{/if}>
	{"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	{$album.data.title|default:$album.data.pathComponent}
      </option>
    {/foreach}
  </select>

  {if isset($form.error.destination.empty)}
  <div class="giError">
    {g->text text="No destination chosen"}
  </div>
  {/if}
  {if isset($form.error.destination.selfMove)}
  <div class="giError">
    {g->text text="You cannot move an album into its own subtree."}
  </div>
  {/if}
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][move]"}" value="{g->text text="Move"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>
