{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Random Highlight"} </h3>

  <input type="checkbox" id="RandomHighlight_cb"
   name="{g->formVar var="form[RandomHighlightOption][isRandomHighlight]"}"
   {if $form.RandomHighlightOption.isRandomHighlight}checked="checked"{/if}/>
  <label for="RandomHighlight_cb">
    {g->text text="Activate random highlight for this album"}
  </label>
</div>
