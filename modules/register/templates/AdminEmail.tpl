{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->text text="New user registration:"}

{g->text text="    Username: %s" arg1=$username}
{g->text text="   Full name: %s" arg1=$name}
{g->text text="       Email: %s" arg1=$email}

{g->text text="Activate or delete this user here"}
{g->url arg1="view=core.SiteAdmin" arg2="subView=register.AdminSelfRegistration"
	forceFullUrl=true htmlEntities=false forceSessionId=false}
