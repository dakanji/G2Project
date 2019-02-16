{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="System Maintenance"} </h2>
</div>

{if isset($status.run)}
<div class="gbBlock">
  {capture name=taskTitle}<b>{g->text text=$AdminMaintenance.tasks[$status.run.taskId].title
   l10Domain=$AdminMaintenance.tasks[$status.run.taskId].l10Domain}</b>{/capture}
  {if ($status.run.success)}
    <h2 class="giSuccess">
      {g->text text="Completed %s task successfully." arg1=$smarty.capture.taskTitle}
    </h2>
  {else}
    <h2 class="giError">
      {g->text text="The %s task failed to complete successfully." arg1=$smarty.capture.taskTitle}
    </h2>
  {/if}
</div>
{/if}

<div class="gbBlock">
  <table class="gbDataTable" width="100%">
    <tr>
      <th> {g->text text="Task name"} </th>
      <th> {g->text text="Last run"} </th>
      <th> {g->text text="Success/Fail"} </th>
      <th> {g->text text="Action"} </th>
    </tr>
    {foreach from=$AdminMaintenance.tasks key=taskId item=info}
    {cycle values="gbEven,gbOdd" assign="rowClass"}
    <tr class="{$rowClass}">
      <td>
	<span id="task-{$taskId}-toggle"
	      class="giBlockToggle gcBackground1 gcBorder2"
	      style="border-width: 1px"
	      onclick="BlockToggle('task-{$taskId}-description', 'task-{$taskId}-toggle', 'table-row')">{if !isset($status.run) || $status.run.taskId != $taskId}+{else}-{/if}</span>
	{g->text text=$info.title l10Domain=$info.l10Domain}
      </td><td>
	{if isset($info.timestamp)}
	  {g->date timestamp=$info.timestamp style="datetime"}
	{else}
	  {g->text text="Not run yet"}
	{/if}
      </td><td>
	{if isset($info.success)}
	  {if $info.success}
	  <div class="giSuccess">
	    {g->text text="Success"}
	  </div>
	  {else}
	  <div class="giError">
	    {g->text text="Failed"}
	  </div>
	  {/if}
	{else}
	  {g->text text="Not run yet"}
	{/if}
      </td><td>
	<a href="{g->url arg1="controller=core.AdminMaintenance" arg2="form[action][runTask]=1"
	 arg3="taskId=`$taskId`"}"{if isset($info.confirmRun)} onclick="return confirm('{g->text
	 text=$info.title forJavascript=1}: {g->text text="Are you sure?" forJavascript=1}')"
	 {/if}>{g->text text="run now"}</a>
      </td>
    </tr>
    <tr class="{$rowClass}" id="task-{$taskId}-description"
     {if !isset($status.run) || $status.run.taskId != $taskId}style="display: none"{/if}>
      <td colspan="4">
	{g->text text=$info.description l10Domain=$info.l10Domain}
	{if !empty($info.details)}
	  <p class="giDescription"> {g->text text="Last Run Details:"} </p>
	  <p class="giInfo">
	  {foreach from=$info.details item=text}
	    {$text} <br/>
	  {/foreach}
	  </p>
	{/if}
      </td>
    </tr>
    {/foreach}
  </table>
</div>
