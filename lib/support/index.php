<?php
define('G2_SUPPORT_URL_FRAGMENT', '');
require_once(dirname(__FILE__) . '/security.inc');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
/* Tell other scripts we passed security.inc ok */
define('G2_SUPPORT', true);
if (!empty($_SERVER['QUERY_STRING'])) {
    foreach (array('phpinfo', 'cache', 'gd', 'chmod') as $script) {
    	/*
    	 * Don't use isset($_GET[$script]) since we want to allow for GET args could collide
    	 * with the above mentioned script names
    	 */
	if ($_SERVER['QUERY_STRING'] == $script ||
	        strncmp($_SERVER['QUERY_STRING'], $script . '&', strlen($script)+1) == 0) {
	    include(dirname(__FILE__) . '/' . $script . '.php');
	    return;
	}
    }
}
?>
<html>
  <head>
    <title>Gallery Support</title>
    <link rel="stylesheet" type="text/css" href="<?php print $baseUrl ?>support.css"/>
  </head>

  <body>
    <div id="content">
      <div id="title">
        <a href="../../">Gallery</a> &raquo; Support
      </div>

      <h2>
	This is a collection of scripts that you use to troubleshoot problems with
	your Gallery installation.
      </h2>

      <h2>
        <a href="index.php?phpinfo">PHP Info</a>
      </h2>
      <p class="description">
        PHP configuration information
      </p>
      <hr class="faint" />

      <h2>
        <a href="index.php?cache">Cache Maintenance</a>
      </h2>
      <p class="description">
        Delete files from the Gallery data cache
      </p>
      <hr class="faint" />

      <h2>
        <a href="index.php?chmod">Filesystem Permissions</a>
      </h2>
      <p class="description">
        Change the filesystem permissions of your Gallery and your storage folder.
      </p>
      <hr class="faint" />

      <h2>
        <a href="index.php?gd">GD</a>
      </h2>
      <p class="description">
        Information about your GD configuration
      </p>
    </div>
  </body>
</html>
