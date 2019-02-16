{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Download %s" arg1=$AdminRepositoryDownload.pluginName} </h2>
</div>

{if isset($form.error)}
<div class="gbBlock">
  <h2 class="giError">
    {if isset($form.error.nothingSelected)}
      {g->text text="No packages have been selected."}
    {/if}
  </h2>
</div>
{/if}

<script type="text/javascript">
// <![CDATA[
  var allSources = [];
{foreach from=$AdminRepositoryDownload.upgradeData item=item}
  allSources.push('{$item.repository}');
{/foreach}
// ]]>
</script>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Download a package in order to use this plugin.  You can upgrade by choosing a newer version of the package to download.  Language packages are optional, You only need to download the ones that you want to use on your site."}
  </p>
  <h2> {g->text text="Base Packages"} </h2>
  {foreach from=$AdminRepositoryDownload.upgradeData item=item}
  <p>
    {if $item.base.relation == "older"}
      <input type="radio" onchange="showLanguagePacks('{$item.repository}')" name="{g->formVar var="form[base]"}" value="{$item.repository}:{$item.base.newBuild}"/>
      {g->text text="%s: version %s (build %s)" arg1="<b>`$item.repositoryName`</b>" arg2=$item.base.newVersion arg3=$item.base.newBuild}
    {elseif $item.base.relation == "newer"}
      <input type="radio" value="false" disabled="disabled" />
      {g->text text="%s: version %s (build %s) %sdowngrading is not supported!%s" arg1="<b>`$item.repositoryName`</b>" arg2=$item.base.newVersion arg3=$item.base.newBuild arg4="<b>" arg5="</b>"}
    {else}
      <input type="radio" onchange="showLanguagePacks('{$item.repository}')" name="{g->formVar var="form[base]"}" value="{$item.repository}:{$item.base.newBuild}" checked="checked"/>
      {g->text text="%sCurrently Installed%s: version %s (build %s)" arg1="<b>" arg2="</b>" arg3=$item.base.newVersion arg4=$item.base.newBuild}
      {assign var="currentlyInstalled" value=$item.repository}
    {/if}
  </p>
  {/foreach}

  <h2> {g->text text="Language Packages"} </h2>
  {foreach from=$AdminRepositoryDownload.upgradeData item=item}
  <div style="position: relative; left: 25px">
    <div class="languagePacks" id="{$item.repository}_languagePacks"
	 style="{if empty($currentlyInstalled) || $item.repository != $currentlyInstalled}display: none{/if}">
      <p id="{$item.repository}_languages">
        {if !empty($item.languages)}
        {g->text text="(%sselect all%s%sselect none%s)"
	   arg1="<a id=\"`$item.repository`_selectAllLink\" href=\"javascript:selectAll('`$item.repository`')\">"
	   arg2="</a>"
	   arg3="<a style=\"display: none\" id=\"`$item.repository`_selectNoneLink\" href=\"javascript:selectNone('`$item.repository`')\">"
	   arg4="</a>"}
        {foreach from=$item.languages key=code item=pack}
        <br/>
	{counter assign="langId"}
	{capture assign="label"}
	{assign var="checked" value=""}
	<label for="lang_{$langId}">
	{if $pack.relation == "older" && $pack.currentBuild}
	  {g->text text="%s version %s (upgrading from %s)"
	      arg1="<b>`$pack.name`</b>" arg2=$pack.newBuild arg3=$pack.currentBuild}
          {if !empty($AdminRepositoryDownload.installedLanguages.$code)}
	  {assign var="checked" value="checked"}
	  {/if}
	{elseif $pack.relation == "older"}
	  {g->text text="%s version %s" arg1="<b>`$pack.name`</b>" arg2=$pack.newBuild}
	{elseif $pack.relation == "newer"}
	  {g->text text="%s version %s (%snewer version %s is installed%s)"
	      arg1="<b>`$pack.name`</b>" arg2=$pack.newBuild arg3="<b>" arg4=$pack.currentBuild arg5="</b>"}
	{else}
	  {g->text text="%s version %s (currently installed)" arg1="<b>`$pack.name`</b>" arg2=$pack.newBuild}
	  {assign var="checked" value="checked"}
	{/if}
	</label>
	{/capture}
	  <input type="hidden" name="{g->formVar var="form[languagesAvailable][]"}" value="{$item.repository}:{$code}"/>
	  <input id="lang_{$langId}" type="checkbox" name="{g->formVar var="form[languages][]"}"
	    value="{$item.repository}:{$code}:{$pack.newBuild}" {if !empty($checked)}checked="{$checked}"{/if}/>
	{$label}
        {/foreach}
        {else} {* !empty($item.languages) *}
          <i>{g->text text="No compatible language packages available"}</i>
        {/if}
      </p>
    </div>
  </div>
  {/foreach}
  {if !isset($currentlyInstalled)}
  <div style="position: relative; left: 25px" id="languageListPlaceholder">
    <i>{g->text text="You must select a base package before choosing language packs."}</i>
  </div>
  {/if}
</div>

<div class="gbBlock gcBackground1">
  <input class="inputTypeSubmit" type="submit" name="{g->formVar var="form[action][download]"}" value="{g->text text="Update"}"/>
  <input class="inputTypeSubmit" type="submit" name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
  <input type="hidden" name="{g->formVar var="form[pluginType]"}" value="{$AdminRepositoryDownload.pluginType}" />
  <input type="hidden" name="{g->formVar var="form[pluginId]"}" value="{$AdminRepositoryDownload.pluginId}" />
</div>
