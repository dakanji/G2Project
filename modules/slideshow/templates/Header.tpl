{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<script type="text/JavaScript">
    //<![CDATA[
    var agent = navigator.userAgent.toLowerCase();
    var appver = parseInt(navigator.appVersion);
    var bCanBlend = (agent.indexOf('msie') != -1) && (agent.indexOf('opera') == -1) &&
                    (appver >= 4) && (agent.indexOf('msie 4') == -1) &&
                    (agent.indexOf('msie 5.0') == -1);
    var filterNames = new Array(16), filters = new Array(16);
    filterNames[0] = '{g->text text="Blend" forJavascript="1"}';
    filters[0] = 'progid:DXImageTransform.Microsoft.Fade(duration=1)';
    filterNames[1] = '{g->text text="Blinds" forJavascript="1"}';
    filters[1] = 'progid:DXImageTransform.Microsoft.Blinds(duration=1,bands=20)';
    filterNames[2] = '{g->text text="Checkerboard" forJavascript="1"}';
    filters[2] = 'progid:DXImageTransform.Microsoft.Checkerboard(duration=1,squaresX=20,squaresY=20)';
    filterNames[3] = '{g->text text="Diagonal" forJavascript="1"}';
    filters[3] = 'progid:DXImageTransform.Microsoft.Strips(duration=1,motion=rightdown)';
    filterNames[4] = '{g->text text="Doors" forJavascript="1"}';
    filters[4] = 'progid:DXImageTransform.Microsoft.Barn(duration=1,orientation=vertical)';
    filterNames[5] = '{g->text text="Gradient" forJavascript="1"}';
    filters[5] = 'progid:DXImageTransform.Microsoft.GradientWipe(duration=1)';
    filterNames[6] = '{g->text text="Iris" forJavascript="1"}';
    filters[6] = 'progid:DXImageTransform.Microsoft.Iris(duration=1,motion=out)';
    filterNames[7] = '{g->text text="Pinwheel" forJavascript="1"}';
    filters[7] = 'progid:DXImageTransform.Microsoft.Wheel(duration=1,spokes=12)';
    filterNames[8] = '{g->text text="Pixelate" forJavascript="1"}';
    filters[8] = 'progid:DXImageTransform.Microsoft.Pixelate(duration=1,maxSquare=10)';
    filterNames[9] = '{g->text text="Radial" forJavascript="1"}';
    filters[9] = 'progid:DXImageTransform.Microsoft.RadialWipe(duration=1,wipeStyle=clock)';
    filterNames[10] = '{g->text text="Rain" forJavascript="1"}';
    filters[10] = 'progid:DXImageTransform.Microsoft.RandomBars(duration=1,orientation=vertical)';
    filterNames[11] = '{g->text text="Slide" forJavascript="1"}';
    filters[11] = 'progid:DXImageTransform.Microsoft.Slide(duration=1,slideStyle=push)';
    filterNames[12] = '{g->text text="Snow" forJavascript="1"}';
    filters[12] = 'progid:DXImageTransform.Microsoft.RandomDissolve(duration=1,orientation=vertical)';
    filterNames[13] = '{g->text text="Spiral" forJavascript="1"}';
    filters[13] = 'progid:DXImageTransform.Microsoft.Spiral(duration=1,gridSizeX=40,gridSizeY=40)';
    filterNames[14] = '{g->text text="Stretch" forJavascript="1"}';
    filters[14] = 'progid:DXImageTransform.Microsoft.Stretch(duration=1,stretchStyle=push)';
    filterNames[15] = '{g->text text="RANDOM" forJavascript="1"}';
    filters[15] = 'RANDOM';
    // ]]>
</script>
