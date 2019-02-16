{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<script type="text/javascript">
  // <![CDATA[
  function SetSizeLimitOption_toggleXY() {ldelim}
    var frm = document.getElementById('itemAdminForm');
    frm.elements["{g->formVar var="form[SizeLimitOption][dimensions][width]"}"].disabled =
      !frm.elements["{g->formVar var="form[SizeLimitOption][dimensionChoice]"}"][1].checked;
    frm.elements["{g->formVar var="form[SizeLimitOption][dimensions][height]"}"].disabled =
      !frm.elements["{g->formVar var="form[SizeLimitOption][dimensionChoice]"}"][1].checked;
  {rdelim}
  function SetSizeLimitOption_toggleSize() {ldelim}
    var frm = document.getElementById('itemAdminForm');
    frm.elements["{g->formVar var="form[SizeLimitOption][filesize]"}"].disabled =
     !frm.elements["{g->formVar var="form[SizeLimitOption][sizeChoice]"}"][1].checked;
  {rdelim}
  // ]]>
</script>

<div class="gbBlock">
  <h3> {g->text text="Define picture size limit"} </h3>

  <div style="margin: 0.5em 0">
    <div style="font-weight: bold">
      {g->text text="Maximum dimensions of full sized images"}
    </div>
    <input type="radio" id="SizeLimit_DimNone" onclick="SetSizeLimitOption_toggleXY()"
	   name="{g->formVar var="form[SizeLimitOption][dimensionChoice]"}" value="unlimited"
     {if $SizeLimitOption.dimensionChoice == "unlimited"}checked="checked"{/if}/>
    <label for="SizeLimit_DimNone">
      {g->text text="No Limits"}
    </label>
    <br/>
    <input type="radio" onclick="SetSizeLimitOption_toggleXY()"
	   name="{g->formVar var="form[SizeLimitOption][dimensionChoice]"}" value="explicit"
     {if $SizeLimitOption.dimensionChoice == "explicit"}checked="checked"{/if}/>
    {g->dimensions formVar="form[SizeLimitOption][dimensions]"
		   width=$SizeLimitOption.width height=$SizeLimitOption.height}

    {if $SizeLimitOption.dimensionChoice == "unlimited"}
    <script type="text/javascript">
      var frm = document.getElementById('itemAdminForm');
      frm.elements["{g->formVar var="form[SizeLimitOption][dimensions][width]"}"].disabled = true;
      frm.elements["{g->formVar var="form[SizeLimitOption][dimensions][height]"}"].disabled = true;
    </script>
    {/if}

    {if !empty($form.error.SizeLimitOption.dim.missing)}
    <div class="giError">
      {g->text text="You must specify at least one of the dimensions"}
    </div>
    {/if}
  </div>

  <div style="margin: 0.5em 0">
    <div style="font-weight: bold">
      {g->text text="Maximum file size of full sized images in kilobytes"}
    </div>
    <input type="radio" id="SizeLimit_SizeNone" onclick="SetSizeLimitOption_toggleSize()"
	   name="{g->formVar var="form[SizeLimitOption][sizeChoice]"}" value="unlimited"
     {if $SizeLimitOption.sizeChoice == "unlimited"}checked="checked"{/if}/>
    <label for="SizeLimit_SizeNone">
      {g->text text="No Limits"}
    </label>
    <br/>
    <input type="radio" onclick="SetSizeLimitOption_toggleSize()"
	   name="{g->formVar var="form[SizeLimitOption][sizeChoice]"}" value="explicit"
     {if $SizeLimitOption.sizeChoice == "explicit"}checked="checked"{/if}/>
    <input type="text" size="7" maxlength="6"
	   name="{g->formVar var="form[SizeLimitOption][filesize]"}"
	   value="{$SizeLimitOption.filesize}"
     {if $SizeLimitOption.sizeChoice != "explicit"}disabled="disabled"{/if}/>

    {if !empty($form.error.SizeLimitOption.filesize.invalid)}
    <div class="giError">
      {g->text text="You must enter a number (greater than zero)"}
    </div>
    {/if}
  </div>

  <input type="checkbox" id="SizeLimit_KeepOriginal"
	 name="{g->formVar var="form[SizeLimitOption][keepOriginal]"}"
   {if $SizeLimitOption.keepOriginal} checked="checked"{/if}/>
  <label for="SizeLimit_KeepOriginal">
    {g->text text="Keep original image?"}
  </label>
  <br/>
  <input type="checkbox" id="SizeLimit_ApplyToDescendents"
	 name="{g->formVar var="form[SizeLimitOption][applyToDescendents]"}"/>
  <label for="SizeLimit_ApplyToDescendents">
    {g->text text="Check here to apply size limits to the pictures in this album and all subalbums"}
  </label>
  <blockquote><p>
    {g->text text="Checking this option will rebuild pictures according to appropriate limits"}
  </p></blockquote>
  {g->changeInDescendents module="sizelimit" text="Use these size limits in all subalbums"}
  <blockquote><p>
    {g->text text="Checking this option will set same picture size limits in all subalbums"}
  </p></blockquote>
</div>
