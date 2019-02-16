{*
 * $Revision: 15543 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<script type="text/javascript">
  // <![CDATA[

  try {ldelim}
    {* http://msdn.microsoft.com/workshop/author/dhtml/reference/constants/clearauthenticationcache.asp *}
    document.execCommand("ClearAuthenticationCache");
  {rdelim} catch (exception) {ldelim}
  {rdelim}

  window.location = "{$TryLogout.scriptUrl}";

  // ]]>
</script>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="If you're not automatically redirected, %sclick here to finish logging out%s." arg1="<a href=\"`$TryLogout.hrefUrl`\">" arg2="</a>"}
  </p>
</div>
