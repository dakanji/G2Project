<?php
/*
@version   v5.20.12  30-Mar-2018
@copyright (c) 2000-2013 John Lim (jlim#natsoft.com). All rights reserved.
@copyright (c) 2014      Damien Regad, Mark Newnham and the ADOdb community
  Released under both BSD license and Lesser GPL library license.
  Whenever there is any discrepancy between the two licenses,
  the BSD license will take precedence.
  Set tabs to 4.

  Currently unsupported: MetaDatabases, MetaTables and MetaColumns, and also inputarr in Execute.
  Native types have been converted to MetaTypes.
  Transactions not supported yet.

  Limitation of url length. For IIS, see MaxClientRequestBuffer registry value.

	  http://support.microsoft.com/default.aspx?scid=kb;en-us;260694
*/

// security - hide paths
if (!defined('ADODB_DIR')) {
	die();
}

if (!defined('_ADODB_CSV_LAYER')) {
	define('_ADODB_CSV_LAYER', 1);

	include_once ADODB_DIR . '/adodb-csvlib.inc.php';

	class ADODB_csv extends ADOConnection {
		public $databaseType     = 'csv';
		public $databaseProvider = 'csv';
		public $hasInsertID      = true;
		public $hasAffectedRows  = true;
		public $fmtTimeStamp     = "'Y-m-d H:i:s'";
		public $_affectedrows    = 0;
		public $_insertid        = 0;
		public $_url;
		public $replaceQuote    = "''"; // string to use to replace quotes
		public $hasTransactions = false;
		public $_errorNo        = false;

		public function __construct() {
		}

		public function _insertid() {
			return $this->_insertid;
		}

		public function _affectedrows() {
			return $this->_affectedrows;
		}

		public function MetaDatabases() {
			return false;
		}

		// returns true or false
		public function _connect($argHostname, $argUsername, $argPassword, $argDatabasename) {
			if (strtolower(substr($argHostname, 0, 7)) !== 'http://') {
				return false;
			}
			$this->_url = $argHostname;

			return true;
		}

		// returns true or false
		public function _pconnect($argHostname, $argUsername, $argPassword, $argDatabasename) {
			if (strtolower(substr($argHostname, 0, 7)) !== 'http://') {
				return false;
			}
			$this->_url = $argHostname;

			return true;
		}

		public function MetaColumns($table, $normalize = true) {
			return false;
		}

		// parameters use PostgreSQL convention, not MySQL
		public function SelectLimit($sql, $nrows = -1, $offset = -1, $inputarr = false, $secs2cache = 0) {
			global $ADODB_FETCH_MODE;

			$nrows  = (int)$nrows;
			$offset = (int)$offset;
			$url    = $this->_url . '?sql=' . urlencode($sql) . "&nrows=$nrows&fetch=" .
			(($this->fetchMode !== false) ? $this->fetchMode : $ADODB_FETCH_MODE) .
			"&offset=$offset";
			$err    = false;
			$rs     = csv2rs($url, $err, false);

			if ($this->debug) {
				echo "$url<br><i>$err</i><br>";
			}

			$at = strpos($err, '::::');

			if ($at === false) {
				$this->_errorMsg = $err;
				$this->_errorNo  = (int)$err;
			} else {
				$this->_errorMsg = substr($err, $at + 4, 1024);
				$this->_errorNo  = -9999;
			}

			if ($this->_errorNo) {
				if ($fn = $this->raiseErrorFn) {
					$fn($this->databaseType, 'EXECUTE', $this->ErrorNo(), $this->ErrorMsg(), $sql, '');
				}
			}

			if (is_object($rs)) {
				$rs->databaseType = 'csv';
				$rs->fetchMode    = ($this->fetchMode !== false) ? $this->fetchMode : $ADODB_FETCH_MODE;
				$rs->connection   = $this;
			}

			return $rs;
		}

		// returns queryID or false
		public function _Execute($sql, $inputarr = false) {
			global $ADODB_FETCH_MODE;

			if (!$this->_bindInputArray && $inputarr) {
				$sqlarr = explode('?', $sql);
				$sql    = '';
				$i      = 0;

				foreach ($inputarr as $v) {
					$sql .= $sqlarr[$i];

					if (gettype($v) == 'string') {
						$sql .= $this->qstr($v);
					} elseif ($v === null) {
						$sql .= 'NULL';
					} else {
						$sql .= $v;
					}
					$i += 1;
				}
				$sql .= $sqlarr[$i];

				if ($i + 1 != sizeof($sqlarr)) {
					echo 'Input Array does not match ?: ' . htmlspecialchars($sql);
				}
				$inputarr = false;
			}

			$url = $this->_url . '?sql=' . urlencode($sql) . '&fetch=' .
			(($this->fetchMode !== false) ? $this->fetchMode : $ADODB_FETCH_MODE);
			$err = false;


			$rs = csv2rs($url, $err, false);

			if ($this->debug) {
				echo urldecode($url) . "<br><i>$err</i><br>";
			}
			$at = strpos($err, '::::');

			if ($at === false) {
				$this->_errorMsg = $err;
				$this->_errorNo  = (int)$err;
			} else {
				$this->_errorMsg = substr($err, $at + 4, 1024);
				$this->_errorNo  = -9999;
			}

			if ($this->_errorNo) {
				if ($fn = $this->raiseErrorFn) {
					$fn($this->databaseType, 'EXECUTE', $this->ErrorNo(), $this->ErrorMsg(), $sql, $inputarr);
				}
			}

			if (is_object($rs)) {
				$rs->fetchMode = ($this->fetchMode !== false) ? $this->fetchMode : $ADODB_FETCH_MODE;

				$this->_affectedrows = $rs->affectedrows;
				$this->_insertid     = $rs->insertid;
				$rs->databaseType    = 'csv';
				$rs->connection      = $this;
			}

			return $rs;
		}

		// Returns: the last error message from previous database operation
		public function ErrorMsg() {
			return $this->_errorMsg;
		}

		// Returns: the last error number from previous database operation
		public function ErrorNo() {
			return $this->_errorNo;
		}

		// returns true or false
		public function _close() {
			return true;
		}
	} // class

	class ADORecordset_csv extends ADORecordset {
		public function __construct($id, $mode = false) {
			parent::__construct($id, $mode);
		}

		public function _close() {
			return true;
		}
	}
} // define
