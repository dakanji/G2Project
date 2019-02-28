{*
 * $Revision: 17254 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<!DOCTYPE html>
<html>
  <head>
    <title>{$item.title|markup:strip|default:$item.pathComponent}</title>
  </head>
  <body>
    <p>{g->image image=$image item=$item}</p>
    <p><a href="{g->url}">{$galleryTitle|markup:strip}</a></p>
  </body>
</html>
