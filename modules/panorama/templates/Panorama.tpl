{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gsContent">
  <div class="gbBlock gcBackground1">
    <h2> {g->text text=$Panorama.item.title|default:$Panorama.item.pathComponent} </h2>
  </div>

  <div class="gbBlock">
    {$Panorama.appletHtml}
  </div>
</div>
