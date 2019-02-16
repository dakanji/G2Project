{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Custom Fields"} </h2>
</div>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="These are the global settings for custom fields. They can be overridden at the album level. Common fields are available on all Gallery items; Album and Photo fields can be assigned only to items of the appropriate type."}
  </p>
</div>

<script type="text/javascript">
  // <![CDATA[
  var removeWarning = '{g->text text="WARNING: All values for this custom field will be deleted! (Except in albums with album-specific settings)"}';
  var albumWarning = '{g->text text="WARNING: Values for this custom field on non-album items will be deleted! (Except in albums with album-specific settings)"}';
  var photoWarning = '{g->text text="WARNING: Values for this custom field on non-photo items will be deleted! (Except in albums with album-specific settings)"}';
  // ]]>
</script>

{include file="gallery:modules/customfield/templates/Admin.tpl"}
