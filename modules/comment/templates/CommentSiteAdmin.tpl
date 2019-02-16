{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Comments Settings"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <table class="gbDataTable"><tr>
    <td>
      <label for="cbLatest">
	{g->text text="Show link for Latest Comments:"}
      </label>
    </td><td>
      <input type="checkbox" id="cbLatest"{if $form.latest} checked="checked"{/if}
       name="{g->formVar var="form[latest]"}"/>
    </td>
  </tr><tr>
    <td>
      {g->text text="Number of comments on Latest Comments page:"}
    </td><td>
      <input type="text" size="5" name="{g->formVar var="form[show]"}" value="{$form.show}"/>

      {if isset($form.error.show)}
      <div class="giError">
	{g->text text="Invalid value"}
      </div>
      {/if}
    </td>
  </tr></table>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
