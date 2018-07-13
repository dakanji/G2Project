<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title> Gallery 2: Localization Status Report (<?php print strftime("%x") ?>)</title>
    <link rel="stylesheet" type="text/css" href="localization/style.css"/>
  </head>
    <body>
      <h1> Localization Status Report for Gallery 2 </h1>
      <h3> Generated: <?php print strftime("%x %X") ?> <br/>
	   Most Recent PO: <?php print strftime("%x %X", $mostRecentPoDate) ?> <br/>
	   Total Translated Strings: <?php print $totalTranslated ?> </h3>

      <center>
	View: <a href="?type=summary"> summary </a> | <b> detail </b>
	<table class="report">
	  <?php $index = 0; ?>
	  <?php foreach ($reportData as $locale => $localeData) { ?>
	  <?php include(dirname(__FILE__) . '/row_header_detail.inc'); ?>
	  <?php $index++; ?>
	    <tr>
	      <td rowspan="<?php print sizeof($localeData['plugins'])?>" class="dark">
		<?php print $index; ?> 
	      </td>
	      <td rowspan="<?php print sizeof($localeData['plugins'])?>" class="dark">
		<?php print $locale ?>
	      </td>
	      <td rowspan="<?php print sizeof($localeData['plugins'])?>" class="locale_percent"
		  style="background-color:#<?php print percentColor($localeData['percentDone']) ?>">
		<?php print $localeData['percentDone'] ?>%
	      </td>
	      <?php $keys = array_keys($localeData['plugins']); ?>
	      <?php $plugin = $keys[0]; ?>
	      <?php $pluginData = $localeData['plugins'][$plugin]; ?>
	      <?php include(dirname(__FILE__) . '/plugin_rowdata.inc'); ?>
	    </tr>

	    <?php for ($i = 1; $i < sizeof($localeData['plugins']); $i++) { ?>
	    <tr>
	      <?php $keys = array_keys($localeData['plugins']); ?>
	      <?php $plugin = $keys[$i]; ?>
	      <?php $pluginData = $localeData['plugins'][$plugin]; ?>
	      <?php include(dirname(__FILE__) . '/plugin_rowdata.inc'); ?>
	    </tr>
	    <?php } ?>
	  <?php } ?>
	</table>
      </center>
    </body>
</html>
