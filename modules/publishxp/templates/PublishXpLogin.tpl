{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<form action="{g->url}" enctype="application/x-www-form-urlencoded" method="post"
 id="publishXpForm">
  <div>
    <script type="text/javascript">
       setSubtitle("{g->text text="Login to your account"}");
       setSubmitOnNext(true);
       setButtons(false, true, false);
    </script>

    {g->hiddenFormVars}
    <input type="hidden" name="{g->formVar var="controller"}" value="publishxp.PublishXpLogin"/>
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
    <input type="hidden" name="{g->formVar var="form[action][login]"}" value="1"/>
  </div>

  <div class="gbBlock">
    <h4>
      {g->text text="Username"}
    </h4>

    <input id="giFormUsername" type="text" name="{g->formVar var="form[userName]"}"
           size="16" value="{$form.userName}"/>
    <script type="text/javascript">document.getElementById('publishXpForm')['{g->formVar
     var="form[userName]"}'].focus();</script>

    {if isset($form.error.userName.missing)}
    <div class="giError">
      {g->text text="You must enter a username"}
    </div>
    {elseif isset($form.error.userName.disabled)}
    <div class="giError">
      {g->text text="Logins temporarily disabled due to multiple failed login attempts."}
    </div>
    {/if}

    <h4> {g->text text="Password"} </h4>

    <input id="giFormPassword" type="password" size="16" name="{g->formVar var="form[password]"}"/>

    {if isset($form.error.password.missing)}
    <div class="giError">
      {g->text text="You must enter a password"}
    </div>
    {/if}

    {if isset($form.error.password.invalid)}
    <div class="giError">
      {g->text text="Your login information is incorrect.  Please try again."}
    </div>
    {/if}
  </div>
</form>
