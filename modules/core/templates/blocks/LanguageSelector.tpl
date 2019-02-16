{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="core.LoadLanguageSelector"}

<div class="{$class}">
  <form id="LanguageSelector" method="post"
	action="{g->url arg1="controller=core.ChangeLanguage" arg2="return=1"}"><div>
	{g->hiddenFormVars}
    <h3> {g->text text="Language"} </h3>
    <select name="{g->formVar var="language"}" onchange="this.form.submit()" style="direction:ltr">
      {html_options options=$block.core.LanguageSelector.list selected=$block.core.LanguageSelector.language}
    </select>
    <noscript>
      <div style="display: inline">
	<input type="submit" class="inputTypeSubmit" value="{g->text text="Go"}"/>
      </div>
    </noscript>
  </div></form>
</div>
