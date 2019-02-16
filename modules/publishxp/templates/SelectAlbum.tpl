{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<form action="{g->url}" enctype="application/x-www-form-urlencoded" method="post"
 id="publishXpForm">
  <div>
    <script type="text/javascript">
      // <![CDATA[
      {capture name=url}{g->url arg1="view=publishxp.NewAlbum"}{/capture}
      setSubtitle("{g->text text="Choose an album for new photos, or create a new album." arg1=$smarty.capture.url}");
      setOnBackUrl("{g->url arg1="view=publishxp.SelectAlbum"}");
      setSubmitOnNext(true);
      setButtons(false, true, false);
      // ]]>
    </script>

    {g->hiddenFormVars}
    <input type="hidden" name="{g->formVar var="controller"}" value="publishxp.SelectAlbum"/>
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
    <input type="hidden" name="{g->formVar var="form[action][select]"}" value="1"/>
  </div>

  <div class="gbBlock">
    <div>
      <select id="{g->formVar var="form[albumId]"}" name="{g->formVar var="form[albumId]"}"
	      size="15" width="40" style="width: 100%; margin-top: 10px">
	{foreach from=$SelectAlbum.albumTree item=album}
	  <option value="{$album.data.id}"
    	    {if ($album.data.id == $form.albumId)}selected="selected"{/if}>
	    {"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	    {$album.data.title|default:$album.data.pathComponent}
	  </option>
	{/foreach}
      </select>
      <script type="text/javascript">document.getElementById('publishXpForm')['{g->formVar
       var="form[albumId]"}'].focus();</script>
      <input type="submit" class="inputTypeSubmit"
       name="{g->formVar var="form[action][newAlbum]"}" value="{g->text text="New Album"}"/>

      {if isset($form.error.albumId.missing)}
      <div class="giError">
	  {g->text text="You must select an album"}
      </div>
      {/if}
    </div>
  </div>
</form>
