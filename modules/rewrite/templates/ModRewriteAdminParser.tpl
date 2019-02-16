{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Embedded Setup"} </h3>

  <p class="giDescription">
    {g->text text="For URL Rewrite to work in an embedded environment you need to set up an extra .htaccess file to hold the mod_rewrite rules."}
  </p>

  <table class="gbDataTable"><tr>
    <td>
      {g->text text="Absolute path to the folder of your embedded .htaccess:"}
    </td><td>
      <input type="text" size="60" name="{g->formVar var="form[embeddedHtaccess]"}" value="{$form.embeddedHtaccess}" id="embeddedHtaccess"/><br/>
    </td>
  </tr><tr>
    <td>
      {g->text text="Please enter the Url to your environment. E.g. http://www.mySite.com/myNiceCMS/"}
    </td><td>
      {$AdminParser.host}<input type="text" size="40" name="{g->formVar var="form[embeddedLocation]"}" value="{$form.embeddedLocation}" id="embeddedLocation"/><br/>
    </td>
  </tr></table>
</div>
