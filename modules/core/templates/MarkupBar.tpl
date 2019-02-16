{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if $theme.markupType == 'bbcode'}
{if !empty($firstMarkupBar)}
<script type="text/javascript">{literal}
  // <![CDATA[
  function openOrCloseTextElement(elementId, bbCodeElement, button) {
    var element = document.getElementById(elementId);
    if (!button.g2ToggleMode) {
      element.value = element.value + '[' + bbCodeElement + ']';
      button.value = '*' + button.value;
    } else {
      element.value = element.value + '[/' + bbCodeElement + ']';
      button.value = button.value.substring(1);
    }
    element.focus();
    button.g2ToggleMode = !button.g2ToggleMode;
  }

  function appendTextElement(elementId, bbCodeElement, button) {
    var element = document.getElementById(elementId);
    element.value = element.value + '[' + bbCodeElement + ']';
    element.focus();
  }

  var isInit = false;

  function appendColorElement(elementId, button) {
    var colorChooser = document.getElementById('Markup_colorChooser');
    if (!button.g2ToggleMode) {
      /* if we already have a popup, don't do anything */
      if (colorChooser.style.display == 'block') {
        return;
      }
      button.value = '*' + button.value;
      colorChooser.g2ElementId = elementId;
      if (!isInit) {
        init();
        isInit = true;
      }
      if (colorChooser.style.display == 'block') {
        colorChooser.style.display = 'none';
      } else {
        var pos = YAHOO.util.Dom.getXY(button);
        colorChooser.style.display = 'block';
        YAHOO.util.Dom.setXY(colorChooser, [pos[0] + 30, pos[1] + button.offsetHeight + 10]);
      }
    } else {
      button.value = button.value.substring(1);
      /* if popup is on the screen just dismiss it */
      if (colorChooser.style.display == 'block') {
        colorChooser.style.display='none';
      } else {
        var element = document.getElementById(elementId);
        element.value = element.value + '[/color]';
        element.focus();
      }
    }
    button.g2ToggleMode = !button.g2ToggleMode;
  }

  {/literal}
  function appendUrlElement(elementId, bbCodeElement) {ldelim}
    var element = document.getElementById(elementId);
    var url = prompt('{g->text text="Enter a URL" forJavascript=true}', ''), text = null;
    if (url != null) text = prompt('{g->text text="Enter some text describing the URL"
					     forJavascript=true}', '');
    if (text != null) {ldelim}
      if (text.length) element.value = element.value + '[url=' + url + ']' + text + '[/url]';
      else element.value = element.value + '[url]' + url + '[/url]';
    {rdelim}      
    element.focus();
  {rdelim}

  function appendImageElement(elementId, bbCodeElement) {ldelim}
    var element = document.getElementById(elementId);
    var url = prompt('{g->text text="Enter an image URL" forJavascript=true}', '');
    if (url != null) element.value = element.value + '[img]' + url + '[/img]';
    element.focus();
  {rdelim}
  // ]]>
</script>
<script type="text/javascript" src="{g->url href="lib/yui/yahoo-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/event-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/dom-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/animation-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/dragdrop-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/color.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/yui/slider-min.js"}"></script>
<script type="text/javascript" src="{g->url href="lib/javascript/ColorChooser.js"}"></script>
{/if}

<div class="gbMarkupBar">
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="B <!-- Button label for Bold -->"}"
	 onclick="openOrCloseTextElement('{$element}', 'b', this)"
	 style="font-weight: bold;"/>
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="i <!-- Button label for italic -->"}"
	 onclick="openOrCloseTextElement('{$element}', 'i', this)"
	 style="font-style: italic; padding-left: 1px; padding-right: 4px"/>
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="list"}"
	 onclick="openOrCloseTextElement('{$element}', 'list', this)"/>
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="bullet"}"
	 onclick="appendTextElement('{$element}', '*', this)"/>
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="url"}"
	 onclick="appendUrlElement('{$element}', this)"/>
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="image"}"
	 onclick="appendImageElement('{$element}', this)"/>
  <input type="button" class="inputTypeButton" tabindex="32767"
	 value="{g->text text="color"}" id="{$element}_color"
	 onclick="appendColorElement('{$element}', this)"/>
</div>

{if !empty($firstMarkupBar)}
<div id="Markup_colorChooser">
  <div id="Markup_colorHandle">&nbsp;</div>
  <div id="Markup_pickerDiv">
    <img id="Markup_pickerBg" src="{g->url href="modules/core/data/pickerbg.png"}" alt=""/>
    <div id="Markup_selector"><img src="{g->url href="modules/core/data/select.gif"}" alt=""/></div>
  </div>

  <div id="Markup_hueBg">
    <div id="Markup_hueThumb"><img src="{g->url href="modules/core/data/hline.png"}" alt=""/></div>
  </div>

  <div id="Markup_valdiv">
    <br/>
    R <input name="rval" id="Markup_rval" type="text" value="0" size="3" maxlength="3"/>
    H <input name="hval" id="Markup_hval" type="text" value="0" size="3" maxlength="3"/>
    <br/>
    G <input name="gval" id="Markup_gval" type="text" value="0" size="3" maxlength="3"/>
    S <input name="gsal" id="Markup_sval" type="text" value="0" size="3" maxlength="3"/>
    <br/>
    B <input name="bval" id="Markup_bval" type="text" value="0" size="3" maxlength="3"/>
    V <input name="vval" id="Markup_vval" type="text" value="0" size="3" maxlength="3"/>
    <br/>
    <br/>
    # <input name="hexval" id="Markup_hexval" type="text" value="0" size="6" maxlength="6"/>
    <br/>
    <input value="Done" class="yui-log-button" style="font-size: 11px;" type="button"
      onclick="userUpdate()"/>
  </div>
  <div id="Markup_swatch">&nbsp;</div>
  <div id="Markup_hint">{g->text text="You can also use the <a href=\"%s\" target=\"_new\">color name</a> for example: %s" arg1="http://www.w3.org/TR/2002/WD-css3-color-20020418/#html4" arg2="[color=red]Your Text[/color]"}</div>
</div>
{/if}
{/if}
