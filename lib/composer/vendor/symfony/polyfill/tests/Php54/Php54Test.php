<?php

/*
 * This file is part of the Symfony package.
 *
 * (c) Fabien Potencier <fabien@symfony.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
namespace Symfony\Polyfill\Tests\Php54;
use PHPUnit\Framework\TestCase;
class Php54Test extends TestCase
{
    /**
     * @dataProvider provideClassUsesValid
     */
    public function testClassUsesValid($classOrObject)
    {
        $this->assertSame(array(), class_uses($classOrObject));
    }

    public function provideClassUsesValid()
    {
        return array(
            array('stdClass'),
            array(new \stdClass()),
            array('Iterator'),
        );
    }

    public function testClassUsesInvalid()
    {
        $this->assertFalse(@class_uses('NotDefined'));
    }

    public function testHexToBinValid()
    {
        // With null byte
        $this->assertEquals("\x61\x62\x00\x63\x64", hex2bin('6162006364'));
        $this->assertEquals("\x61\x62\x63\x64", hex2bin('61626364'));
    }

    public function testHexToBinInvalid()
    {
        // Invalid type
        $this->assertNull(@hex2bin(array()));

        // Invalid string length
        $this->assertFalse(@hex2bin('123'));
    }
}

