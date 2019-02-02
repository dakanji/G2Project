<?php
/*
 * Description: A tool for searching the Gallery2 database
 * Date: 02 April 2012
 * License: GNU General Public License (Version 2 or Later at your option)
 * Copyright 2012 Dayo Akanji (dakanji@users.sourceforge.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * General Public License for more details.
 *
 * You can receive a copy of the GNU General Public License by writing to
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA	02110-1301, USA.
 *
 * You can also find a copy of the GNU General Public License on the
 * website of the Free Software Foundation at http://www.gnu.org
 */

// Verification
if (!defined('G2_SUPPORT')) {
	define('G2_SUPPORT_FILE', true);

	include_once __DIR__ . '/lib/support/defaultloc.inc';
}
// Prime variables
if ($_POST['searchstring']) {
	$search_string = $_POST['searchstring'];
}

if ($_POST['advance']) {
	$advance = true;
}

if ($_POST['deep']) {
	$deep = true;
}

// if neither $advance nor $deep is set, this is a reset call so, move on to rendering html
if (isset($advance) || isset($deep)) {
	$output = connect();
	// if connect function returned data, this is an error so, move on to rendering html
	if (is_null($output) == false) {
		$output = process($search_string, $advance, $deep);
	}
}

// Connect to Gallery2
function connect() {
	include_once '../../embed.php';
	$ret = GalleryEmbed::init(
		array(
			'fullInit'   => false,
			'noDatabase' => true,
		)
	);

	if ($ret) {
		return '<div class=\"error center\">Error: Could not connect to Gallery.</div><div class=\"error\">' . $ret->getAsHtml() . '</div>';
	}

	return null;
}

// Carry out the search
function process($search_string, $advance, $deep) {
	global $gallery;

	if ($search_string) {
		// Connect to G2 database or throw error and exit
		$g2_db = $gallery->getConfig('storage.config');
		$link  = mysql_connect($g2_db['hostname'], $g2_db['username'], $g2_db['password']);

		if (!$link) {
			return '<div class=\"error center\">Error: Could not connect to database</div>';
		}
		mysql_select_db($g2_db['database'], $link);

		// Get all tables in G2 database
		$table_sql    = 'SHOW TABLES FROM ' . $g2_db['database'];
		$table_result = mysql_query($table_sql);

		if (!$table_result) {
			return '<div class=\"error center\">Error: ' . mysql_error() . '</div>';
		}
		// Store returned table names in an array
		$g2_tables = array();

		while ($row = mysql_fetch_row($table_result)) {
			array_push($g2_tables, $row[0]);
		}
		// Release memory
		mysql_free_result($table_result);

		// Escape illegal values
		$search_string = mysql_real_escape_string($search_string);
		// Init variable to hold html output
		$html = '';
		// Loop through database tables
		foreach ($g2_tables as $key => $g2_table) {
			// Get all columns in current database table
			$query  = 'SELECT * FROM ' . $g2_table;
			$result = mysql_query($query);

			if ($result) {
				if (mysql_num_rows($result) > 0) {
					$numfields = mysql_num_fields($result);
					// Loop through table columns and search for string in each column
					for ($i = 0; $i < $numfields; $i++) {
						$g2_column = mysql_field_name($result, $i);

						if ($deep) {
							// SQL for "Deep" mode - looks for strings and substrings
							$sql = 'SELECT * FROM ' . $g2_table . ' WHERE ' . $g2_column . " LIKE '%" . $search_string . "%'";
						} else {
							// SQL for standard mode - only looks for whole strings
							$sql = 'SELECT * FROM ' . $g2_table . ' WHERE ' . $g2_column . ' LIKE ' . $search_string;
						}
						$success = mysql_query($sql);
						// If search string is found, append to html output
						if ($success) {
							if (mysql_affected_rows() > 0) {
								// Set plurals if more than one instance found
								if (mysql_affected_rows() > 1) {
									$instance = 'instances';
								} else {
									$instance = 'instance';
								}
								// Append result
								$html .= '<div class="success center">Found ' . mysql_affected_rows() . ' ' . $instance .
								' in Column "' . $g2_column . '" of Table "' . $g2_table . '"</div>';
							}
						}
						// Release memory
						mysql_free_result($success);
					}
				}
				// Release memory
				mysql_free_result($result);
			}
		}
	} elseif ($advance) {
		// html error message for empty search string if not first page load
		$html = '<div class="error center">Error: Empty Search String</div>';
	}
	// html warning message if search string is not found and this is not not the first page load
	if (!$html && $advance) {
		$html = '<div class="warning center">The search string "' . $search_string . '" was not found in the database</div>';
	}
	// return html output
	return $html;
}

?>

<!DOCTYPE HTML>
<html>
<head>
	<title>Gallery Support | MySQL Database Search Tool</title>
	<link rel="stylesheet" type="text/css" href="<?php echo $baseUrl; ?>support.css">
</head>
<body>
	<div id="content">
		<div id="title">
			<a href="../../">Gallery</a> &raquo;
			<a href="index.php">Support</a> &raquo; MySQL Database Search Tool
		</div>
		<div class="center">
			<h2>
				A tool for searching the Gallery2 database<br>
				Valid only for MySQL based installations
			</h2>
			<form action="search_db.php" method="POST">
				<input type="hidden" name="advance" value=true /><br>
				<?php
				if ($search_string) {
					?>
					<input type="search" name="searchstring" value=<?php echo $search_string; ?>><br>
					<?php
				} else {
					?>
					<input required type="search" name="searchstring" placeholder="Search"><br>
					<?php
				}
				?>
				<?php
				if ($deep) {
					?>
					Match substrings: <input type="checkbox" name="deep" value=true checked="yes"/><br>
					<?php
				} else {
					?>
					Match substrings: <input type="checkbox" name="deep" value=true/><br>
					<?php
				}
				?>
				<input type="submit" value="Search Database">
			</form>
			<form action="search_db.php" method="POST">
				<input type="hidden" name="searchstring">
				<input type="submit" value="Reset">
			</form>
		</div>

		<?php
		if ($output) {
			?>
			<hr class="faint">
			<?php echo $output; ?>
			<?php
		}
		?>
	</div>
</body>
</html>
