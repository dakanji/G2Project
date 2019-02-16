{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Edit a group"} </h2>
</div>

<div class="gbBlock">
  <h4>
    {g->text text="Group Name"}
    <span class="giSubtitle"> {g->text text="(required)"} </span>
  </h4>

  <input type="text" id="giFormGroupname"
   name="{g->formVar var="form[groupName]"}" value="{$form.groupName}"/>
  <input type="hidden" name="{g->formVar var="groupId"}" value="{$AdminEditGroup.group.id}"/>
  <script type="text/javascript">
    document.getElementById('siteAdminForm')['{g->formVar var="form[groupName]"}'].focus();
  </script>

  {if isset($form.error.groupName.missing)}
  <div class="giError">
    {g->text text="You must enter a group name"}
  </div>
  {/if}
  {if isset($form.error.groupName.exists)}
  <div class="giError">
    {g->text text="Group '%s' already exists" arg1=$form.groupName}
  </div>
  {/if}
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][undo]"}" value="{g->text text="Reset"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>
