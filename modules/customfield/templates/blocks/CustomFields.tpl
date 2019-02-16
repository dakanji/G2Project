{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="customfield.LoadCustomFields" itemId=$item.id|default:$theme.item.id}

{if !empty($block.customfield.LoadCustomFields.fields)}
<div class="{$class}">
  <h3> {g->text text="Custom Fields"} </h3>
  <p class="giDescription">
    {foreach from=$block.customfield.LoadCustomFields.fields key=field item=value}
      {$field}: {$value|markup}<br/>
    {/foreach}
  </p>
</div>
{/if}
