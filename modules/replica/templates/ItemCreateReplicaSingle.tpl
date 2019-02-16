{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Replicate %s" arg1=$ItemCreateReplicaSingle.itemTypeNames.0} </h2>
</div>

{if isset($status.linked)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Successfully created"}
</h2></div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="A replica is an item that shares the original data file with another item, to save disk space. In all other respects it is a separate item, with its own captions, thumbnail, resizes, comments, etc. Captions are initially copied from the source item but may be changed. Either the replica or the source may be moved or deleted without affecting the other."}
  </p>

  <h3> {g->text text="Destination"} </h3>

  <p class="giDescription">
    {g->text text="Choose a destination album"}
  </p>

  <select name="{g->formVar var="form[destination]"}" onchange="checkPermissions(this.form)">
    {foreach from=$ItemCreateReplicaSingle.albumTree item=album}
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
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][link]"}" value="{g->text text="Create"}"/>
</div>
