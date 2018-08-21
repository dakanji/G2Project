<?php
/*
@version   v5.20.12  30-Mar-2018
@copyright (c) 2000-2013 John Lim (jlim#natsoft.com). All rights reserved.
@copyright (c) 2014      Damien Regad, Mark Newnham and the ADOdb community
  Released under both BSD license and Lesser GPL library license.
  Whenever there is any discrepancy between the two licenses,
  the BSD license will take precedence. See License.txt.
  Set tabs to 4 for best viewing.

  Latest version is available at http://adodb.sourceforge.net

  Microsoft Access data driver. Requires ODBC. Works only on MS Windows.
*/
if (!defined('_ADODB_ODBC_LAYER')) {
	if (!defined('ADODB_DIR')) {
		die();
	}

	include ADODB_DIR . '/drivers/adodb-odbc.inc.php';
}

if (!defined('_ADODB_ACCESS')) {
	define('_ADODB_ACCESS', 1);

	class ADODB_access extends ADODB_odbc {
		public $databaseType = 'access';

		// support mssql SELECT TOP 10 * FROM TABLE
		public $hasTop  = 'top';
		public $fmtDate = '#Y-m-d#';

		// note not comma
		public $fmtTimeStamp = '#Y-m-d h:i:sA#';

		// strangely enough, setting to true does not work reliably
		public $_bindInputArray = false;
		public $sysDate         = "FORMAT(NOW,'yyyy-mm-dd')";
		public $sysTimeStamp    = 'NOW';
		public $hasTransactions = false;
		public $upperCase       = 'ucase';

		public function __construct() {
			global $ADODB_EXTENSION;

			$ADODB_EXTENSION = false;
			parent::__construct();
		}

		public function Time() {
			return time();
		}

		public function BeginTrans() {
			return false;
		}

		public function IfNull($field, $ifNull) {
			// if Access
			return " IIF(IsNull($field), $ifNull, $field) ";
		}
	}

	class ADORecordSet_access extends ADORecordSet_odbc {
		public $databaseType = 'access';

		public function __construct($id, $mode = false) {
			return parent::__construct($id, $mode);
		}
	}
}
