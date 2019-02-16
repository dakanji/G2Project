/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

var hue;
var picker;
var dd;

function init() {
	hue = YAHOO.widget.Slider.getVertSlider("Markup_hueBg", "Markup_hueThumb", 0, 180);
	hue.onChange = function(newVal) { hueUpdate(newVal); };

	picker = YAHOO.widget.Slider.getSliderRegion("Markup_pickerDiv", "Markup_selector", 
			0, 180, 0, 180);
	picker.onChange = function(newX, newY) { pickerUpdate(newX, newY); };
	hueUpdate();

	dd = new YAHOO.util.DD("Markup_colorChooser");
	dd.setHandleElId("Markup_colorHandle");
	dd.endDrag = function(e) {	};
    // yuck. correctly handle PNG transparency in Win IE
    // also, the color it will be below SELECT elements (date pieces)
    // see http://www.codetoad.com/forum/20_22736.asp
    var isIE = !window.opera && navigator.userAgent.indexOf('MSIE') != -1;
    if (isIE) {
        var imgID = "Markup_pickerbg";
        var img = document.getElementById(imgID);
        var imgName = img.src.toUpperCase();
        var imgTitle = (img.title) ? "title='" + img.title + "' " : "title='" + img.alt + "' ";
        var strNewHTML = "<span id='" + imgID + "'" + imgTitle
            + " style=\"width:192px; height:192px;display:inline-block;"
            + "filter:progid:DXImageTransform.Microsoft.AlphaImageLoader"
            + "(src=\'" + img.src + "\', sizingMethod='scale');\"></span>"; 
        img.outerHTML = strNewHTML;
    }
}

function pickerUpdate(newX, newY) {
	swatchUpdate();
}

function hueUpdate(newVal) {
	var h = (180 - hue.getValue()) / 180;
	if (h == 1) { h = 0; }
	var a = YAHOO.util.Color.hsv2rgb( h, 1, 1);

	document.getElementById("Markup_pickerDiv").style.backgroundColor = 
		"rgb(" + a[0] + ", " + a[1] + ", " + a[2] + ")";

	swatchUpdate();
}

function swatchUpdate() {
	var h = (180 - hue.getValue());
	if (h == 180) { h = 0; }
	document.getElementById("Markup_hval").value = (h*2);

	h = h / 180;

	var s = picker.getXValue() / 180;
	document.getElementById("Markup_sval").value = Math.round(s * 100);

	var v = (180 - picker.getYValue()) / 180;
	document.getElementById("Markup_vval").value = Math.round(v * 100);

	var a = YAHOO.util.Color.hsv2rgb( h, s, v );

	document.getElementById("Markup_swatch").style.backgroundColor = 
		"rgb(" + a[0] + ", " + a[1] + ", " + a[2] + ")";

	document.getElementById("Markup_rval").value = a[0];
	document.getElementById("Markup_gval").value = a[1];
	document.getElementById("Markup_bval").value = a[2];
	document.getElementById("Markup_hexval").value = 
		YAHOO.util.Color.rgb2hex(a[0], a[1], a[2]);
}

function userUpdate() {
	var colorChooser = document.getElementById("Markup_colorChooser");
    var element = document.getElementById(colorChooser.g2ElementId);
	var color = document.getElementById("Markup_hexval").value;
	element.value = element.value + '[color=#' + color + ']';
	colorChooser.style.display = 'none';
	element.focus();
}

