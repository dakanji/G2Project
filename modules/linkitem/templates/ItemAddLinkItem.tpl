{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <table class="gbDataTable"><tr><td>
    <input type="radio" id="rbAlbum"{if $form.linkType=='album'} checked="checked"{/if}
     name="{g->formVar var="form[linkType]"}" value="album"}
  </td><td>
    <label for="rbAlbum">
      <b> {g->text text="Link to Album:"} </b> &nbsp;
    </label>
  </td><td>
    <select name="{g->formVar var="form[linkedAlbumId]"}">
      {foreach from=$ItemAddLinkItem.albumTree item=album}
	<option value="{$album.data.id}"{if $album.data.id == $form.linkedAlbumId}
	 selected="selected"{/if}>
	  {"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	  {$album.data.title|default:$album.data.pathComponent}
	</option>
      {/foreach}
    </select>

    {if isset($form.error.linkedAlbumId.missing)}
    <div class="giError">
      {g->text text="You must enter an album id"}
    </div>
    {/if}
    {if isset($form.error.linkedAlbumId.invalid)}
    <div class="giError">
      {g->text text="Invalid album id"}
    </div>
    {/if}
  </td></tr><tr><td>
    <input type="radio" id="rbUrl"{if $form.linkType=='url'} checked="checked"{/if}
     name="{g->formVar var="form[linkType]"}" value="url"}
  </td><td>
    <label for="rbUrl">
      <b> {g->text text="Link to External URL:"} </b>
    </label>
  </td><td>
    <input type="text" size="60"
     name="{g->formVar var="form[linkUrl]"}" value="{$form.linkUrl}"/>
    {if isset($form.error.linkUrl.missing)}
    <div class="giError">
      {g->text text="You must enter an URL"}
    </div>
    {/if}
  </td></tr></table>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][addLinkItem]"}" value="{g->text text="Add Link"}"/>
</div>
