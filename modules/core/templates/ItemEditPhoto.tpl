{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Resized Photos"} </h3>

  <p class="giDescription">
    {g->text text="These sizes are alternate resized versions of the original you would like to have available for viewing."}
  </p>

  {if $ItemEditPhoto.editSizes.can.createResizes}
    {counter start=0 assign=index}
    {foreach from=$form.resizes item=resize}
      <input type="checkbox"{if $form.resizes.$index.active} checked="checked"{/if}
       name="{g->formVar var="form[resizes][$index][active]"}"/>
      {g->dimensions formVar="form[resizes][$index]"
		     width=$form.resizes.$index.width height=$form.resizes.$index.height}
      <br/>

      {if !empty($form.error.resizes.$index.size.missing)}
      <div class="giError">
	{g->text text="You must enter a valid size"}
      </div>
      {/if}
      {if !empty($form.error.resizes.$index.size.invalid)}
      <div class="giError">
	{g->text text="You must enter a number (greater than zero)"}
      </div>
      {/if}
      {counter}
    {/foreach}
  {else}
  <b>
    {g->text text="There are no graphics toolkits enabled that support this type of photo, so we cannot create or modify resized versions."}
    {if $user.isAdmin}
      <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=core.AdminPlugins"}">
	{g->text text="site admin"}
      </a>
    {/if}
  </b>
  {/if}
</div>

{* Include our extra ItemEditOptions *}
{foreach from=$ItemEdit.options item=option}
  {include file="gallery:`$option.file`" l10Domain=$option.l10Domain}
{/foreach}

<div class="gbBlock gcBackground1">
  <input type="hidden" name="{g->formVar var="mode"}" value="editSizes"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][undo]"}" value="{g->text text="Reset"}"/>
</div>
