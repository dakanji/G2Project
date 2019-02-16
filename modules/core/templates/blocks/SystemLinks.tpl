{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{assign var="class" value=$class|replace:"SystemLinks":"SystemLink"}
{assign var="order" value=$order|default:""|split}
{assign var="othersAt" value=$othersAt|default:0}
{assign var="othersAt" value=$othersAt-1}
{assign var="separator" value=$separator|default:""}

{capture name="SystemLinks"}
  {foreach from=$theme.systemLinks key=linkId item=link}
    {if !in_array($linkId, $order)}
    <span class="{$class}">
      <a href="{g->url params=$link.params}">{$link.text}</a>
    </span>
    {$separator}
    {/if}
  {/foreach}
{/capture}

{foreach from=$order key=index item=linkId}
  {if $index==$othersAt}
    {assign var="SystemLinksShown" value=true}
    {$smarty.capture.SystemLinks}
  {/if}
  {if isset($theme.systemLinks[$linkId])}
  <span class="{$class}">
    <a href="{g->url
       params=$theme.systemLinks[$linkId].params}">{$theme.systemLinks[$linkId].text}</a>
  </span>
  {$separator}
  {/if}
{/foreach}
{if !isset($SystemLinksShown)}{$smarty.capture.SystemLinks}{/if}
