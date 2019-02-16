{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if !empty($navigationLinks)}
<div class="{$class}">
  <h3> {g->text text="Navigation"} </h3>
  <ul>
    {foreach from=$navigationLinks item=link}
      <li>
        <a href="{$link.url}">
          {$link.name}
        </a>
      </li>
    {/foreach}
  </ul>
</div>
{/if}
