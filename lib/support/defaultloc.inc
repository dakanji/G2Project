<?php
header('Location: ' . str_replace(basename($_SERVER['PHP_SELF']), "index.php", $_SERVER["REQUEST_URI"]));
?>
