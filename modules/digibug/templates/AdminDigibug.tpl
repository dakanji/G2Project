{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Digibug Photo Printing Settings"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Sell your photos as prints or printed gift products!  Visit the %sDigibug configuration page%s for information on registering for a Digibug Pro Photographer account, and to learn how to obtain your Digibug Company ID and Event ID.  For general information, please refer to the %sDigibug.com website.%s"
     arg1="<a href=\"http://www.digibug.com/redirects/digibugapi_how_to.php\">" arg2="</a>"
     arg3="<a href=\"http://www.digibug.com/\">" arg4="</a>"}
  </p>

  <table class="gbDataTable">
    <tr><td>
      <label for="formDigibugCustomerId">
	{g->text text="Digibug Company ID"}
      </label>
    </td><td>
      <input type="text" size="6" id="formDigibugCustomerId" autocomplete="off"
       name="{g->formVar var="form[digibugCustomerId]"}" value="{$form.digibugCustomerId}"/>
    </td></tr>
    {if isset($form.error.digibugCustomerId.missing)}
    <tr><td colspan="2">
      <div class="giError">
	{g->text text="You must enter a digibug customer id."}
      </div>
    </td></tr>
    {/if}
    <tr><td>
      <label for="formDigibugEventId">
	{g->text text="Event ID"}
      </label>
    </td><td>
      <input type="text" size="6" id="formDigibugEventId" autocomplete="off"
       name="{g->formVar var="form[digibugPricelistId]"}" value="{$form.digibugPricelistId}"/>
    </td></tr>
    {if isset($form.error.digibugPricelistId.missing)}
    <tr><td colspan="2">
      <div class="giError">
	{g->text text="Please create an event or use your default event id as your pricelist id."}
      </div>
    </td></tr>
    {/if}
  </table>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  {if $AdminDigibug.isConfigure}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
  {else}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
  {/if}
</div>
