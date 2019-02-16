{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="imageblock.LoadImageBlock"
	     blocks=$blocks|default:null repeatBlock=$repeatBlock|default:null
	     maxSize=$maxSize|default:null itemId=$itemId|default:null
	     link=$link|default:null linkTarget=$linkTarget|default:null
	     useDefaults=$useDefaults|default:true showHeading=$showHeading|default:true
	     showTitle=$showTitle|default:true showDate=$showDate|default:true
	     showViews=$showViews|default:false showOwner=$showOwner|default:false}

{if !empty($ImageBlockData)}
<div class="{$class}">
  {include file="gallery:modules/imageblock/templates/ImageBlock.tpl"}
</div>
{/if}
