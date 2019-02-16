{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Nokia Image Upload Configuration"} </h2>
</div>

{*
 * successful configure should return to modules view.
 * this block only needed until that bug is fixed.
 *}
{if isset($status.configured)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="This module creates a target album for uploaded images.  All users who perform an upload are automatically added to a group with upload access to this album.  The album and group will be created using the information below."}
  </p>

  <h4> {g->text text="User Group"} </h4>
  <p class="giDescription">
    {g->text text="Enter a name for the group"}
  </p>

  <input type="text" size="30" name="{g->formVar var="form[group]"}" value="{$form.group}"/>

  {if isset($form.error.group.missing)}
  <div class="giError">
    {g->text text="Missing group name"}
  </div>
  {/if}
  {if isset($form.error.group.duplicate)}
  <div class="giError">
    {g->text text="Group name already exists"}
  </div>
  {/if}

  <h4> {g->text text="Upload Album"} </h4>
  <p class="giDescription">
    {g->text text="Enter a title for the Upload Album"}
  </p>

  <input type="text" size="30" name="{g->formVar var="form[album]"}" value="{$form.album}"/>

  {if isset($form.error.album.missing)}
  <div class="giError">
    {g->text text="Missing album title"}
  </div>
  {/if}

  <p class="giDescription" style="margin-top:0.7em">
    {g->text text="Choose a location for the Upload Album"}
  </p>
  <select name="{g->formVar var="form[parent]"}">
    {foreach from=$NokiaUploadConfigure.albumTree item=album}
      <option value="{$album.data.id}" {if $album.data.id==$form.parent}selected="selected"{/if}>
	{"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	{$album.data.title|default:$album.data.pathComponent}
      </option>
    {/foreach}
  </select>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Configure"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>
