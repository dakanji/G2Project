{*
 * $Revision: 15671 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<form action="{g->url}" method="post" id="userAdminForm"
      enctype="{$UserAdmin.enctype|default:"application/x-www-form-urlencoded"}">
  <div>
    {g->hiddenFormVars}
    <input type="hidden" name="{g->formVar var="controller"}" value="{$controller}"/>
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
  </div>

  <table width="100%" cellspacing="0" cellpadding="0">
    <tr valign="top">
      <td id="gsSidebarCol"><div id="gsSidebar" class="gcBorder1">
	<div class="gbBlock">
	  <h2> {g->text text="User Options"} </h2>
	  <ul>
	    {foreach from=$UserAdmin.subViewChoices item=choice}
	      <li class="{g->linkId urlParams=$choice}">
	      {if ($UserAdmin.subViewName == $choice.view)}
		{$choice.name}
	      {else}
		<a href="{g->url arg1="view=core.UserAdmin" arg2="subView=`$choice.view`"}">
		  {$choice.name}
		</a>
	      {/if}
	      </li>
	    {/foreach}
	  </ul>
	</div>

	{g->block type="core.NavigationLinks" class="gbBlock"
		  navigationLinks=$UserAdmin.navigationLinks}
      </div></td>

      <td>
	<div id="gsContent" class="gcBorder1">
	  {include file="gallery:`$UserAdmin.viewBodyFile`" l10Domain=$UserAdmin.viewL10Domain}
	</div>
      </td>
    </tr>
  </table>
</form>
