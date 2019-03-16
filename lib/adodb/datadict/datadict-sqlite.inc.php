<?php

/**
 * @version   v5.20.12  30-Mar-2018
 * @copyright (c) 2000-2013 John Lim (jlim#natsoft.com). All rights reserved.
 * @copyright (c) 2014      Damien Regad, Mark Newnham and the ADOdb community
 * Released under both BSD license and Lesser GPL library license.
 * Whenever there is any discrepancy between the two licenses,
 * the BSD license will take precedence.
 *
 * Set tabs to 4 for best viewing.
 *
 * SQLite datadict Andrei Besleaga
 *
 */

// security - hide paths
if (!defined('ADODB_DIR')) {
	die();
}

class ADODB2_sqlite extends ADODB_DataDict {
	public $databaseType = 'sqlite';
	public $seqField     = false;
	public $addCol       = ' ADD COLUMN';
	public $dropTable    = 'DROP TABLE IF EXISTS %s';
	public $dropIndex    = 'DROP INDEX IF EXISTS %s';
	public $renameTable  = 'ALTER TABLE %s RENAME TO %s';

	public function ActualType($meta) {
		switch (strtoupper($meta)) {
			case 'C':
				//  TEXT , TEXT affinity
				return 'VARCHAR';

			case 'XL':
				//  TEXT , TEXT affinity
				return 'LONGTEXT';

			case 'X':
				//  TEXT , TEXT affinity
				return 'TEXT';

			case 'C2':
				//  TEXT , TEXT affinity
				return 'VARCHAR';

			case 'X2':
				//  TEXT , TEXT affinity
				return 'LONGTEXT';

			case 'B':
				//  TEXT , NONE affinity , BLOB
				return 'LONGBLOB';

			case 'D':
				// NUMERIC , NUMERIC affinity
				return 'DATE';

			case 'T':
				// NUMERIC , NUMERIC affinity
				return 'DATETIME';

			case 'L':
				// NUMERIC , INTEGER affinity
				return 'TINYINT';

			case 'R':
			case 'I4':
			case 'I':
				// NUMERIC , INTEGER affinity
				return 'INTEGER';

			case 'I1':
				// NUMERIC , INTEGER affinity
				return 'TINYINT';

			case 'I2':
				// NUMERIC , INTEGER affinity
				return 'SMALLINT';

			case 'I8':
				// NUMERIC , INTEGER affinity
				return 'BIGINT';

			case 'F':
				// NUMERIC , REAL affinity
				return 'DOUBLE';

			case 'N':
				// NUMERIC , NUMERIC affinity
				return 'NUMERIC';

			default:
				return $meta;
		}
	}

	// return string must begin with space
	public function _CreateSuffix($fname, $ftype, $fnotnull, $fdefault, $fautoinc, $fconstraint, $funsigned) {
		$suffix = '';

		if ($funsigned) {
			$suffix .= ' UNSIGNED';
		}

		if ($fnotnull) {
			$suffix .= ' NOT NULL';
		}

		if (strlen($fdefault)) {
			$suffix .= " DEFAULT $fdefault";
		}

		if ($fautoinc) {
			$suffix .= ' AUTOINCREMENT';
		}

		if ($fconstraint) {
			$suffix .= ' ' . $fconstraint;
		}

		return $suffix;
	}

	public function AlterColumnSQL($tabname, $flds, $tableflds = '', $tableoptions = '') {
		if ($this->debug) {
			ADOConnection::outp('AlterColumnSQL not supported natively by SQLite');
		}

		return array();
	}

	public function DropColumnSQL($tabname, $flds, $tableflds = '', $tableoptions = '') {
		if ($this->debug) {
			ADOConnection::outp('DropColumnSQL not supported natively by SQLite');
		}

		return array();
	}

	public function RenameColumnSQL($tabname, $oldcolumn, $newcolumn, $flds = '') {
		if ($this->debug) {
			ADOConnection::outp('RenameColumnSQL not supported natively by SQLite');
		}

		return array();
	}
}
