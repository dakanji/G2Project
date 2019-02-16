{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  {if isset($form.adminFlag)}
    <p class="giDescription">
      <a href="{g->url arg1="controller=customfield.CustomFieldItemAdmin"
                       arg2="cfAdmin=`$form.adminFlag`" arg3="itemId=`$form.itemId`"
                       arg4="return=true"}">
	{if $form.adminFlag==2}
	  {g->text text="Override global custom field settings for this album"}
	{else}
	  {g->text text="Edit custom field settings for this album"}
	{/if}
      </a>
    </p>
  {/if}

  {if !empty($form.fields)}
  <table class="gbDataTable">
  {foreach from=$form.fields key=field item=value}
    <tr><td>{$field}</td><td>
    {if isset($form.choices[$field])}
      <select name="{g->formVar var="form[fields][$field]"}">
	<option value="">&laquo; {g->text text="No Value"} &raquo;</option>
	{foreach from=$form.choices[$field] item=choice}<option{if
	    $choice==$value} selected="selected"{/if}>{$choice}</option>{/foreach}
      </select>
    {else}
      <input type="text" size="40"
       name="{g->formVar var="form[fields][$field]"}" value="{$value}"/>
    {/if}
    </td></tr>
  {/foreach}
  </table>
  {/if}
</div>

{if !empty($form.fields)}
<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
{/if}
