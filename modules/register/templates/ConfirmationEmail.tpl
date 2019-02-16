{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->text text="Hello %s," arg1=$name}

{g->text text="You receive this email because you have registered at %s" arg1=$baseUrl}
{g->text text="Your username is: %s" arg1=$username}

{g->text text="To finish the registration process please click the following link:"}
{$confirmationUrl}

{g->text text="If you did not register at this site then please ignore this email.  The registration will not become valid and you will not receive any further emails.  Sorry for the inconvenience."}

{g->text text="Thank you!"}
