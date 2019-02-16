{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Add Items"} </h2>
</div>

{if (!$ItemAdd.hasToolkit)}
<div class="gbBlock giWarning">
  {g->text text="You don't have any Graphics Toolkit activated that can handle JPEG images.  If you add images, you will probably not have any thumbnails."}
  {capture name="url"}
    {g->url arg1="view=core.SiteAdmin" arg2="subView=core.AdminPlugins"}
  {/capture}
  {if $ItemAdd.isAdmin}
    {g->text text="Visit the <a href=\"%s\">Modules</a> page to activate a Graphics Toolkit." arg1=$smarty.capture.url}
  {/if}
</div>
{/if}

<div class="gbTabBar">
  {foreach from=$ItemAdd.plugins item=plugin}
    {if $plugin.isSelected}
      <span class="giSelected o"><span>
	{$plugin.title}
      </span></span>
    {else}
      <span class="o"><span>
	<a href="{g->url arg1="view=core.ItemAdmin" arg2="subView=core.ItemAdd"
	 arg3="itemId=`$ItemAdmin.item.id`" arg4="addPlugin=`$plugin.id`"}">{$plugin.title}</a>
      </span></span>
    {/if}
  {/foreach}
</div>

<input type="hidden" name="{g->formVar var="addPlugin"}" value="{$ItemAdd.addPlugin}"/>

{include file="gallery:`$ItemAdd.pluginFile`" l10Domain=$ItemAdd.pluginL10Domain}
