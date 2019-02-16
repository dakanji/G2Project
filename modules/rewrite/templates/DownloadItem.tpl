{*
 * $Revision: 17265 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <title>{$item.title|markup:strip|default:$item.pathComponent}</title>
  </head>
  <body>
    <p>{g->image image=$image item=$item}</p>
    <p><a href="{g->url}">{$galleryTitle|markup:strip}</a></p>
  </body>
</html>
