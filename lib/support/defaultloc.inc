<?php
$fileInfo = pathinfo($_SERVER['PHP_SELF']);
$file = basename($file, '.' . $fileInfo['extension']);
header("Location: " . str_replace($fileInfo['filename'], "index.php?" . $file, $_SERVER['REQUEST_URI']));
?>