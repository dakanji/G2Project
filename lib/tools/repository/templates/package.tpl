<?php
function unpack_{$random}($outputDir) {ldelim}
    /* Create directory structure. */
{foreach from=$directories item=directory}
{if $directory}
    GalleryUtilities::guaranteeDirExists($outputDir . '{$directory}');
{/if}
{/foreach}

    /* Recreate individual files. */
{foreach from=$files item=file}
    expand_{$random}($outputDir, '{$file.path}', '{$file.data}');

{/foreach}
{rdelim}

function expand_{$random}($outputDir, $relativePath, $data) {ldelim}
    $fd = fopen($outputDir . $relativePath, 'w');
    fwrite($fd, base64_decode($data));
    fclose($fd);
{rdelim}

$unpackFunction = "unpack_{$random}";
?>
