{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if empty($item)} {assign var=item value=$theme.item} {/if}

{* Load up the EXIF data *}
{g->callback type="exif.LoadExifInfo" itemId=$item.id}

{if !empty($block.exif.LoadExifInfo.exifData)}
<div class="{$class}">
  <h3> {g->text text="Photo Properties"} </h3>

  {if isset($block.exif.LoadExifInfo.mode)}
  <div>
    {if ($block.exif.LoadExifInfo.mode == 'summary')}
      {g->text text="summary"}
    {else}
      <a href="{g->url arg1="controller=exif.SwitchDetailMode" arg2="mode=summary" arg3="return=true"}">
	{g->text text="summary"}
      </a>
    {/if}

    {if ($block.exif.LoadExifInfo.mode == 'detailed')}
      {g->text text="details"}
    {else}
      <a href="{g->url arg1="controller=exif.SwitchDetailMode" arg2="mode=detailed" arg3="return=true"}">
	{g->text text="details"}
      </a>
    {/if}
  </div>
  {/if}

  {if !empty($block.exif.LoadExifInfo.exifData)}
  <table class="gbDataTable">
    {section name=outer loop=$block.exif.LoadExifInfo.exifData step=2}
    <tr>
      {section name=inner loop=$block.exif.LoadExifInfo.exifData start=$smarty.section.outer.index max=2}
      <td class="gbEven">
	{g->text text=$block.exif.LoadExifInfo.exifData[inner].title}
      </td>
      <td class="gbOdd">
	{$block.exif.LoadExifInfo.exifData[inner].value}
      </td>
      {/section}
    </tr>
    {/section}
  </table>
  {/if}
</div>
{/if}
