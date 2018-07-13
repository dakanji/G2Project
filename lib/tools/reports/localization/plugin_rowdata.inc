	    <?php newRow(); ?>
	    <td class="<?php print modifier('plugin') ?>">
	      <?php print $plugin ?>
	    </td>
	    <td style="background-color:#<?php print percentColor($pluginData['percentDone']) ?>">
	      <?php print $pluginData['percentDone'] ?>%
	    </td>

	    <?php if (isset($pluginData['missing'])) { ?>
	    <td colspan="5" class="<?php print modifier('missing') ?>">
	      Missing
	    </td>
	    <?php } else { ?>
	    <td class="<?php print modifier('total') ?>">
	      <?php print $pluginData['total'] ?>
	    </td>
	    <td class="<?php print modifier('translated') ?>">
	      <?php print $pluginData['translated'] ?>
	    </td>
	    <td class="<?php print modifier('fuzzy') ?>">
	      <?php print $pluginData['fuzzy'] ?>
	    </td>
	    <td class="<?php print modifier('untranslated') ?>">
	      <?php print $pluginData['untranslated'] ?>
	    </td>
	    <td class="<?php print modifier('obsolete') ?>">
	      <?php print $pluginData['obsolete'] ?>
	    </td>
	    <?php } ?>
