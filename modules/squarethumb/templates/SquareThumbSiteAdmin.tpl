{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Square Thumbnails"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <table class="gbDataTable"><tr>
    <td>
      <input type="radio" id="rbCrop" name="{g->formVar var="form[mode]"}" value="crop"
       {if $form.mode=="crop"}checked="checked"{/if}/>
    </td>
    <td>
      <label for="rbCrop">
	{g->text text="Crop images to square"}
      </label>
    </td>
  </tr><tr>
    <td>
      <input type="radio" id="rbFit" name="{g->formVar var="form[mode]"}" value="fit"
       {if $form.mode=="fit"}checked="checked"{/if}/>
    </td>
    <td>
      <label for="rbFit">
	{g->text text="Fit images to square, padding with background color:"}
      </label>
      <input type="text" size="6" name="{g->formVar var="form[color]"}" value="{$form.color}"/>

      {if isset($form.error.color.missing)}
      <div class="giError">
	{g->text text="Color value missing"}
      </div>
      {/if}
      {if isset($form.error.color.invalid)}
      <span class="giError">
	{g->text text="Color value invalid"}
      </span>
      {/if}
    </td>
  </tr></table>

  <p class="giDescription">
    {g->text text="Background color is in RGB hex format; 000000 is black, FFFFFF is white."}
  </p>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
