<?php

function myTemplateTags() {
} // Bad, using camel caps.

function my_template_tags() {
} // Good.

function _my_template_tags() {
} // OK.

function __my_template_tags() {
} // Bad.

class My_Plugin {
	public static function __my_init() {
	}

	// Bad, only PHP magic methods should be prefixed with double underscore.

	public function myInit() {
	}

	// Bad.

	public static function _my_init() {
	}

	// OK.

	public function my_init() {
	}

	// OK.

	public function __invoke() {
	}

	// OK.
}

// Verify that CamelCase is not checked for extended classes or interfaces.
class Test extends WP_UnitTestCase {
	public function setUp() {
	}

	// OK.
}

class Foo implements ArrayAccess {
	public function offsetSet($key, $value) {
	}

	// OK.

	public function offsetUnset($key) {
	}

	// OK.

	public function offsetExists($key) {
	}

	// OK.

	public function offsetGet($key) {
	}

	// OK.
}

// Verify all PHP supported magic methods.
class Its_A_Kind_Of_Magic {
	public function __construct() {
	}

	// Ok.

	public function __destruct() {
	}

	// Ok.

	public function __call($a, $b) {
	}

	// Ok.

	public static function __callStatic($a, $b) {
	}

	// Ok.

	public function __get($a) {
	}

	// Ok.

	public function __set($a, $b) {
	}

	// Ok.

	public function __isset($a) {
	}

	// Ok.

	public function __unset($a) {
	}

	// Ok.

	public function __sleep() {
	}

	// Ok.

	public function __wakeup() {
	}

	// Ok.

	public function __toString() {
	}

	// Ok.

	public function __set_state() {
	}

	// Ok.

	public function __clone() {
	}

	// Ok.

	public function __invoke() {
	}

	// Ok.

	public function __debugInfo() {
	}

	// Ok.
}

// Verify SoapClient magic methods.
class My_Soap extends SoapClient {
	public function __doRequest() {
	}

	// Ok.

	public function __getFunctions() {
	}

	// Ok.

	public function __getLastRequest() {
	}

	// Ok.

	public function __getLastRequestHeaders() {
	}

	// Ok.

	public function __getLastResponse() {
	}

	// Ok.

	public function __getLastResponseHeaders() {
	}

	// Ok.

	public function __getTypes() {
	}

	// Ok.

	public function __setCookie() {
	}

	// Ok.

	public function __setLocation() {
	}

	// Ok.

	public function __setSoapHeaders() {
	}

	// Ok.

	public function __soapCall() {
	}

	// Ok.
}

class My_Soap {
	public function __doRequest() {
	}

	// Bad.

	private function __getFunctions() {
	}

	// Bad.

	protected function __getLastRequest() {
	}

	// Bad.

	public function __getLastRequestHeaders() {
	}

	// Bad.

	public function __getLastResponse() {
	}

	// Bad.

	public function __getLastResponseHeaders() {
	}

	// Bad.

	public function __getTypes() {
	}

	// Bad.

	public function __setCookie() {
	}

	// Bad.

	public function __setLocation() {
	}

	// Bad.

	public function __setSoapHeaders() {
	}

	// Bad.

	public function __soapCall() {
	}

	// Bad.
}

class My_Soap extends somethingElse {
	public function __doRequest() {
	}

	// Ok - as somethingElse might be extended from SoapClient again.

	private function __getFunctions() {
	}

	// Ok.

	protected function __getLastRequest() {
	}

	// Ok.

	public function __getLastRequestHeaders() {
	}

	// Ok.

	public function __getLastResponse() {
	}

	// Ok.

	public function __getLastResponseHeaders() {
	}

	// Ok.

	public function __getTypes() {
	}

	// Ok.

	public function __setCookie() {
	}

	// Ok.

	public function __setLocation() {
	}

	// Ok.

	public function __setSoapHeaders() {
	}

	// Ok.

	public function __soapCall() {
	}

	// Ok.
}
