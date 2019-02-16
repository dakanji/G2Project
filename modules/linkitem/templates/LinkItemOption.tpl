{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Link"} </h3>

  <h4> {g->text text="URL:"} </h4>
  <input type="text" size="60"
   name="{g->formVar var="form[LinkItemOption][link]"}" value="{$form.LinkItemOption.link}"/>

  {if isset($form.LinkItemOption.error.link.missing)}
  <div class="giError">
    {g->text text="Missing URL"}
  </div>
  {/if}
</div>
