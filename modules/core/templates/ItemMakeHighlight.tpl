{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Album Highlight"} </h2>
</div>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="You can make this item the thumbnail for its parent or any ancestor album."}
  </p>
  <p>
    {g->text text="Highlight for:"}
    <select name="{g->formVar var="form[parentId]"}">
    {counter start=0 assign=count}
    {foreach from=$ItemMakeHighlight.parentList item=parent}
      <option value="{$parent.id}"> {$parent.title|markup|indent:$count:"-- "} </option>
      {counter assign=count}
    {/foreach}
    </select>
  </p>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][makeHighlight]"}" value="{g->text text="Highlight"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>
