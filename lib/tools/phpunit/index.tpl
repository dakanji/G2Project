<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <title>Gallery Unit Tests</title>
    <STYLE TYPE="text/css">
      <?php include ("stylesheet.css"); ?>
    </STYLE>
  </head>
  <body>
  <?php if (!isset($compactView)): ?>
    <script type="text/javascript" language="javascript">
      function setFilter(value) {
        document.forms[0].filter.value=value;
      }
      function reRun() {
        setFilter(failedTestFilter);
        document.forms[0].submit();
      }
    </script>
    <div id="status" style="display: none;">
      <div class="header">Run Status</div>
      <div class="body">
	Pass: <span id="pass_count">&nbsp;</span>, Fail <span id="fail_count">&nbsp;</span>, Skip: <span id="skip_count">&nbsp;</span>, Total: <span id="total_count">&nbsp;</span> <br/>
	Estimated time remaining: <span id="estimated_time_remaining">&nbsp;</span> <br/>
	Memory Usage: <span id="used_memory">&nbsp;</span> (<?php print (0 < ini_get('memory_limit')) ? ini_get('memory_limit') : 0; ?> allowed)
      </div>
    </div>

    <h1>Gallery Unit Tests</h1>
    <div class="section">
      This is the Gallery test framework.  We'll use this to verify
      that the Gallery code is functioning properly.  It'll help us
      identify bugs in the code when we add new features, port to new
      systems, or add support for new database back ends.  All the
      tests should pass with a green box that says <b>OK</b> in it).
    </div>

    <?php if (!$isSiteAdmin): ?>
    <h2> <span class="error">ERROR!</span> </h2>
    <div class="section">
      You are not logged in as a Gallery site administrator so you are
      not allowed to run the unit tests.  If you have cookies disabled, then you
      must go back to the page where you logged in and copy the part of your URL
      that looks like this:
      <p>
	<code>g2_GALLERYSID=51c0ca5a9ce1296ccfd5307fa77fd998</code>
      </p>
      get rid of the <i>g2_GALLERYSID</i> part and paste it into this text box then
      click the Reload Page button.  That will transfer your session from
      the page where you logged in over to this page.

      <a href="../../../main.php?g2_view=core.UserAdmin&g2_subView=core.UserLogin&g2_return=<?php echo $_SERVER['REQUEST_URI']?>">[ login ]</a>
      <form>
	<input type="text" size=33 name="<?php echo isset($sessionKey) ? $sessionKey : '' ?>">
	  <input type="submit" value="Reload page">
      </form>
    </div>
    <?php endif; ?>

    <script type="text/javascript">
      examplesVisible = false;
      function toggleFilterExamples() {
        myList = document.getElementById('help_and_examples');
        myIndicator = document.getElementById('filter_examples_toggle_indicator');
        if (examplesVisible) {
	  myList.style.display = 'none';
	  myIndicator.innerHTML = '+';
	} else {
	  myList.style.display = 'inline';
	  myIndicator.innerHTML = '-';
	}
	examplesVisible = !examplesVisible;
      }

      modulesListingVisible = false;
      function toggleModulesListing() {
        myList = document.getElementById('modules_listing');
        myIndicator = document.getElementById('modules_listing_toggle_indicator');
        if (modulesListingVisible) {
          myList.style.display = 'none';
          myIndicator.innerHTML = '+';
        } else {
          myList.style.display = 'inline';
          myIndicator.innerHTML = '-';
        }
        modulesListingVisible = !modulesListingVisible;
      }
    </script>

    <?php if (sizeof($incorrectDevEnv) > 0): ?>
    <div style="float: right; width: 500px; border: 2px solid red; padding: 3px">
      <h2 style="margin: 0px"> Development Environment Warning </h2>
      <div style="margin-left: 5px">
        The following settings in your development environment are not correct.  See the <a href="http://codex.gallery2.org/index.php/Gallery2:Developer_Guidelines#PHP_Settings">G2 Development Environment</a> page for more information
      </div>
      <br/>
      <table border="0" class="details">
        <tr>
          <th> PHP Setting </th>
          <th> Actual Value </th>
          <th> Expected Value(s) </th>
        </tr>
        <?php foreach (array_keys($incorrectDevEnv) as $key): ?>
        <tr>
          <td> <?php print $key ?> </td>
          <td> <?php print $incorrectDevEnv[$key][1] ?> </td>
          <td> <?php print join(' <b>or</b> ', $incorrectDevEnv[$key][0]) ?> </td>
        </tr>
        <?php endforeach; ?>
      </table>
    </div>
    <?php endif; ?>

    <h2>Filter</h2>
    <div class="section">
      <form>
	<?php if (isset($sessionKey)): ?>
	<input type="hidden" name="<?php echo $sessionKey?>" value="<?php echo $sessionId ?>"/>
	<?php endif; ?>

	<input type="text" name="filter" size="60" value="<?php echo $displayFilter ?>"
	       id="filter" style="margin-top: 0.3em; margin-bottom: 0.3em"/>
	<?php if (!isset($_GET['filter'])): ?>
	  <script type="text/javascript"> document.getElementById('filter').focus(); </script>
	<?php endif; ?>

	<br/>
        <span id="filter_examples_toggle"
          href="#"
          onclick="toggleFilterExamples()">
          Help/Examples
          <span id="filter_examples_toggle_indicator"
            style="padding-left: .3em; padding-right: 0.3em; border: solid #a6caf0; border-width: 1px; background: #eee">+</span>
        </span>

        <div id="help_and_examples" style="display: none">
         <br/>
	  Enter a regular expression string to restrict testing to classes containing
          that text in their class name or test method.  If you use an exclamation before a
          module/class/test name(s) encapsulated in parenthesis and separated with bars, this will
          exclude the matching tests. Use ":#-#" to restrict which matching tests are actually run.
          You can also specify multiple spans with ":#-#,#-#,#-#".
	  Append ":1by1" to run tests one-per-request; automatic refresh stops when a test fails.

          <ul id="filter_examples_list">
            <li>
              <a href="javascript:setFilter('AddCommentControllerTest.testAddComment')">AddCommentControllerTest.testAddComment</a>
            </li>
            <li>
              <a href="javascript:setFilter('AddCommentControllerTest.testAdd')">AddCommentControllerTest.testAdd</a>
            </li>
            <li>
              <a href="javascript:setFilter('AddCommentControllerTest')">AddCommentControllerTest</a>
            </li>
            <li>
              <a href="javascript:setFilter('comment')">comment</a>
            </li>
            <li>
              <a href="javascript:setFilter('!(comment)')">!(comment)</a>
            </li>
            <li>
              <a href="javascript:setFilter('!(comment|core)')">!(comment|core)</a>
            </li>
            <li>
              <a href="javascript:setFilter('comment:1-3')">comment:1-3</a>
            </li>
            <li>
              <a href="javascript:setFilter('comment:3-')">comment:3-</a>
            </li>
            <li>
              <a href="javascript:setFilter('comment:-5')">comment:-5</a>
            </li>
            <li>
              <a href="javascript:setFilter('comment:1-3,6-8,10-12')">comment:1-3,6-8,10-12</a>
            </li>
            <li>
              <a href="javascript:setFilter('comment:-3,4-')">comment:-3,4-</a>
            </li>
            <li>
              <a href="javascript:setFilter('core:1by1')">core:1by1</a>
            </li>
          </ul>
        </div>
      </form>
    </div>

    <h2>Modules</h2>

    <div class="section" style="width: 100%">
      <?php
      $activeCount = 0;
      foreach ($moduleStatusList as $moduleId => $moduleStatus) {
        if (!empty($moduleStatus['active'])) {
          $activeCount++;
        }
      }
      ?>
      <?php printf("%d active, %d total", $activeCount, sizeof($moduleStatusList)); ?>
      <span onclick="toggleModulesListing()" id="modules_listing_toggle_indicator"
            style="padding-left: .3em; padding-right: 0.3em; border: solid #a6caf0; border-width: 1px; background: #eee">+</span>
      <br/>
      <table cellspacing="1" cellpadding="1" border="0"
        width="800" align="center" class="details"
        id="modules_listing"
        style="display: none">
        <tr>
          <th> Module Id </th>
          <th> Active </th>
          <th> Installed </th>
        </tr>
        <?php foreach ($moduleStatusList as $moduleId => $moduleStatus): ?>
        <tr>
          <td style="width: 100px">
            <?php print $moduleId ?>
          </td>
          <td style="width: 100px">
            <?php print !empty($moduleStatus['active']) ? "active" : "not active" ?>
          </td>
          <td style="width: 100px">
            <?php print !empty($moduleStatus['available']) ? "installed" : "not available" ?>
          </td>
        </tr>
        <?php endforeach; ?>
      </table>
    </div>
  <?php endif; /* compactView */ ?>

  <h2>Test Results</h2>

  <table cellspacing="1" cellpadding="1" border="0" width="90%" align="CENTER" class="details">
  <tr><th>#</th><th>Module</th><th>Class</th><th>Function</th><th>Success?</th><th>Time</th></tr>
  <?php $i = 0;
    foreach ($testSuite->fTests as $testClass):
    foreach ($testClass->fTests as $test): $i++;
    if (isset($testOneByOne) && $testOneByOne != $i) continue; ?>
    <tr id="testRow<?php print $i ?>">
    <td><?php print $i ?></td>
    <td><?php print $test->getModuleId() ?></td>
    <td><?php print $test->classname() ?></td>
    <td><?php print $test->name() ?></td>
    <td><a href="#fail<?php print $i ?>" style="display:none">FAIL</a>&nbsp;</td><td>&nbsp;</td>
  </tr><?php endforeach; endforeach; $totalTests = $i;?>
  </table>

  <div id="testSummary" style="display:none">
    <h2>Summary</h2>

    <p><span id="testTime">&nbsp;</span> seconds elapsed</p>
    <p><span id="testCount">&nbsp;</span> run</p>
    <p><span id="testFailCount">&nbsp;</span> failed
       with <span id="testErrorCount">&nbsp;</span></p>
    <p><a href="http://codex.gallery2.org/Gallery2:Test_Matrix#Unit_Tests">Test Matrix Entry</a>:
       <br/><b><span id="testReport">&nbsp;</span></b>
       <br/>(<span><a href="javascript:changeUsername()">change username</a></span>)
    </p>
    <script type="text/javascript">
      function getUsernameFromCookie() {
	var dc = document.cookie;
	if (dc) {
	  var m = dc.match(/g2_phpunit_username=(.*?);/);
	  if (m && m.length == 2) {
           return m[1];
          }
	}
        return 'NAME_PLACEHOLDER';
      }

      function setCookie(key, value) {
	document.cookie = key + '=' + escape(value) +
	'; expires=Sunday, January 17, 2038 4:00:00 PM';
      }

      function setUsername(oldUsername, newUsername) {
        var report = document.getElementById('testReport').firstChild;
	report.nodeValue = report.nodeValue.replace('\|' + oldUsername + '\|', '|' + newUsername + '|');
	setCookie('g2_phpunit_username', newUsername);
      }

      function changeUsername() {
	setUsername(getUsernameFromCookie(), prompt('What is your username?'));
      }

      function showStatus() {
	document.getElementById("status").style.display = 'block';
      }

      function hideStatus() {
	document.getElementById("status").style.display = 'none';
      }

      function updateStats(pass, fail, skip, usedMemory, force) {
	if (pass || force) {
	  passCount += pass;
	  passCountEl.innerHTML = passCount;
	}
	if (fail || force) {
	  failCount += fail;
	  failCountEl.innerHTML = failCount;
	}
	if (skip || force) {
	  skipCount += skip;
	  skipCountEl.innerHTML = skipCount;
	}
	usedMemoryEl.innerHTML = usedMemory;

	var completedCount = passCount + failCount + skipCount;
	var elapsed = (new Date().getTime() / 1000) - startTime;
	var completionPercent = completedCount / totalCount;
        var estimatedTotalTime = elapsed / completionPercent;
	var estimatedRemainingTime = (1 - completionPercent) * estimatedTotalTime;
	estimatedRemainingTime = Math.round(estimatedRemainingTime);
	estimatedTimeRemainingEl.innerHTML = estimatedRemainingTime + " seconds";
      }

      var startTime = new Date().getTime() / 1000;
      var passCount = failCount = skipCount = 0;
      var totalCount = <?php print $totalTests; ?>;
      var passCountEl = document.getElementById('pass_count');
      var failCountEl = document.getElementById('fail_count');
      var skipCountEl = document.getElementById('skip_count');
      var estimatedTimeRemainingEl = document.getElementById('estimated_time_remaining');
      var usedMemoryEl = document.getElementById('used_memory');
      document.getElementById('total_count').innerHTML = totalCount;
      updateStats(0, 0, 0, 0, 1);
    </script>

    <input type="button" onclick="reRun();" value="Re-run broken tests"
     id="runBrokenButton" style="display:none"/>
  </div>

  <?php
    $result = new GalleryTestResult();
    $testSuite->run($result, $range);
    $result->report();
  ?>
  </body>
</html>
