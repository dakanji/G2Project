{*
 * $Revision: 15949 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Move an Item"} </h2>
</div>

{if isset($status.moved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text one="Successfully moved %d item" many="Successfully moved %d items"
	   count=$status.moved.count arg1=$status.moved.count}
</h2></div>
{/if}
{if !empty($form.error)}
<div class="gbBlock"><h2 class="giError">
  {g->text text="There was a problem processing your request."}
</h2></div>
{/if}

<div class="gbBlock">
{if empty($ItemMove.peers)}
  <p class="giDescription">
    {g->text text="This album contains no items to move."}
  </p>
{else}
  <h3> {g->text text="Source"} </h3>

  <p class="giDescription">
    {g->text text="Choose the items you want to move"}
    {if ($ItemMove.numPages > 1) }
      {g->text text="(page %d of %d)" arg1=$ItemMove.page arg2=$ItemMove.numPages}
      <br/>
      {g->text text="Items selected here will remain selected when moving between pages."}
      {if !empty($ItemMove.selectedIds)}
	<br/>
	{g->text one="One item selected on other pages." many="%d items selected on other pages."
		 count=$ItemMove.selectedIdCount arg1=$ItemMove.selectedIdCount}
      {/if}
    {/if}
  </p>

  <script type="text/javascript">
    //<![CDATA[
    // Validation code.  This Javascript snippet validates the source and destination information
    // to make sure that you don't attempt to do something that you shouldn't, ie, it will help
    // you to avoid the situation where you try to move an item into an album where you don't have
    // the right permissions.  This is only a hint to the UI -- we perform the same permission
    // checks on the server side (so circumventing or disabling this javascript won't allow you to
    // do something that you don't have permission to do anyway).

    // The user can add data items to these albums
    permission = new Array();
    permission['addDataItem'] = new Array();
    {foreach from=$ItemMove.albumIds.addDataItem item=id}
      permission['addDataItem'][{$id}] = 1;
    {/foreach}

    // The user can add album items to these albums
    permission['addAlbumItem'] = new Array();
    {foreach from=$ItemMove.albumIds.addAlbumItem item=id}
      permission['addAlbumItem'][{$id}] = 1;
    {/foreach}

    // Check what the destination album accepts.  If it can handle data items and
    // album items then we're done.	 Else, scan the selected items and make sure that
    // we haven't selected something that we can't handle.	If we have, then remove
    // the selection and alert the user.
    function checkPermissions(form, quiet) {ldelim}
      destinationId = form.elements['{g->formVar var="form[destination]"}'].value;
      if (permission['addDataItem'][destinationId] && permission['addAlbumItem'][destinationId]) {ldelim}
	{foreach from=$ItemMove.peerTypes.album key=id item=unused}
	  form.elements['{g->formVar var="form[selectedIds][$id]"}'].disabled = 0;
	{/foreach}
	{foreach from=$ItemMove.peerTypes.data key=id item=unused}
	  form.elements['{g->formVar var="form[selectedIds][$id]"}'].disabled = 0;
	{/foreach}
	return;
      {rdelim}

      changed = 0;
      if (permission['addDataItem'][destinationId]) {ldelim}
	{foreach from=$ItemMove.peerTypes.album key=id item=unused}
	  if (form.elements['{g->formVar var="form[selectedIds][$id]"}'].checked) {ldelim}
	    form.elements['{g->formVar var="form[selectedIds][$id]"}'].checked = 0;
	    changed = 1;
	  {rdelim}
	  form.elements['{g->formVar var="form[selectedIds][$id]"}'].disabled = 1;
	{/foreach}
	{foreach from=$ItemMove.peerTypes.data key=id item=unused}
	  form.elements['{g->formVar var="form[selectedIds][$id]"}'].disabled = 0;
	{/foreach}
	if (changed && !quiet) {ldelim}
	  alert("{g->text text="The destination you chose does not accept sub-albums, so all sub-albums have been deselected."}");
	{rdelim}
      {rdelim} else {ldelim}
	{foreach from=$ItemMove.peerTypes.data key=id item=unused}
	  if (form.elements['{g->formVar var="form[selectedIds][$id]"}'].checked) {ldelim}
	    form.elements['{g->formVar var="form[selectedIds][$id]"}'].checked = 0;
	    changed = 1;
	  {rdelim}
	  form.elements['{g->formVar var="form[selectedIds][$id]"}'].disabled = 1;
	{/foreach}
	{foreach from=$ItemMove.peerTypes.album key=id item=unused}
	  form.elements['{g->formVar var="form[selectedIds][$id]"}'].disabled = 0;
	{/foreach}
	if (changed && !quiet) {ldelim}
	  alert("{g->text text="The destination you chose only accepts sub-albums, so all non-albums have been deselected."}");
	{rdelim}
      {rdelim}
    {rdelim}

    function setCheck(val) {ldelim}
      var frm = document.getElementById('itemAdminForm');
      {foreach from=$ItemMove.peers item=peer}
	frm.elements['g2_form[selectedIds][{$peer.id}]'].checked = val;
      {/foreach}
    {rdelim}
    function invertCheck(val) {ldelim}
      var frm = document.getElementById('itemAdminForm');
      {foreach from=$ItemMove.peers item=peer}
	frm.elements['g2_form[selectedIds][{$peer.id}]'].checked = !frm.elements['g2_form[selectedIds][{$peer.id}]'].checked;
      {/foreach}
    {rdelim}
    //]]>
  </script>

  <table>
    <colgroup width="60"/>
    {foreach from=$ItemMove.peers item=peer}
    {assign var="peerItemId" value=$peer.id}
    <tr>
      <td align="center">
	{if isset($peer.thumbnail)}
	  <a id="thumb_{$peerItemId}" href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$peerItemId`"}">
	    {g->image item=$peer image=$peer.thumbnail maxSize=50 class="giThumbnail"}
	  </a>
	{else}
	  &nbsp;
	{/if}
      </td><td>
	<input type="checkbox" id="cb_{$peerItemId}"{if $peer.selected} checked="checked"{/if}
	 name="{g->formVar var="form[selectedIds][$peerItemId]"}"/>
      </td><td>
	<label for="cb_{$peerItemId}">
	  {$peer.title|default:$peer.pathComponent}
	</label>
	<i>
	  {if isset($ItemMove.peerTypes.data.$peerItemId)}
	    {g->text text="(data)"}
	  {/if}
	  {if isset($ItemMove.peerTypes.album.$peerItemId)}
	    {if isset($ItemMove.peerDescendentCounts.$peerItemId)}
	      {g->text one="(album containing %d item)" many="(album containing %d items)"
		       count=$ItemMove.peerDescendentCounts.$peerItemId
		       arg1=$ItemMove.peerDescendentCounts.$peerItemId}
	    {else}
	      {g->text text="(empty album)"}
	    {/if}
	  {/if}
	</i>

	{if !empty($form.error.source.$peerItemId.permission.delete)}
	<div class="giError">
	  {g->text text="You are not allowed to move this item away from here."}<br/>
	</div>
	{/if}
	{if !empty($form.error.source.$peerItemId.permission.addAlbumItem)}
	<div class="giError">
	  {g->text text="You are not allowed to move an album to the chosen destination."}<br/>
	</div>
	{/if}
	{if !empty($form.error.source.$peerItemId.permission.addDataItem)}
	<div class="giError">
	  {g->text text="You are not allowed to move an item to the chosen destination."}<br/>
	</div>
	{/if}
	{if !empty($form.error.source.$peerItemId.selfMove)}
	<div class="giError">
	  {g->text text="You cannot move an album into its own subtree."}<br/>
	</div>
	{/if}
      </td>
    </tr>
    {/foreach}
    <script type="text/javascript">
      //<![CDATA[
      {foreach from=$ItemMove.peers item=peer}
      {if isset($peer.resize)}
      {* force and alt/longdesc parameter here so that we avoid issues with single quotes in the title/description *}
      new YAHOO.widget.Tooltip("gTooltip", {ldelim}
          context: "thumb_{$peer.id}", text: '{g->image item=$peer image=$peer.resize class="giThumbnail" maxSize=500 alt="" longdesc=""}',
          showDelay: 250 {rdelim});
      {elseif isset($peer.thumbnail)}
      new YAHOO.widget.Tooltip("gTooltip", {ldelim}
          context: "thumb_{$peer.id}", text: '{g->image item=$peer image=$peer.thumbnail class="giThumbnail" alt="" longdesc=""}',
          showDelay: 250 {rdelim});
      {/if}
      {/foreach}
      //]]>
    </script>
  </table>
  <input type="hidden" name="{g->formVar var="page"}" value="{$ItemMove.page}"/>
  <input type="hidden" name="{g->formVar var="form[numPerPage]"}" value="{$ItemMove.numPerPage}"/>
  {foreach from=$ItemMove.selectedIds item=selectedId}
    <input type="hidden" name="{g->formVar var="form[selectedIds][$selectedId]"}" value="on"/>
  {/foreach}

  <input type="button" class="inputTypeButton" onclick="setCheck(1)"
   name="{g->formVar var="form[action][checkall]"}" value="{g->text text="Check All"}"/>
  <input type="button" class="inputTypeButton" onclick="setCheck(0)"
   name="{g->formVar var="form[action][checknone]"}" value="{g->text text="Check None"}"/>
  <input type="button" class="inputTypeButton" onclick="invertCheck()"
   name="{g->formVar var="form[action][invert]"}" value="{g->text text="Invert"}"/>

  {if ($ItemMove.page > 1)}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][previous]"}" value="{g->text text="Previous Page"}"/>
  {/if}
  {if ($ItemMove.page < $ItemMove.numPages)}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][next]"}" value="{g->text text="Next Page"}"/>
  {/if}
</div>

<div class="gbBlock">
  <h3> {g->text text="Destination"} </h3>

  <p class="giDescription">
    {g->text text="Choose a new album for them"}
  </p>

  <select name="{g->formVar var="form[destination]"}"
   onchange="checkPermissions(this.form)">
    {foreach from=$ItemMove.albumTree item=album}
      <option value="{$album.data.id}"
	      {if ($album.data.id == $form.destination)}selected="selected"{/if}>
	{"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	{$album.data.title|default:$album.data.pathComponent}
      </option>
    {/foreach}
  </select>

  {if !empty($form.error.destination.permission)}
  <div class="giError">
    {g->text text="You don't have the permission to add items in this album"}
  </div>
  {/if}
  {if !empty($form.error.destination.empty)}
  <div class="giError">
    {g->text text="No destination chosen"}
  </div>
  {/if}
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][move]"}" value="{g->text text="Move"}"/>
  {if $ItemMove.canCancel}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
  {/if}
{/if}
</div>
