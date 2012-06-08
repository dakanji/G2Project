{*
 * $Revision: 15949 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Rotate or Flip an Item"} </h2>
</div>

{if isset($status.rotated)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text one="Successfully rotated %d item" many="Successfully rotated %d items"
	   count=$status.rotated.count arg1=$status.rotated.count}
</h2></div>
{/if}
{if !empty($form.error)}
<div class="gbBlock"><h2 class="giError">
  {g->text text="There was a problem processing your request."}
</h2></div>
{/if}

<div class="gbBlock">
{if empty($ItemRotate.peers)}
  <p class="giDescription">
    {g->text text="This album contains no items to rotate."}
  </p>
{else}
  <h3> {g->text text="Source"} </h3>

  <p class="giDescription">
    {g->text text="Choose items to rotate"}
    {if ($ItemRotate.numPages > 1) }
      {g->text text="(page %d of %d)" arg1=$ItemRotate.page arg2=$ItemRotate.numPages}
      <br/>
      {g->text text="Items selected here will remain selected when moving between pages."}
      {if !empty($ItemRotate.selectedIds)}
	<br/>
	{g->text one="One item selected on other pages." many="%d items selected on other pages."
		 count=$ItemRotate.selectedIdCount arg1=$ItemRotate.selectedIdCount}
      {/if}
    {/if}
  </p>

  <script type="text/javascript">
    //<![CDATA[
    function setCheck(val) {ldelim}
      var frm = document.getElementById('itemAdminForm');
      {foreach from=$ItemRotate.peers item=peer}
	frm.elements['g2_form[selectedIds][{$peer.id}]'].checked = val;
      {/foreach}
    {rdelim}
    function invertCheck(val) {ldelim}
      var frm = document.getElementById('itemAdminForm');
      {foreach from=$ItemRotate.peers item=peer}
	frm.elements['g2_form[selectedIds][{$peer.id}]'].checked = !frm.elements['g2_form[selectedIds][{$peer.id}]'].checked;
      {/foreach}
    {rdelim}
    //]]>
  </script>

  <table>
    <colgroup width="60"/>
    {foreach from=$ItemRotate.peers item=peer}
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
	  {if isset($ItemRotate.peerTypes.data.$peerItemId)}
	    {g->text text="(data)"}
	  {/if}
	  {if isset($ItemRotate.peerTypes.album.$peerItemId)}
	    {if isset($ItemRotate.peerDescendentCounts.$peerItemId)}
	      {g->text one="(album containing %d item)" many="(album containing %d items)"
		       count=$ItemRotate.peerDescendentCounts.$peerItemId
		       arg1=$ItemRotate.peerDescendentCounts.$peerItemId}
	    {else}
	      {g->text text="(empty album)"}
	    {/if}
	  {/if}
	</i>
      </td>
    </tr>
    {/foreach}
    <script type="text/javascript">
      //<![CDATA[
      {foreach from=$ItemRotate.peers item=peer}
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
  <input type="hidden" name="{g->formVar var="page"}" value="{$ItemRotate.page}"/>
  <input type="hidden" name="{g->formVar var="form[numPerPage]"}" value="{$ItemRotate.numPerPage}"/>
  {foreach from=$ItemRotate.selectedIds item=selectedId}
    <input type="hidden" name="{g->formVar var="form[selectedIds][$selectedId]"}" value="on"/>
  {/foreach}

  <input type="button" class="inputTypeButton" onclick="setCheck(1)"
   name="{g->formVar var="form[action][checkall]"}" value="{g->text text="Check All"}"/>
  <input type="button" class="inputTypeButton" onclick="setCheck(0)"
   name="{g->formVar var="form[action][checknone]"}" value="{g->text text="Check None"}"/>
  <input type="button" class="inputTypeButton" onclick="invertCheck()"
   name="{g->formVar var="form[action][invert]"}" value="{g->text text="Invert"}"/>

  {if ($ItemRotate.page > 1)}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][previous]"}" value="{g->text text="Previous Page"}"/>
  {/if}
  {if ($ItemRotate.page < $ItemRotate.numPages)}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][next]"}" value="{g->text text="Next Page"}"/>
  {/if}
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][ccw]"}" value="{g->text text="CC 90&deg;"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][flip]"}" value="{g->text text="180&deg;"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cw]"}" value="{g->text text="C 90&deg;"}"/>

  {if $ItemRotate.canCancel}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
  {/if}
{/if}
</div>
