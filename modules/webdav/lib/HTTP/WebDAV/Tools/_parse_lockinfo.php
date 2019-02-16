<?php
//
// +----------------------------------------------------------------------+
// | PHP Version 4                                                        |
// +----------------------------------------------------------------------+
// | Copyright (c) 1997-2003 The PHP Group                                |
// +----------------------------------------------------------------------+
// | This source file is subject to version 2.02 of the PHP license,      |
// | that is bundled with this package in the file LICENSE, and is        |
// | available at through the world-wide-web at                           |
// | http://www.php.net/license/2_02.txt.                                 |
// | If you did not receive a copy of the PHP license and are unable to   |
// | obtain it through the world-wide-web, please send a note to          |
// | license@php.net so we can mail you a copy immediately.               |
// +----------------------------------------------------------------------+
// | Authors: Hartmut Holzgraefe <hholzgra@php.net>                       |
// |          Christian Stocker <chregu@bitflux.ch>                       |
// +----------------------------------------------------------------------+
//
// $Id: _parse_lockinfo.php 15342 2006-12-01 21:14:46Z andy_st $

/**
 * Helper class for parsing LOCK request bodies
 *
 * @package HTTP_WebDAV_Server
 * @author Hartmut Holzgraefe <hholzgra@php.net>
 * @version 0.99.1dev
 */
class _parse_lockinfo
{
    /**
     * Success state flag
     *
     * @var boolean
     * @access public
     */
    var $success = false;

    /**
     * Lock type, currently only write
     *
     * @var string
     * @access public
     */
    var $locktype = '';

    /**
     * Lock scope, shared or exclusive
     *
     * @var string
     * @access public
     */
    var $lockscope = '';

    /**
     * Lock owner information
     *
     * @var string
     * @access public
     */
    var $owner = '';

    /**
     * Flag that is set during lock owner read
     *
     * @var boolean
     * @access private
     */
    var $collect_owner = false;

    /**
     * Constructor
     *
     * @param resource input stream file descriptor
     * @access public
     */
    function _parse_lockinfo($handle)
    {
        // open input stream
        if (!$handle) {
            $this->success = false;
            return;
        }

        // success state flag
        $this->success = true;

        // remember if any input was parsed
        $had_input = false;

        // create namespace aware XML parser
        $parser = xml_parser_create_ns('UTF-8', ' ');

        // set tag & data handlers
        xml_set_element_handler($parser, array(&$this, '_startElement'),
            array(&$this, "_endElement"));

        xml_set_character_data_handler($parser, array(&$this, '_data'));

        // we want a case sensitive parser
        xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, false);

        // parse input
        while ($this->success && !feof($handle)) {
            $line = fgets($handle);
            if (is_string($line)) {
                $had_input = true;
                $this->success &= xml_parse($parser, $line, false);
            }
        }

        // finish parsing
        if ($had_input) {
            $this->success &= xml_parse($parser, '', true);
        }

        // check if required tags where found
        $this->success &= !empty($this->locktype);
        $this->success &= !empty($this->lockscope);

        // free parser resource
        xml_parser_free($parser);

        // close input stream
        fclose($handle);
    }

    /**
     * Start tag handler
     *
     * @access private
     * @param resource parser
     * @param string tag name
     * @param array tag attributes
     * @return void
     */
    function _startElement($parser, $name, $attrs)
    {
        // namespace handling
        if (strstr($name, ' ')) {
            list ($ns, $name) = explode(' ', $name);
            if (empty($ns)) {
                $this->success = false;
            }
        }

        // everything within the <owner> tag needs to be collected
        if ($this->collect_owner) {
            $ns_short = '';
            $ns_attr = '';
            if ($ns) {
                if ($ns == 'DAV:') {
                    $ns_short = 'D:';
                } else {
                    $ns_attr = ' xmlns="' . $ns . '"';
                }
            }
            $this->owner .= "<$ns_short$name$ns_attr>";
        } else if ($ns == 'DAV:') {
            // parse only the essential tags
            switch ($name) {
            case 'write':
                $this->locktype = $name;
                break;
            case 'exclusive':
            case 'shared':
                $this->lockscope = $name;
                break;
            case 'owner':
                $this->collect_owner = true;
                break;
            }
        }
    }

    /**
     * End tag handler
     *
     * @access private
     * @param resource parser
     * @param string tag name
     * @return void
     */
    function _endElement($parser, $name)
    {
        // namespace handling
        if (strstr($name, ' ')) {
            list ($ns, $name) = explode(' ', $name);
	} else {
	    $ns = '';
	}

        // <owner> finished?
        if ($ns == 'DAV:' && $name == 'owner') {
            $this->collect_owner = false;
        }

        // within <owner> we have to collect everything
        if ($this->collect_owner) {
            $ns_short = '';
            $ns_attr = '';
            if ($ns) {
                if ($ns == 'DAV:') {
                    $ns_short = 'D:';
                } else {
                    $ns_attr = ' xmlns="' . $ns . '"';
                }
            }
            $this->owner .= "</$ns_short$name$ns_attr>";
        }
    }

    /**
     * Character data handler
     *
     * @access private
     * @param resource parser
     * @param string data
     * @return void
     */
    function _data($parser, $data)
    {
        // only the <owner> tag has data content
        if ($this->collect_owner) {
            $this->owner .= $data;
        }
    }
}
?>
