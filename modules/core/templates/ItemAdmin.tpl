{*
 * $Revision: 15905 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<form action="{g->url}" method="post" enctype="{$ItemAdmin.enctype}" id="itemAdminForm">
  <div>
    {g->hiddenFormVars}
    {if !empty($controller)}
    <input type="hidden" name="{g->formVar var="controller"}" value="{$controller}"/>
    {/if}
    {if !empty($form.formName)}
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
    {/if}
    <input type="hidden" name="{g->formVar var="itemId"}" value="{$ItemAdmin.item.id}"/>
  </div>

  <table width="100%" cellspacing="0" cellpadding="0">
    <tr valign="top">
    <td id="gsSidebarCol"><div id="gsSidebar" class="gcBorder1">
      {if $ItemAdmin.item.parentId or !empty($ItemAdmin.thumbnail)}
      <div class="gbBlock">
	{if empty($ItemAdmin.thumbnail)}
	  {g->text text="No Thumbnail"}
	{else}
	  {g->image item=$ItemAdmin.item image=$ItemAdmin.thumbnail maxSize=130}
	{/if}
	<h3> {$ItemAdmin.item.title|markup} </h3>
      </div>
      {/if}

      <div class="gbBlock">
	<h2> {g->text text="Options"} </h2>
	<ul>
	  {foreach from=$ItemAdmin.subViewChoices key=choiceName item=choiceParams}
	    <li class="{g->linkId urlParams=$choiceParams}">
	    {if isset($choiceParams.active)}
	      {$choiceName}
	    {else}
	      <a href="{g->url params=$choiceParams}"> {$choiceName} </a>
	    {/if}
	    </li>
	  {/foreach}
	</ul>
      </div>

      {g->block type="core.NavigationLinks" class="gbBlock"
		navigationLinks=$ItemAdmin.navigationLinks}
    </div></td>

    <td>
      <div id="gsContent" class="gcBorder1">
	{include file="gallery:`$ItemAdmin.viewBodyFile`" l10Domain=$ItemAdmin.viewL10Domain}
      </div>
    </td>
  </tr></table>
</form>
