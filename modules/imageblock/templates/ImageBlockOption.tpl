{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Image Block"} </h3>

  <input type="checkbox" id="ImageBlockOption_setDisabled"
   name="{g->formVar var="form[ImageBlockOption][setDisabled]"}"
   {if $form.ImageBlockOption.setDisabled}checked="checked"{/if}/>
  <label for="ImageBlockOption_setDisabled">
    {g->text text="Prevent this album from being displayed in the Image Block"}
  </label>
  <br/>

  <input type="checkbox" id="ImageBlockOption_setRecursive" checked="checked"
   name="{g->formVar var="form[ImageBlockOption][setRecursive]"}"/>
  <label for="ImageBlockOption_setRecursive">
    {g->text text="Apply ImageBlock settings to sub-albums"}
  </label>
</div>
