{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{capture name="baseUrl"}{g->url arg1="controlle=migrate.Redirect"
				forceSessionId=false forceFullUrl=true}{/capture}
<IfModule mod_rewrite.c>
  RewriteEngine On
  RewriteCond %{ldelim}REQUEST_FILENAME{rdelim} !-f
  RewriteCond %{ldelim}REQUEST_FILENAME{rdelim} !-d
  Rewritecond %{ldelim}REQUEST_FILENAME{rdelim} !gallery_remote2.php
  RewriteRule (.*)$ {$smarty.capture.baseUrl|replace:"controlle=":"controller="|regex_replace:"#^.*?://[^/]*#":""}&g2_path=$1 [QSA]
</IfModule>
