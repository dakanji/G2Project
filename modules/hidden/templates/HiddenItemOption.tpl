{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {$form.HiddenItemOption.heading} </h3>

  <p class="giDescription">
    {g->text text="Hidden items are not visible to guest users until the page for the item is accessed directly."}
    {if $form.HiddenItemOption.isAlbum}
      {g->text text="Contents of hidden albums are restricted until the URL for the album is visited."}
    {/if}
    {g->text text="Note that guests need permission to view resizes and/or original items, as visiting the direct URL grants only simple view permission; access to resizes/originals is still controlled by item permissions."}
  </p>

  <input type="checkbox" id="HiddenItem_cb"
   name="{g->formVar var="form[HiddenItemOption][setHidden]"}"
   {if ($form.HiddenItemOption.isHidden)}checked="checked"{/if}/>
  <label for="HiddenItem_cb">
    {g->text text="Hidden"}
  </label>

  {if $form.HiddenItemOption.isAlbum}
    <input type="hidden" name="{g->formVar var="form[HiddenItemOption][progressBar]"}"
     value="{if $form.HiddenItemOption.isHidden}un{/if}hide"/>
  {/if}
</div>
