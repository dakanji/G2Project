{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{* Set defaults *}
{if empty($item)} {assign var=item value=$theme.item} {/if}
{g->callback type="comment.AddComment" itemId=$item.id}

{if !isset($expand)}{assign var="expand" value=true}{/if}

{if !empty($block.comment.AddComment)}
<div id="AddComment_block" {if $expand}style="display: none"{/if} class="{$class}">
  {include file="gallery:modules/comment/templates/AddComment.tpl"
           AddComment=$block.comment.AddComment inBlock=1}
</div>

{if $expand}
<div id="AddComment_trigger" class="{$class}" onclick="AddComment_showBlock()">
  <div class="gbBlock gcBackground1">
    <h3> {g->text text="Add Comment"} </h3>
  </div>
  <textarea cols=80 rows=5></textarea>
</div>

{literal}
<script type="text/javascript">
  // <![CDATA[
  function AddComment_showBlock() {
    document.getElementById('AddComment_block').style.display='block';
    document.getElementById('AddComment_trigger').style.display='none';
  }
  // ]]>
</script>
{/literal}
{/if}
{/if}
