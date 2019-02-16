{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if $callCount == 1}
<script type="text/javascript" src="{g->url href="lib/yui/yahoo-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/dom-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/event-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/connection-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/animation-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/autocomplete-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/javascript/AutoComplete.js"}"></script>
{/if}
<script type="text/javascript">
  // <![CDATA[
  YAHOO.util.Event.addListener(
    this, 'load',
    function(e, data) {ldelim} autoCompleteAttach(data[0], data[1]); {rdelim},
    ['{$element}', '{$url}']);
  // ]]>
</script>

