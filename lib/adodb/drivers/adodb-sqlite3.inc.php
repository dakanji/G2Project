<?php
/*
@version   v5.20.12  30-Mar-2018
@copyright (c) 2000-2013 John Lim (jlim#natsoft.com). All rights reserved.
@copyright (c) 2014      Damien Regad, Mark Newnham and the ADOdb community
  Released under both BSD license and Lesser GPL library license.
  Whenever there is any discrepancy between the two licenses,
  the BSD license will take precedence.

  Latest version is available at http://adodb.sourceforge.net

  SQLite info: http://www.hwaci.com/sw/sqlite/

  Install Instructions:
  ====================
  1. Place this in adodb/drivers
  2. Rename the file, remove the .txt prefix.
*/

// security - hide paths
if (!defined('ADODB_DIR')) {
	die();
}

class ADODB_sqlite3 extends ADOConnection {
	public $databaseType    = 'sqlite3';
	public $replaceQuote    = "''"; // string to use to replace quotes
	public $concat_operator = '||';
	public $_errorNo        = 0;
	public $hasLimit        = true;
	public $hasInsertID     = true;        /// supports autoincrement ID?
	public $hasAffectedRows = true;    /// supports affected rows for update/delete?
	public $metaTablesSQL   = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name";
	public $sysDate         = "adodb_date('Y-m-d')";
	public $sysTimeStamp    = "adodb_date('Y-m-d H:i:s')";
	public $fmtTimeStamp    = "'Y-m-d H:i:s'";

	public function __construct() {
	}

	public function ServerInfo() {
		$version            = SQLite3::version();
		$arr['version']     = $version['versionString'];
		$arr['description'] = 'SQLite 3';

		return $arr;
	}

	public function BeginTrans() {
		if ($this->transOff) {
			return true;
		}
		$ret             = $this->Execute('BEGIN TRANSACTION');
		$this->transCnt += 1;

		return true;
	}

	public function CommitTrans($ok = true) {
		if ($this->transOff) {
			return true;
		}

		if (!$ok) {
			return $this->RollbackTrans();
		}
		$ret = $this->Execute('COMMIT');

		if ($this->transCnt > 0) {
			$this->transCnt -= 1;
		}

		return !empty($ret);
	}

	public function RollbackTrans() {
		if ($this->transOff) {
			return true;
		}
		$ret = $this->Execute('ROLLBACK');

		if ($this->transCnt > 0) {
			$this->transCnt -= 1;
		}

		return !empty($ret);
	}

	// mark newnham
	public function MetaColumns($table, $normalize = true) {
		global $ADODB_FETCH_MODE;
		$false            = false;
		$save             = $ADODB_FETCH_MODE;
		$ADODB_FETCH_MODE = ADODB_FETCH_ASSOC;

		if ($this->fetchMode !== false) {
			$savem = $this->SetFetchMode(false);
		}
		$rs = $this->Execute("PRAGMA table_info('$table')");

		if (isset($savem)) {
			$this->SetFetchMode($savem);
		}

		if (!$rs) {
			$ADODB_FETCH_MODE = $save;

			return $false;
		}
		$arr = array();

		while ($r = $rs->FetchRow()) {
			$type = explode('(', $r['type']);
			$size = '';

			if (sizeof($type) == 2) {
				$size = trim($type[1], ')');
			}
			$fn                 = strtoupper($r['name']);
			$fld                = new ADOFieldObject();
			$fld->name          = $r['name'];
			$fld->type          = $type[0];
			$fld->max_length    = $size;
			$fld->not_null      = $r['notnull'];
			$fld->default_value = $r['dflt_value'];
			$fld->scale         = 0;

			if (isset($r['pk']) && $r['pk']) {
				$fld->primary_key = 1;
			}

			if ($save == ADODB_FETCH_NUM) {
				$arr[] = $fld;
			} else {
				$arr[strtoupper($fld->name)] = $fld;
			}
		}
		$rs->Close();
		$ADODB_FETCH_MODE = $save;

		return $arr;
	}

	public function _init($parentDriver) {
		$parentDriver->hasTransactions = false;
		$parentDriver->hasInsertID     = true;
	}

	public function _insertid() {
		return $this->_connectionID->lastInsertRowID();
	}

	public function _affectedrows() {
		return $this->_connectionID->changes();
	}

	public function ErrorMsg() {
		if ($this->_logsql) {
			return $this->_errorMsg;
		}

		return ($this->_errorNo) ? $this->ErrorNo() : ''; //**tochange?
	}

	public function ErrorNo() {
		return $this->_connectionID->lastErrorCode(); //**tochange??
	}

	public function SQLDate($fmt, $col = false) {
		$fmt = $this->qstr($fmt);

		return ($col) ? "adodb_date2($fmt,$col)" : "adodb_date($fmt)";
	}

	public function _createFunctions() {
		$this->_connectionID->createFunction('adodb_date', 'adodb_date', 1);
		$this->_connectionID->createFunction('adodb_date2', 'adodb_date2', 2);
	}

	// returns true or false
	public function _connect($argHostname, $argUsername, $argPassword, $argDatabasename) {
		if (empty($argHostname) && $argDatabasename) {
			$argHostname = $argDatabasename;
		}
		$this->_connectionID = new SQLite3($argHostname);
		$this->_createFunctions();

		return true;
	}

	// returns true or false
	public function _pconnect($argHostname, $argUsername, $argPassword, $argDatabasename) {
		// There's no permanent connect in SQLite3
		return $this->_connect($argHostname, $argUsername, $argPassword, $argDatabasename);
	}

	// returns query ID if successful, otherwise false
	public function _query($sql, $inputarr = false) {
		$rez = $this->_connectionID->query($sql);

		if ($rez === false) {
			$this->_errorNo = $this->_connectionID->lastErrorCode();
		}
		// If no data was returned, we don't need to create a real recordset
		elseif ($rez->numColumns() == 0) {
			$rez->finalize();
			$rez = true;
		}

		return $rez;
	}

	public function SelectLimit($sql, $nrows = -1, $offset = -1, $inputarr = false, $secs2cache = 0) {
		$nrows     = (int)$nrows;
		$offset    = (int)$offset;
		$offsetStr = ($offset >= 0) ? " OFFSET $offset" : '';
		$limitStr  = ($nrows >= 0) ? " LIMIT $nrows" : ($offset >= 0 ? ' LIMIT 999999999' : '');

		if ($secs2cache) {
			$rs = $this->CacheExecute($secs2cache, $sql . "$limitStr$offsetStr", $inputarr);
		} else {
			$rs = $this->Execute($sql . "$limitStr$offsetStr", $inputarr);
		}

		return $rs;
	}

	/*
		This algorithm is not very efficient, but works even if table locking
		is not available.

		Will return false if unable to generate an ID after $MAXLOOPS attempts.
	*/
	public $_genSeqSQL = 'create table %s (id integer)';

	public function GenID($seq = 'adodbseq', $start = 1) {
		// if you have to modify the parameter below, your database is overloaded,
		// or you need to implement generation of id's yourself!
		$MAXLOOPS = 100;
		//$this->debug=1;
		while (--$MAXLOOPS >= 0) {
			@($num = $this->GetOne("select id from $seq"));

			if ($num === false) {
				$this->Execute(sprintf($this->_genSeqSQL, $seq));
				$start -= 1;
				$num    = '0';
				$ok     = $this->Execute("insert into $seq values($start)");

				if (!$ok) {
					return false;
				}
			}
			$this->Execute("update $seq set id=id+1 where id=$num");

			if ($this->affected_rows() > 0) {
				$num        += 1;
				$this->genID = $num;

				return $num;
			}
		}

		if ($fn = $this->raiseErrorFn) {
			$fn($this->databaseType, 'GENID', -32000, "Unable to generate unique id after $MAXLOOPS attempts", $seq, $num);
		}

		return false;
	}

	public function CreateSequence($seqname = 'adodbseq', $start = 1) {
		if (empty($this->_genSeqSQL)) {
			return false;
		}
		$ok = $this->Execute(sprintf($this->_genSeqSQL, $seqname));

		if (!$ok) {
			return false;
		}
		$start -= 1;

		return $this->Execute("insert into $seqname values($start)");
	}

	public $_dropSeqSQL = 'drop table %s';

	public function DropSequence($seqname = 'adodbseq') {
		if (empty($this->_dropSeqSQL)) {
			return false;
		}

		return $this->Execute(sprintf($this->_dropSeqSQL, $seqname));
	}

	// returns true or false
	public function _close() {
		return $this->_connectionID->close();
	}

	public function MetaIndexes($table, $primary = false, $owner = false) {
		$false = false;
		// save old fetch mode
		global $ADODB_FETCH_MODE;
		$save             = $ADODB_FETCH_MODE;
		$ADODB_FETCH_MODE = ADODB_FETCH_NUM;

		if ($this->fetchMode !== false) {
			$savem = $this->SetFetchMode(false);
		}
		$SQL = sprintf("SELECT name,sql FROM sqlite_master WHERE type='index' AND tbl_name='%s'", strtolower($table));
		$rs  = $this->Execute($SQL);

		if (!is_object($rs)) {
			if (isset($savem)) {
				$this->SetFetchMode($savem);
			}
			$ADODB_FETCH_MODE = $save;

			return $false;
		}

		$indexes = array();

		while ($row = $rs->FetchRow()) {
			if ($primary && preg_match('/primary/i', $row[1]) == 0) {
				continue;
			}

			if (!isset($indexes[$row[0]])) {
				$indexes[$row[0]] = array(
					'unique'  => preg_match('/unique/i', $row[1]),
					'columns' => array(),
				);
			}
			/**
			 * There must be a more elegant way of doing this,
			 * the index elements appear in the SQL statement
			 * in cols[1] between parentheses
			 * e.g CREATE UNIQUE INDEX ware_0 ON warehouse (org,warehouse)
			 */
			$cols = explode('(', $row[1]);
			$cols = explode(')', $cols[1]);
			array_pop($cols);
			$indexes[$row[0]]['columns'] = $cols;
		}

		if (isset($savem)) {
			$this->SetFetchMode($savem);
			$ADODB_FETCH_MODE = $save;
		}

		return $indexes;
	}
}

/*--------------------------------------------------------------------------------------
		Class Name: Recordset
--------------------------------------------------------------------------------------*/

class ADORecordset_sqlite3 extends ADORecordSet {
	public $databaseType = 'sqlite3';
	public $bind         = false;

	public function __construct($queryID, $mode = false) {
		if ($mode === false) {
			global $ADODB_FETCH_MODE;
			$mode = $ADODB_FETCH_MODE;
		}

		switch ($mode) {
			case ADODB_FETCH_NUM:
				$this->fetchMode = SQLITE3_NUM;

				break;

			case ADODB_FETCH_ASSOC:
				$this->fetchMode = SQLITE3_ASSOC;

				break;

			default:
				$this->fetchMode = SQLITE3_BOTH;

				break;
		}
		$this->adodbFetchMode = $mode;

		$this->_queryID = $queryID;

		$this->_inited = true;
		$this->fields  = array();

		if ($queryID) {
			$this->_currentRow = 0;
			$this->EOF         = !$this->_fetch();
			@$this->_initrs();
		} else {
			$this->_numOfRows   = 0;
			$this->_numOfFields = 0;
			$this->EOF          = true;
		}

		return $this->_queryID;
	}

	public function FetchField($fieldOffset = -1) {
		$fld             = new ADOFieldObject();
		$fld->name       = $this->_queryID->columnName($fieldOffset);
		$fld->type       = 'VARCHAR';
		$fld->max_length = -1;

		return $fld;
	}

	public function _initrs() {
		$this->_numOfFields = $this->_queryID->numColumns();
	}

	public function Fields($colname) {
		if ($this->fetchMode != SQLITE3_NUM) {
			return $this->fields[$colname];
		}

		if (!$this->bind) {
			$this->bind = array();

			for ($i = 0; $i < $this->_numOfFields; $i++) {
				$o                                = $this->FetchField($i);
				$this->bind[strtoupper($o->name)] = $i;
			}
		}

		return $this->fields[$this->bind[strtoupper($colname)]];
	}

	public function _seek($row) {
		// sqlite3 does not implement seek
		if ($this->debug) {
			ADOConnection::outp('SQLite3 does not implement seek');
		}

		return false;
	}

	public function _fetch($ignore_fields = false) {
		$this->fields = $this->_queryID->fetchArray($this->fetchMode);

		return !empty($this->fields);
	}

	public function _close() {
	}
}
