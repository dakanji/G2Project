{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if !isset($showAdvancedLink)} {assign var="showAdvancedLink" value="true"} {/if}

{g->addToTrailer}
<script type="text/javascript">
  // <![CDATA[
  search_SearchBlock_init('{g->text text="Search the Gallery"}', '{g->text text="Please enter a search term."}');
  // ]]>
</script>
{/g->addToTrailer}

<div class="{$class}">
  <form id="search_SearchBlock" action="{g->url}" method="post" onsubmit="return search_SearchBlock_checkForm()">
    <div>
      {g->hiddenFormVars}
      <input type="hidden" name="{g->formVar var="view"}" value="search.SearchScan"/>
      <input type="hidden" name="{g->formVar var="form[formName]"}" value="search_SearchBlock"/>
      <input type="text" id="searchCriteria" size="18"
	     name="{g->formVar var="form[searchCriteria]"}"
	     value="{g->text text="Search the Gallery"}"
	     onfocus="search_SearchBlock_focus()"
	     onblur="search_SearchBlock_blur()"
	     class="textbox"/>
      <input type="hidden" name="{g->formVar var="form[useDefaultSettings]"}" value="1" />
    </div>
    {if $showAdvancedLink}
    <div>
      <a href="{g->url arg1="view=search.SearchScan" arg2="form[useDefaultSettings]=1"
		       arg3="return=1"}"
	 class="{g->linkId view="search.SearchScan"} advanced">{g->text text="Advanced Search"}</a>
    </div>
    {/if}
  </form>
</div>

