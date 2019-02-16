{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <p class="giDescription">
    {g->text text="These are album-specific custom field settings. Common fields are available on all items; Album and Photo fields can be assigned only to items of the appropriate type."}
  </p>
  <p class="giDescription">
    <a href="{g->url arg1="controller=customfield.CustomFieldItemAdmin"
                     arg2="cfAdmin=-1" arg3="itemId=`$form.itemId`" arg4="return=true"}">
      {g->text text="Edit custom field values for this album"}
    </a>
    <br/>
    <a href="{g->url arg1="controller=customfield.CustomFieldItemAdmin"
                     arg2="cfAdmin=-2" arg3="itemId=`$form.itemId`" arg4="return=true"}"
       onclick="return confirm('{g->text text="WARNING: Values for all fields that do not also exist in the global settings will be deleted for this album and its items!"}')">
      {g->text text="Revert to global custom field settings for this album"}
    </a>
  </p>
</div>

<script type="text/javascript">
  // <![CDATA[
  var removeWarning = '{g->text text="WARNING: Values for this custom field in this album will be deleted!"}';
  var albumWarning = '{g->text text="WARNING: Values for this custom field in this album will be deleted!"}';
  var photoWarning = '{g->text text="WARNING: Values for this custom field on non-photo items in this album will be deleted!"}';
  // ]]>
</script>

<input type="hidden" name="{g->formVar var="form[cfAdmin]"}" value="1"/>
{include file="gallery:modules/customfield/templates/Admin.tpl"}
