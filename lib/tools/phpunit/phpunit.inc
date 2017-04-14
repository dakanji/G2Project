<?php
//
// PHP framework for testing, based on the design of "JUnit".
//
// Written by Fred Yankowski <fred@ontosys.com>
//            OntoSys, Inc  <http://www.OntoSys.com>
//
// $Id: phpunit.inc 20957 2009-12-16 04:57:07Z mindless $

// Copyright (c) 2000 Fred Yankowski

// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

    error_reporting(E_ALL & ~E_STRICT);

/*
interface Test {
  function run(&$aTestResult);
  function countTestCases();
}
*/

function trace($msg) {
  return;
  print($msg);
  flush();
}

if (phpversion() >= '4') {
    function PHPUnit_error_handler($errno, $errstr, $errfile, $errline) {
	global $PHPUnit_testRunning;

	if ($errno == 2048) {
	    /*
	     * We're ignoring E_STRICT for now because of PHP 5's retarded inability
	     * to allow us to use standard PHP4 constructs like 'var'.
	     */
	    return;
	}

	if (!(error_reporting() & 8192) && $errno == 8192) {
	    /*
	     * We're ignoring E_DEPRECATED to avoid the "Assigning the return value of new by
	     * reference" deprecation warning.
	     */
	    return;
	}

	/*
	 * http://us4.php.net/file_exists#47860
	 * Older php versions generate stat warnings whenever a file_exists or
	 * other filesystem function stats a file that doesn't exist.  The functions
	 * still work correctly.  Allow unit tests to ignore these warnings.
	 */
	if (version_compare(phpversion(), '4.3.3') <= 0 &&
		preg_match('{stat failed for.*\(errno=0 -}', $errstr)) {
	    return;
	}

	/*
	 * Smarty smarty_core_write_file does @unlink() on Windows platforms without doing a
	 * file_exists. Ignore these errors.
	 */
	if (error_reporting() == 0 && DIRECTORY_SEPARATOR == '\\') {
	    global $gallery;
	    if (isset($gallery)) {
		$templateDir = $gallery->getConfig('data.smarty.templates_c');
		if (strpos($errstr, 'unlink(' . $templateDir) !== false) {
		    return;
		}
	    }
	}

	$message = "<B>PHP ERROR:</B> ".$errstr;
	if (function_exists('debug_backtrace')) {
	    $traceSource = debug_backtrace();
	    if (phpversion() < '5') {
		array_shift($traceSource);
	    }
	    foreach ($traceSource as $traceEntry) {
		/* Trace with call_user_func may miss file/line in an entry */
		$buf = ' <B>in</B> ' . (isset($traceEntry['file']) ? $traceEntry['file'] : '??');
		if (isset($traceEntry['class']) && isset($traceEntry['function'])) {
		    $buf .= " ($traceEntry[class]::$traceEntry[function]) ";
		} else if (isset($traceEntry['function'])) {
		    $buf .= " ($traceEntry[function]) ";
		}
		$buf .= ' <B>at line</B> ' . (isset($traceEntry['line']) ? $traceEntry['line'] : '??');
		$trace[] = $buf;
	    }
	    $message .= join('<BR>', $trace);
	} else {
	    $message .=" <B>in</B> ".$errfile." <B>at line</B> ".$errline;
	}
	$PHPUnit_testRunning[0]->fail($message);
    }
}

class PHPUnitException {
    /* Emulate a Java exception, sort of... */
  var $message;
  var $type;
  function PHPUnitException($message, $type = 'FAILURE') {
    $this->message = $message;
    $this->type = $type;
  }
  function getMessage() {
    return $this->message;
  }
  function getType() {
    return $this->type;
  }
}

class Assert {
  function assert($boolean, $message=0) {
    if (! $boolean)
      $this->fail($message);
  }

  function assertEquals($expected, $actual, $message=0) {
      if (gettype($expected) != gettype($actual)) {
	  $this->failNotEquals($expected, $actual, "expected", $message);
	  return;
      }
      if (phpversion() < '4') {
	  if (is_object($expected) or is_object($actual)
	      or is_array($expected) or is_array($actual)) {
	      $this->error("INVALID TEST: cannot compare arrays or objects in PHP3");
	      return;
	  }
      }
      if (phpversion() >= '4' && is_object($expected)) {
	  if (get_class($expected) != get_class($actual)) {
	      $this->failNotEquals($expected, $actual, "expected", $message);
	      return;
	  }
	  if (method_exists($expected, "equals")) {
	      if (! $expected->equals($actual)) {
		  $this->failNotEquals($expected, $actual, "expected", $message);
	      }
	      return;		// no further tests after equals()

	  }
      }
      if (phpversion() >= '4.0.4') {
	  if (is_null($expected) != is_null($actual)) {
	      $this->failNotEquals($expected, $actual, "expected", $message);
	      return;
	  }
      }
      if ($expected != $actual) {
	  $this->failNotEquals($expected, $actual, "expected", $message);
      }
  }

  function assertRegexp($regexp, $actual, $message=false) {
    if (! preg_match($regexp, $actual)) {
      $this->failNotEquals($regexp, $actual, "pattern", $message);
    }
  }

  function assertEqualsMultilineStrings($string0, $string1,
  $message="") {
    $lines0 = explode("\n",$string0);
    $lines1 = explode("\n",$string1);
    if (sizeof($lines0) != sizeof($lines1)) {
      $this->failNotEquals(sizeof($lines0)." line(s)",
                           sizeof($lines1)." line(s)", "expected", $message);
    }
    for($i=0; $i< sizeof($lines0); $i++) {
      $this->assertEquals(trim($lines0[$i]),
                          trim($lines1[$i]),
                          "line ".($i+1)." of multiline strings differ. ".$message);
    }
  }

  function _formatValue($value, $class="") {
      if (phpversion() >= '4.0.0') {
	  if (is_object($value)) {
	      if (method_exists($value, "toString") ) {
		  $translateValue = htmlspecialchars($value->toString());
	      }
	      else {
		  $translateValue = htmlspecialchars(serialize($value));
	      }
	  }
	  else if (is_array($value)) {
	      if (phpversion() >= '4.2.0') {
		  $translateValue = '<pre>' . htmlspecialchars(var_export($value, 1)) . '</pre>';
	      } else {
		  $translateValue = htmlspecialchars(serialize($value));
	      }
	  } else {
	      $translateValue = htmlspecialchars($value);
	  }
      }
      else {
	  $translateValue = $value;
      }
      $htmlValue = "<code class=\"$class\">" . $translateValue . "</code>";
      if (phpversion() >= '4.0.0') {
	  if (is_bool($value)) {
	      $htmlValue = $value ? "<i>true</i>" : "<i>false</i>";
	  }
	  elseif (phpversion() >= '4.0.4' && is_null($value)) {
	      $htmlValue = "<i>null</i>";
	  }
	  $htmlValue .= "&nbsp;&nbsp;&nbsp;<span class=\"typeinfo\">";
	  $htmlValue .= "type:" . gettype($value);
	  $htmlValue .= is_object($value) ? ", class:" . get_class($value) : "";
	  $htmlValue .= "</span>";
      }
      return $htmlValue;
  }

  function failNotEquals($expected, $actual, $expected_label, $message=0) {
    // Private function for reporting failure to match.
    $str = $message ? ($message . ' <br>') : '';
    //$str .= "($expected_label/actual)<br>";
    $str .= sprintf("%s<br>%s",
		    $this->_formatValue($expected, "expected"),
		    $this->_formatValue($actual, "actual"));
    $this->fail($str);
  }
}

class TestCase extends Assert /* implements Test */ {
  /* Defines context for running tests.  Specific context -- such as
     instance variables, global variables, global state -- is defined
     by creating a subclass that specializes the setUp() and
     tearDown() methods.  A specific test is defined by a subclass
     that specializes the runTest() method. */
  var $fName;
  var $fClassName;
  var $fModuleId;
  var $fResult;
  var $fExceptions = array();
  var $fTiming;
  var $fLifeCycle = array();
  var $fSkipped = false;

  function TestCase($name) {
    $this->fName = $name;
  }

  function setModuleId($moduleId) {
      $this->fModuleId = $moduleId;
  }

  function getModuleId() {
      return $this->fModuleId;
  }

  function run(&$testResult, $ranges) {
    /* Run this single test, by calling the run() method of the
       TestResult object which will in turn call the runBare() method
       of this object.  That complication allows the TestResult object
       to do various kinds of progress reporting as it invokes each
       test.  Create/obtain a TestResult object if none was passed in.
       Note that if a TestResult object was passed in, it must be by
       reference. */
    $this->fResult = $testResult;
    $testResult->run($this, $ranges);
    $this->fResult = 0;
    return $testResult;
  }

  function classname() {
	  if (isset($this->fClassName)) {
		return $this->fClassName;
	  } else {
		return get_class($this);
	  }
  }

  function countTestCases() {
    return 1;
  }

  function runTest() {
    if (phpversion() >= '4') {
	global $PHPUnit_testRunning;
	eval('$PHPUnit_testRunning[0] = & $this;');
	// Saved ref to current TestCase, so that the error handler
	// can access it.  This code won't even parse in PHP3, so we
	// hide it in an eval.

	$old_handler = set_error_handler("PHPUnit_error_handler");
	// errors will now be handled by our error handler
      }

      $name = $this->name();
      if (phpversion() >= '4' && ! method_exists($this, $name)) {
          $this->error("Method '$name' does not exist");
      } else {
          $ret = $this->$name();
	  if ($ret) {
	      global $gallery;
	      $this->fail($ret->getAsHtml() . $gallery->getDebugBuffer());
	  }
      }

      if (phpversion() >= '4') {
	  // Remove PHPUnit_error_handler AND any handlers added by test
	  while (true) {
	      // Call set/restore just to see what the current error handler is:
	      $old_handler = set_error_handler('PHPUnit_error_handler');
	      restore_error_handler();
	      // Remove current error handler:
	      restore_error_handler();
	      // If we just removed PHPUnit_error_handler then previous state is restored.
	      // If all error handlers are gone then test must have called restore_error_handler
	      // (preferably they don't do that).
	      if ($old_handler == 'PHPUnit_error_handler' || empty($old_handler)) {
		  break;
	      }
	  }
	  // Original code:
	  //if (isset($old_handler)) {
	  //    set_error_handler($old_handler); // revert to prior error handler
	  //} else {
	  //    restore_error_handler();
	  //}
	  $PHPUnit_testRunning = null;
      }
  }

  function setUp() /* expect override */ {
      //print("TestCase::setUp()<br>\n");
      $this->fLifeCycle['setUp'] = 1;
      return null;
  }

  function tearDown() /* possible override */ {
      //print("TestCase::tearDown()<br>\n");
      $this->fLifeCycle['tearDown'] = 1;
      return null;
  }

  ////////////////////////////////////////////////////////////////


  function _createResult() /* protected */ {
    /* override this to use specialized subclass of TestResult */
    return new TestResult;
  }

  function fail($message=0) {
    //printf("TestCase::fail(%s)<br>\n", ($message) ? $message : '');
    /* JUnit throws AssertionFailedError here.  We just record the
       failure and carry on */
    $this->fExceptions[] = new PHPUnitException($message, 'FAILURE');
  }

  function error($message) {
    /* report error that requires correction in the test script
       itself, or (heaven forbid) in this testing infrastructure */
    $this->fExceptions[] = new PHPUnitException($message, 'ERROR');
    $this->fResult->stop();	// [does not work]
  }

  function failed() {
      reset($this->fExceptions);
      while (list($key, $exception) = each($this->fExceptions)) {
	  if ($exception->type == 'FAILURE')
	      return true;
      }
      return false;
  }
  function errored() {
      reset($this->fExceptions);
      while (list($key, $exception) = each($this->fExceptions)) {
	  if ($exception->type == 'ERROR')
	      return true;
      }
      return false;
  }

  function elapsed() {
      list ($startMicro, $startSecs) = explode(' ', $this->fTiming['start']);
      list ($stopMicro, $stopSecs) = explode(' ', $this->fTiming['stop']);
      return ($stopSecs - $startSecs) + ($stopMicro - $startMicro);
  }

  function getExceptions() {
    return $this->fExceptions;
  }

  function name() {
    return $this->fName;
  }

  function skip() {
    $this->fSkipped = true;
  }

  function wasSkipped() {
    return $this->fSkipped;
  }

  function runBare() {
    global $gallery;
    $this->fTiming['start'] = microtime();
    $ret = $this->setUp();
    if ($ret) {
	$this->fail($ret->getAsHtml() . $gallery->getDebugBuffer());
    }
    $this->assert(!empty($this->fLifeCycle['setUp']), 'TestSuite setUp() method did not call parent::setUp()');
    if (!($this->errored() || $this->failed())) {
	$this->runTest();
    }
    $ret = $this->tearDown();
    if ($ret) {
	$this->fail($ret->getAsHtml() . $gallery->getDebugBuffer());
    }
    $this->assert(!empty($this->fLifeCycle['tearDown']), 'TestSuite tearDown() method did not call parent::tearDown()');
    $this->fTiming['stop'] = microtime();
  }
}


class TestSuite /* implements Test */ {
  /* Compose a set of Tests (instances of TestCase or TestSuite), and
     run them all. */
  var $fTests = array();
  var $fClassname;
  var $fModuleId;

  function TestSuite($classname=false,$moduleid=false,$filter=null) {
    // Find all methods of the given class whose name starts with
    // "test" and add them to the test suite.

    // PHP3: We are just _barely_ able to do this with PHP's limited
    // introspection...  Note that PHP seems to store method names in
    // lower case, and we have to avoid the constructor function for
    // the TestCase class superclass.  Names of subclasses of TestCase
    // must not start with "Test" since such a class will have a
    // constructor method name also starting with "test" and we can't
    // distinquish such a construtor from the real test method names.
    // So don't name any TestCase subclasses as "Test..."!

    // PHP4:  Never mind all that.  We can now ignore constructor
    // methods, so a test class may be named "Test...".

    if (empty($classname))
      return;
    $this->fClassname = $classname;
    $this->fModuleId = $moduleid;
    $this->fFilter = $filter;


    // PHP4 ONLY - introspection, submitted by Dylan Kuhn

    $names = get_class_methods($classname);
    while (list($key, $method) = each($names)) {
      if (preg_match('/^test/', $method)) {
        $test = new $classname($method);
        if (strcasecmp($method, $classname) == 0 || is_subclass_of($test, $method)) {
          // Ignore the given method name since it is a constructor:
          // it's the name of our test class or it is the name of a
          // superclass of our test class.  (This code smells funny.
          // Anyone got a better way?)

          //print "skipping $method<br>";
	} else {
          if (substr($this->fFilter, 0, 2) == '!(' & substr($this->fFilter, -1, 1) == ')') {
	    $newFilter = substr($this->fFilter, 1);
            if (!isset($this->fFilter) || !preg_match("/{$newFilter}/i", "$moduleid.$classname.$method")) {
              $test->setModuleId($this->fModuleId);
              $this->addTest($test);
            }
	  } else {
	    if (!isset($this->fFilter) || preg_match("/{$this->fFilter}/i", "$moduleid.$classname.$method")) {
	      $test->setModuleId($this->fModuleId);
	      $this->addTest($test);
	    }
	  }
        }
      }
    }
  }

  function addTest($test) {
    /* Add TestCase or TestSuite to this TestSuite */
    $this->fTests[] = $test;
  }

  function run(&$testResult, $ranges) {
    /* Run all TestCases and TestSuites comprising this TestSuite,
       accumulating results in the given TestResult object. */
    reset($this->fTests);
    while (list($na, $test) = each($this->fTests)) {
      if ($testResult->shouldStop())
	break;
      $test->run($testResult, $ranges);
    }
  }

  function countTestCases() {
    /* Number of TestCases comprising this TestSuite (including those
       in any constituent TestSuites) */
    $count = 0;
    reset($this->fTests);
    while (list($na, $test_case) = each($this->fTests)) {
      $count += $test_case->countTestCases();
    }
    return $count;
  }
}


class TestFailure {
  /* Record failure of a single TestCase, associating it with the
     exception that occurred */
  var $fFailedTestName;
  var $fFailedClassName;
  var $fException;

  function TestFailure(&$test, &$exception) {
    $this->fFailedClassName = $test->classname();
    $this->fFailedTestName = $test->name();
    $this->fException = $exception;
  }

  function getExceptions() {
      // deprecated
      return array($this->fException);
  }
  function getException() {
      return $this->fException;
  }

  function getTestName() {
    return $this->fFailedTestName;
  }

  function getClassName() {
    return $this->fFailedClassName;
  }
}


class TestResult {
  /* Collect the results of running a set of TestCases. */
  var $fFailures = array();
  var $fErrors = array();
  var $fRunTests = 0;
  var $fUnskippedTests = 0;
  var $fStop = false;

  function TestResult() { }

  function _endTest(&$test) /* protected */ {
      /* specialize this for end-of-test action, such as progress
	 reports  */
  }

  function addError($test, $exception) {
      $this->fErrors[] = new TestFailure($test, $exception);
  }

  function addFailure($test, $exception) {
      $this->fFailures[] = new TestFailure($test, $exception);
  }

  function getFailures() {
    return $this->fFailures;
  }

  function run(&$test, $ranges) {
    /* Run a single TestCase in the context of this TestResult */
    $count = ++$this->fRunTests;
    $skipFlag = true;
    foreach ($ranges as $tempRange) {
        if ($count >= $tempRange[0] && $count <= $tempRange[1]) {
            $skipFlag = false;
        }
    }
    if ($skipFlag) {
        $test->skip();
    }
    $this->_startTest($test);

    if (!$test->wasSkipped()) {
	$this->fUnskippedTests++;
	$test->runBare();
    }

    /* this is where JUnit would catch AssertionFailedError */
    $exceptions = $test->getExceptions();
    reset($exceptions);
    while (list($key, $exception) = each($exceptions)) {
	if ($exception->type == 'ERROR')
	    $this->addError($test, $exception);
	else if ($exception->type == 'FAILURE')
	    $this->addFailure($test, $exception);
    }
    //    if ($exceptions)
    //      $this->fFailures[] = new TestFailure(&$test, &$exceptions);
    $this->_endTest($test);
  }

  function countTests() {
    return $this->fUnskippedTests;
  }

  function shouldStop() {
    return $this->fStop;
  }

  function _startTest($test) /* protected */ {
      /* specialize this for start-of-test actions */
  }

  function stop() {
    /* set indication that the test sequence should halt */
    $fStop = true;
  }

  function errorCount() {
      return count($this->fErrors);
  }
  function failureCount() {
      return count($this->fFailures);
  }
  function countFailures() {
      // deprecated
      return $this->failureCount();
  }
}

class TextTestResult extends TestResult {
  /* Specialize TestResult to produce text/html report */
  function TextTestResult() {
    $this->TestResult();  // call superclass constructor
  }

  function report() {
    /* report result of test run */
    $nRun = $this->countTests();
    $nFailures = $this->failureCount();
    $nErrors = $this->errorCount();
    printf("<p>%s test%s run<br>", $nRun, ($nRun == 1) ? '' : 's');
    printf("%s failure%s<br>\n", $nFailures, ($nFailures == 1) ? '' : 's');
    printf("%s error%s.<br>\n", $nErrors, ($nErrors == 1) ? '' : 's');

    if ($nFailures > 0) {
	print("<h2>Failures</h2>");
	print("\n<ol>\n");
	$failures = $this->getFailures();
	while (list($i, $failure) = each($failures)) {
	    $failedTestName = $failure->getTestName();
	    printf("<li>%s</li>\n", $failedTestName);

	    $exceptions = $failure->getExceptions();
	    print("\n<ul>");
	    while (list($na, $exception) = each($exceptions)) {
                printf("<li>%s</li>\n", $exception->getMessage());
            }
	    print("</ul>");
	}
	print("</ol>\n");
    }

    if ($nErrors > 0) {
	print("<h2>Errors</h2>");
	print("\n<ol>\n");
	reset($this->fErrors);
	while (list($i, $error) = each($this->fErrors)) {
	    $erroredTestName = $error->getTestName();
	    printf("<li>%s</li>\n", $failedTestName);

	    $exception = $error->getException();
	    print("\n<ul>");
	    printf("<li>%s</li>\n", $exception->getMessage());
	    print("</ul>");
	}
	print("</ol>\n");

    }
  }

  function _startTest($test) {
      if (phpversion() > '4') {
	  printf("%s - %s ", get_class($test), $test->name());
      } else {
	  printf("%s ", $test->name());
      }
    flush();
  }

  function _endTest($test) {
    $outcome = $test->failed()
       ? "<font color=\"red\">FAIL</font>"
       : "<font color=\"green\">ok</font>";
    printf("$outcome<br>\n");
    flush();
  }
}

// PrettyTestResult created by BJG 17/11/01
// beacuse the standard test result provided looks
// rubbish.
class PrettyTestResult extends TestResult {

    var $_totalElapsed = 0;

  /* Specialize TestResult to produce text/html report */
  function PrettyTestResult() {
    $this->TestResult();  // call superclass constructor
	echo "<h2>Test Results</h2>";

	echo "<TABLE CELLSPACING=\"1\" CELLPADDING=\"1\" BORDER=\"0\" WIDTH=\"90%\" ALIGN=\"CENTER\" class=\"details\">";
	echo "<TR><TH>#</TH><TH>Module</TH><TH>Class</TH><TH>Function</TH><TH>Success?</TH><TH>Time</TH></TR>";
  }

  function report() {
	echo "</TABLE>";
    /* report result of test run */
    $nRun = $this->countTests();
    $nFailures = $this->countFailures();
	echo "<h2>Summary</h2>";

    printf("<p>%2.4f seconds elapsed</p>", $this->_totalElapsed);
    printf("<p>%s test%s run</p>", $nRun, ($nRun == 1) ? '' : 's');
    printf("<p>%s failure%s.</p>\n", $nFailures, ($nFailures == 1) ? '' : 's');
    if ($nFailures == 0)
      return;

    $failures = $this->getFailures();
    $newFilter = array();
    foreach ($failures as $failure) {
	$newFilter[$failure->getClassName() . '.' . $failure->getTestName()] = 1;
    }
    printf('<input type="button" onClick="javascript:reRun(\'%s\');" value="Re-run broken tests">',
	   '(' . join('|', array_keys($newFilter)) . ')$');

    echo "<h2>Failure Details</h2>";
    print("\n<ol>\n");
    reset($failures);
    while (list($i, $failure) = each($failures)) {
      $failedTestName = $failure->getTestName();
      printf("\n<li>%s</li>\n", $failedTestName);

      $exceptions = $failure->getExceptions();
      print("<ul>");
      while (list($na, $exception) = each($exceptions)) {
	printf("\n<li>%s</li>\n", $exception->getMessage());
      }
      print("</ul>");
    }
    print("</ol>\n");
  }

  function _startTest($test) {
      if ($test->wasSkipped()) {
	  $style = ' style="text-decoration: line-through;"';
      } else {
	  $style = '';
      }
      printf("<TR><TD$style>%d</TD><TD$style>%s </TD><TD$style>%s </TD><TD$style>%s </TD>", $this->fRunTests, $test->getModuleId(), $test->classname(),$test->name());
    flush();
  }

  function _endTest($test) {
    $outcome = $test->failed()
       ? " class=\"Failure\">FAIL"
       : " class=\"Pass\">OK";

    if ($test->wasSkipped()) {
	$outcome = ' class="Skipped">SKIP';
	$elapsed = '0.0000';
    } else {
	$elapsed = sprintf("%2.4f", $test->elapsed());
	$this->_totalElapsed += $elapsed;
    }
    printf("<TD$outcome</TD><TD>$elapsed</TD></TR>");
    flush();
  }
}

class TestRunner {
  /* Run a suite of tests and report results. */
  function run(&$suite, $ranges) {
      $result = new TextTestResult;
      $suite->run($result, $ranges);
      $result->report();
  }
}

?>
