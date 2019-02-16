{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{*
 * Go through each breadcrumb and display it as a link.
 *
 * G2 uses the highlight id to figure out which page to draw when you follow the
 * breadcrumbs back up the album tree.  Don't make the last item a link.
 *}
<div class="{$class}">
  {foreach name=parent from=$theme.parents item=parent}
  {if !$smarty.foreach.parent.last}
  <a href="{g->url params=$parent.urlParams}" class="BreadCrumb-{counter name="BreadCrumb"}">
    {$parent.title|markup:strip|default:$parent.pathComponent}</a>
  {else}
  <a href="{g->url params=$parent.urlParams}" class="BreadCrumb-{counter name="BreadCrumb"}">
    {$parent.title|markup:strip|default:$parent.pathComponent}</a>
  {/if}
  {if isset($separator)} {$separator} {/if}
  {/foreach}

  {if ($theme.pageType == 'admin' || $theme.pageType == 'module')}
  <a href="{g->url arg1="view=core.ShowItem"
		   arg2="itemId=`$theme.item.id`"}" class="BreadCrumb-{counter name="BreadCrumb"}">
     {$theme.item.title|markup:strip|default:$theme.item.pathComponent}</a>
  {else}
  <span class="BreadCrumb-{counter name="BreadCrumb"}">
     {$theme.item.title|markup:strip|default:$theme.item.pathComponent}</span>
  {/if}
</div>
