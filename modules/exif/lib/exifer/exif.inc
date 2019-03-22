<?php

//================================================================================================
//================================================================================================
//================================================================================================

/*
	Exifer 1.5
	Extracts EXIF information from digital photos.
	Copyright � 2005 Jake Olefsky
	http://www.offsky.com/software/exif/index.php
	jake@olefsky.com
	This program is free software; you can redistribute it and/or modify it under the terms of
	the GNU General Public License as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details. http://www.gnu.org/copyleft/gpl.html
	SUMMARY:
				This script will correctly parse all of the EXIF data included in images taken
				with digital cameras.  It will read the IDF0, IDF1, SubIDF and InteroperabilityIFD
				fields as well as parsing some of the MakerNote fields that vary depending on
				camera make and model.  This script parses more tags than the internal PHP exif
				implementation and it will correctly identify and decode what all the values mean.
				This version will correctly parse the MakerNote field for Nikon, Olympus, and Canon
				digital cameras.  Others will follow.
	TESTED WITH:
				Nikon CoolPix 700
				Nikon CoolPix 4500
				Nikon CoolPix 950
				Nikon Coolpix 5700
				Canon PowerShot S200
				Canon PowerShot S110
				Olympus C2040Z
				Olympus C960
				Canon Ixus
				Canon EOS 300D
				Canon Digital Rebel
				Canon EOS 10D
				Canon PowerShot G2
				FujiFilm DX 10
				FujiFilm MX-1200
				FujiFilm FinePix2400
				FujiFilm FinePix2600
				FujiFilm FinePix S602
				FujiFilm FinePix40i
				Sony D700
				Sony Cybershot
				Kodak DC210
				Kodak DC240
				Kodak DC4800
				Kodak DX3215
				Ricoh RDC-5300
				Sanyo VPC-G250
				Sanyo VPC-SX550
				Epson 3100z
	If you improve this script or test it with a camera not on my list, please email me a copy of
	the improvement so everyone can benefit from it.  Thanks!
	VERSION HISTORY:
	1.0 	Sept 23, 2002
			First Public Release
	1.1		Jan 25, 2003
			+Gracefully handled the error case where you pass an empty string to this library
			+Fixed an inconsistency in the Olympus Camera parsing module
			+Added support for parsing the MakerNote of Canon images.
			+Modified how the imagefile is opened so it works for windows machines.
			+Correctly parses the FocalPlaneResolutionUnit and PhotometricInterpretation fields
			+Negative rational numbers are properly displayed
			+Strange old cameras that use Motorola endineness are now properly supported
			+Tested with several more cameras
			Potential Problem: Negative Shorts and Negative Longs may not be correctly displayed, but I
				have not yet found an example of negative shorts or longs being used.
	1.2		Mar 30, 2003
			+Fixed an error that was displayed if you edited your image with WinXP's image viewer
			+Fixed a bug that caused some images saved from 3rd party software to not parse correctly
			+Changed the ExposureTime tag to display in fractional seconds rather than decimal
			+Updated the ShutterSpeedValue tag to have the units of 'sec'
			+Added support for parsing the MakeNote of FujiFilm images
			+Added support for parsing the MakeNote of Sanyo images
			+Fixed a bug with parsing some Olympus MakerNote tags
			+Tested with several more cameras
	1.3		Jun 15, 2003
			+Fixed Canon MakerNote support for some models
				(Canon has very difficult and inconsistent MakerNote syntax)
			+Negative signed shorts and negative signed longs are properly displayed
			+Several more tags are defined
			+More information in my comments about what each tag is
			+Parses and Displays GPS information if available
			+Tested with several more cameras
	1.4		Sept 14, 2003
			+This software is now licensed under the GNU General Public License
			+Exposure time is now correctly displayed when the numerator is 10
			+Fixed the calculation and display of ShutterSpeedValue, ApertureValue and MaxApertureValue
			+Fixed a bug with the GPS code
			+Tested with several more cameras
	1.5		Feb 18, 2005
			+It now gracefully deals with a passed in file that cannot be found.
			+Fixed a GPS bug for the parsing of Altitude and other signed rational numbers
			+Defined more values for Canon cameras.
			+Added "bulb" detection for ShutterSpeed
			+Made script loading a little faster and less memory intensive.
			+Bug fixes
			+Better error reporting
			+Graceful failure for files with corrupt exif info.
			+QuickTime (including iPhoto) messes up the Makernote tag for certain photos (no workaround yet)
			+Now reads exif information when the jpeg markers are out of order
			+Gives raw data output for IPTC, COM and APP2 fields which are sometimes set by other applications
			+Improvements to Nikon Makernote parsing
*/

//================================================================================================

//================================================================================================
//================================================================================================
//================================================================================================
//================================================================================================
// Converts from Intel to Motorola endien.  Just reverses the bytes (assumes hex is passed in)
//================================================================================================
//================================================================================================
function intel2Moto($intel) {
	$len  = strlen($intel);
	$moto = '';

	for ($i = 0; $i <= $len; $i += 2) {
		$moto .= substr($intel, $len - $i, 2);
	}

	return $moto;
}

//================================================================================================
//================================================================================================
// Looks up the name of the tag
//================================================================================================
//================================================================================================
function lookup_tag($tag) {
	switch ($tag) {
			//used by IFD0 "Camera Tags"
		case '000b':
			$tag = 'ACDComment';

			//text string up to 999 bytes long
			break;

		case '00fe':
			$tag = 'ImageType';

			//integer -2147483648 to 2147483647
			break;

		case '0106':
			$tag = 'PhotometricInterpret';

			//?? Please send sample image with this tag
			break;

		case '010e':
			$tag = 'ImageDescription';

			//text string up to 999 bytes long
			break;

		case '010f':
			$tag = 'Make';

			//text string up to 999 bytes long
			break;

		case '0110':
			$tag = 'Model';

			//text string up to 999 bytes long
			break;

		case '0112':
			$tag = 'Orientation';

			//integer values 1-9
			break;

		case '0115':
			$tag = 'SamplePerPixel';

			//integer 0-65535
			break;

		case '011a':
			$tag = 'xResolution';

			//positive rational number
			break;

		case '011b':
			$tag = 'yResolution';

			//positive rational number
			break;

		case '011c':
			$tag = 'PlanarConfig';

			//integer values 1-2
			break;

		case '0128':
			$tag = 'ResolutionUnit';

			//integer values 1-3
			break;

		case '0131':
			$tag = 'Software';

			//text string up to 999 bytes long
			break;

		case '0132':
			$tag = 'DateTime';

			//YYYY:MM:DD HH:MM:SS
			break;

		case '013b':
			$tag = 'Artist';

			//text string up to 999 bytes long
			break;

		case '013c':
			$tag = 'HostComputer';

			//text string
			break;

		case '013e':
			$tag = 'WhitePoint';

			//two positive rational numbers
			break;

		case '013f':
			$tag = 'PrimaryChromaticities';

			//six positive rational numbers
			break;

		case '0211':
			$tag = 'YCbCrCoefficients';

			//three positive rational numbers
			break;

		case '0213':
			$tag = 'YCbCrPositioning';

			//integer values 1-2
			break;

		case '0214':
			$tag = 'ReferenceBlackWhite';

			//six positive rational numbers
			break;

		case '8298':
			$tag = 'Copyright';

			//text string up to 999 bytes long
			break;

		case '8649':
			$tag = 'PhotoshopSettings';

			//??
			break;

		case '8825':
			$tag = 'GPSInfoOffset';

			break;

		case '8769':
			$tag = 'ExifOffset';

			//positive integer
			break;

			//used by Exif SubIFD "Image Tags"
		case '829a':
			$tag = 'ExposureTime';

			//seconds or fraction of seconds 1/x
			break;

		case '829d':
			$tag = 'FNumber';

			//positive rational number
			break;

		case '8822':
			$tag = 'ExposureProgram';

			//integer value 1-9
			break;

		case '8824':
			$tag = 'SpectralSensitivity';

			//??
			break;

		case '8827':
			$tag = 'ISOSpeedRatings';

			//integer 0-65535
			break;

		case '9000':
			$tag = 'ExifVersion';

			//??
			break;

		case '9003':
			$tag = 'DateTimeOriginal';

			//YYYY:MM:DD HH:MM:SS
			break;

		case '9004':
			$tag = 'DateTimedigitized';

			//YYYY:MM:DD HH:MM:SS
			break;

		case '9101':
			$tag = 'ComponentsConfiguration';

			//??
			break;

		case '9102':
			$tag = 'CompressedBitsPerPixel';

			//positive rational number
			break;

		case '9201':
			$tag = 'ShutterSpeedValue';

			//seconds or fraction of seconds 1/x
			break;

		case '9202':
			$tag = 'ApertureValue';

			//positive rational number
			break;

		case '9203':
			$tag = 'BrightnessValue';

			//positive rational number
			break;

		case '9204':
			$tag = 'ExposureBiasValue';

			//positive rational number (EV)
			break;

		case '9205':
			$tag = 'MaxApertureValue';

			//positive rational number
			break;

		case '9206':
			$tag = 'SubjectDistance';

			//positive rational number (meters)
			break;

		case '9207':
			$tag = 'MeteringMode';

			//integer 1-6 and 255
			break;

		case '9208':
			$tag = 'LightSource';

			//integer 1-255
			break;

		case '9209':
			$tag = 'Flash';

			//integer 1-255
			break;

		case '920a':
			$tag = 'FocalLength';

			//positive rational number (mm)
			break;

		case '9213':
			$tag = 'ImageHistory';

			//text string up to 999 bytes long
			break;

		case '927c':
			$tag = 'MakerNote';

			//a bunch of data
			break;

		case '9286':
			$tag = 'UserComment';

			//text string
			break;

		case '9290':
			$tag = 'SubsecTime';

			//text string up to 999 bytes long
			break;

		case '9291':
			$tag = 'SubsecTimeOriginal';

			//text string up to 999 bytes long
			break;

		case '9292':
			$tag = 'SubsecTimeDigitized';

			//text string up to 999 bytes long
			break;

		case 'a000':
			$tag = 'FlashPixVersion';

			//??
			break;

		case 'a001':
			$tag = 'ColorSpace';

			//values 1 or 65535
			break;

		case 'a002':
			$tag = 'ExifImageWidth';

			//ingeter 1-65535
			break;

		case 'a003':
			$tag = 'ExifImageHeight';

			//ingeter 1-65535
			break;

		case 'a004':
			$tag = 'RelatedSoundFile';

			//text string 12 bytes long
			break;

		case 'a005':
			$tag = 'ExifInteroperabilityOffset';

			//positive integer
			break;

		case 'a20c':
			$tag = 'SpacialFreqResponse';

			//??
			break;

		case 'a20b':
			$tag = 'FlashEnergy';

			//positive rational number
			break;

		case 'a20e':
			$tag = 'FocalPlaneXResolution';

			//positive rational number
			break;

		case 'a20f':
			$tag = 'FocalPlaneYResolution';

			//positive rational number
			break;

		case 'a210':
			$tag = 'FocalPlaneResolutionUnit';

			//values 1-3
			break;

		case 'a214':
			$tag = 'SubjectLocation';

			//two integers 0-65535
			break;

		case 'a215':
			$tag = 'ExposureIndex';

			//positive rational number
			break;

		case 'a217':
			$tag = 'SensingMethod';

			//values 1-8
			break;

		case 'a300':
			$tag = 'FileSource';

			//integer
			break;

		case 'a301':
			$tag = 'SceneType';

			//integer
			break;

		case 'a302':
			$tag = 'CFAPattern';

			//undefined data type
			break;

		case 'a401':
			$tag = 'CustomerRender';

			//values 0 or 1
			break;

		case 'a402':
			$tag = 'ExposureMode';

			//values 0-2
			break;

		case 'a403':
			$tag = 'WhiteBalance';

			//values 0 or 1
			break;

		case 'a404':
			$tag = 'DigitalZoomRatio';

			//positive rational number
			break;

		case 'a405':
			$tag = 'FocalLengthIn35mmFilm';

			//??
			break;

		case 'a406':
			$tag = 'SceneCaptureMode';

			//values 0-3
			break;

		case 'a407':
			$tag = 'GainControl';

			//values 0-4
			break;

		case 'a408':
			$tag = 'Contrast';

			//values 0-2
			break;

		case 'a409':
			$tag = 'Saturation';

			//values 0-2
			break;

		case 'a40a':
			$tag = 'Sharpness';

			//values 0-2
			break;

			//used by Interoperability IFD
		case '0001':
			$tag = 'InteroperabilityIndex';

			//text string 3 bytes long
			break;

		case '0002':
			$tag = 'InteroperabilityVersion';

			//datatype undefined
			break;

		case '1000':
			$tag = 'RelatedImageFileFormat';

			//text string up to 999 bytes long
			break;

		case '1001':
			$tag = 'RelatedImageWidth';

			//integer in range 0-65535
			break;

		case '1002':
			$tag = 'RelatedImageLength';

			//integer in range 0-65535
			break;

			//used by IFD1 "Thumbnail"
		case '0100':
			$tag = 'ImageWidth';

			//integer in range 0-65535
			break;

		case '0101':
			$tag = 'ImageLength';

			//integer in range 0-65535
			break;

		case '0102':
			$tag = 'BitsPerSample';

			//integers in range 0-65535
			break;

		case '0103':
			$tag = 'Compression';

			//values 1 or 6
			break;

		case '0106':
			$tag = 'PhotometricInterpretation';

			//values 0-4
			break;

		case '010e':
			$tag = 'ThumbnailDescription';

			//text string up to 999 bytes long
			break;

		case '010f':
			$tag = 'ThumbnailMake';

			//text string up to 999 bytes long
			break;

		case '0110':
			$tag = 'ThumbnailModel';

			//text string up to 999 bytes long
			break;

		case '0111':
			$tag = 'StripOffsets';

			//??
			break;

		case '0112':
			$tag = 'ThumbnailOrientation';

			//integer 1-9
			break;

		case '0115':
			$tag = 'SamplesPerPixel';

			//??
			break;

		case '0116':
			$tag = 'RowsPerStrip';

			//??
			break;

		case '0117':
			$tag = 'StripByteCounts';

			//??
			break;

		case '011a':
			$tag = 'ThumbnailXResolution';

			//positive rational number
			break;

		case '011b':
			$tag = 'ThumbnailYResolution';

			//positive rational number
			break;

		case '011c':
			$tag = 'PlanarConfiguration';

			//values 1 or 2
			break;

		case '0128':
			$tag = 'ThumbnailResolutionUnit';

			//values 1-3
			break;

		case '0201':
			$tag = 'JpegIFOffset';

			break;

		case '0202':
			$tag = 'JpegIFByteCount';

			break;

		case '0212':
			$tag = 'YCbCrSubSampling';

			break;

			//misc
		case '00ff':
			$tag = 'SubfileType';

			break;

		case '012d':
			$tag = 'TransferFunction';

			break;

		case '013d':
			$tag = 'Predictor';

			break;

		case '0142':
			$tag = 'TileWidth';

			break;

		case '0143':
			$tag = 'TileLength';

			break;

		case '0144':
			$tag = 'TileOffsets';

			break;

		case '0145':
			$tag = 'TileByteCounts';

			break;

		case '014a':
			$tag = 'SubIFDs';

			break;

		case '015b':
			$tag = 'JPEGTables';

			break;

		case '828d':
			$tag = 'CFARepeatPatternDim';

			break;

		case '828e':
			$tag = 'CFAPattern';

			break;

		case '828f':
			$tag = 'BatteryLevel';

			break;

		case '83bb':
			$tag = 'IPTC/NAA';

			break;

		case '8773':
			$tag = 'InterColorProfile';

			break;

		case '8828':
			$tag = 'OECF';

			break;

		case '8829':
			$tag = 'Interlace';

			break;

		case '882a':
			$tag = 'TimeZoneOffset';

			break;

		case '882b':
			$tag = 'SelfTimerMode';

			break;

		case '920b':
			$tag = 'FlashEnergy';

			break;

		case '920c':
			$tag = 'SpatialFrequencyResponse';

			break;

		case '920d':
			$tag = 'Noise';

			break;

		case '9211':
			$tag = 'ImageNumber';

			break;

		case '9212':
			$tag = 'SecurityClassification';

			break;

		case '9214':
			$tag = 'SubjectLocation';

			break;

		case '9215':
			$tag = 'ExposureIndex';

			break;

		case '9216':
			$tag = 'TIFF/EPStandardID';

			break;

		case 'a20b':
			$tag = 'FlashEnergy';

			break;

		default:
			$tag = 'unknown:' . $tag;

			break;
	}

	return $tag;
}

//================================================================================================
//================================================================================================
// Looks up the datatype
//================================================================================================
//================================================================================================
function lookup_type(&$type, &$size) {
	switch ($type) {
		case '0001':
			$type = 'UBYTE';
			$size = 1;

			break;

		case '0002':
			$type = 'ASCII';
			$size = 1;

			break;

		case '0003':
			$type = 'USHORT';
			$size = 2;

			break;

		case '0004':
			$type = 'ULONG';
			$size = 4;

			break;

		case '0005':
			$type = 'URATIONAL';
			$size = 8;

			break;

		case '0006':
			$type = 'SBYTE';
			$size = 1;

			break;

		case '0007':
			$type = 'UNDEFINED';
			$size = 1;

			break;

		case '0008':
			$type = 'SSHORT';
			$size = 2;

			break;

		case '0009':
			$type = 'SLONG';
			$size = 4;

			break;

		case '000a':
			$type = 'SRATIONAL';
			$size = 8;

			break;

		case '000b':
			$type = 'FLOAT';
			$size = 4;

			break;

		case '000c':
			$type = 'DOUBLE';
			$size = 8;

			break;

		default:
			$type = 'error:' . $type;
			$size = 0;

			break;
	}

	return $type;
}

//================================================================================================
//================================================================================================
// Formats Data for the data type
//================================================================================================
//================================================================================================
function formatData($type, $tag, $intel, $data) {
	if ($type == 'ASCII') {
		//do nothing
	} elseif ($type == 'URATIONAL' || $type == 'SRATIONAL') {
		$data = bin2hex($data);

		if ($intel == 1) {
			$data = intel2Moto($data);
		}

		if ($intel == 1) {
			$top = hexdec(substr($data, 8, 8));
		} //intel stores them bottom-top

		else {
			$top = hexdec(substr($data, 0, 8));
		} //motorola stores them top-bottom

		if ($intel == 1) {
			$bottom = hexdec(substr($data, 0, 8));
		} //intel stores them bottom-top

		else {
			$bottom = hexdec(substr($data, 8, 8));
		} //motorola stores them top-bottom

		if ($type == 'SRATIONAL' && $top > 2147483647) {
			$top = $top - 4294967296;
		} //this makes the number signed instead of unsigned

		if ($bottom != 0) {
			$data = $top / $bottom;
		} elseif ($top == 0) {
			$data = 0;
		} else {
			$data = $top . '/' . $bottom;
		}

		if (($tag == '011a' || $tag == '011b') && $bottom == 1) {
			//XResolution YResolution
			$data = $top . ' dots per ResolutionUnit';
		} elseif ($tag == '829d') {
			//FNumber
			$data = 'f ' . $data;
		} elseif ($tag == '9204') {
			//ExposureBiasValue
			$data = $data . ' EV';
		} elseif ($tag == '9205' || $tag == '9202') {
			//ApertureValue and MaxApertureValue
			//ApertureValue is given in the APEX Mode. Many thanks to Matthieu Froment for this code
			//The formula is : Aperture = 2*log2(FNumber) <=> FNumber = e((Aperture.ln(2))/2)
			$data = exp(($data * log(2)) / 2);

			//Focal is given with a precision of 1 digit.
			$data = round($data, 1);
			$data = 'f ' . $data;
		} elseif ($tag == '920a') {
			//FocalLength
			$data = $data . ' mm';
		} elseif ($tag == '829a' || $tag == '9201') {
			//ExposureTime or ShutterSpeedValue
			// The ShutterSpeedValue is given in the APEX mode. Many thanks to Matthieu Froment for this code
			// The formula is : Shutter = - log2(exposureTime) (Appendix C of EXIF spec.)
			// Where shutter is in APEX, log2(exposure) = ln(exposure)/ln(2)
			// So final formula is : exposure = exp(-ln(2).shutter)
			// The formula can be developed : exposure = 1/(exp(ln(2).shutter))
			if ($tag == '9201') {
				$data = exp($data * log(2));

				if ($data >= 1000000) {
					// Throw out near-zero value
					$data = null;
				} elseif ($data >= 1.99) {
					$prec = $data >= 38 ? -1 : 0;
					$data = '1/' . round($data, $prec) . ' sec';
				} elseif ($data > 0) {
					$data = 1 / $data;
				}
			} elseif ($tag == '829a' && $top > 0 && $bottom % $top == 0) {
				$data = '1/' . (int)($bottom / $top) . ' sec';
			}

			if (is_float($data)) {
				if ($data > 0) {
					$prec = $data < 1 ? 2 : ($data < 10 ? 1 : 0);
					$data = round($data, $prec) . ' sec';
				} else {
					$data = 'Bulb';
				}
			}
		}
	} elseif ($type == 'USHORT' || $type == 'SSHORT' || $type == 'ULONG' || $type == 'SLONG' || $type == 'FLOAT' || $type == 'DOUBLE') {
		$data = bin2hex($data);

		if ($type == 'USHORT' || $type == 'SSHORT') {
			$data = substr($data, 0, 4);
		}

		if ($intel == 1) {
			$data = intel2Moto($data);
		}

		$data = hexdec($data);

		if ($type == 'SSHORT' && $data > 32767) {
			$data = $data - 65536;
		} //this makes the number signed instead of unsigned

		if ($type == 'SLONG' && $data > 2147483647) {
			$data = $data - 4294967296;
		} //this makes the number signed instead of unsigned

		if ($tag == '0112') {
			//Orientation
			if ($data == 1) {
				$data = 'Normal (O deg)';
			} elseif ($data == 2) {
				$data = 'Mirrored';
			} elseif ($data == 3) {
				$data = 'Upsidedown';
			} elseif ($data == 4) {
				$data = 'Upsidedown Mirrored';
			} elseif ($data == 5) {
				$data = '90 deg CW Mirrored';
			} elseif ($data == 6) {
				$data = '90 deg CCW';
			} elseif ($data == 7) {
				$data = '90 deg CCW Mirrored';
			} elseif ($data == 8) {
				$data = '90 deg CW';
			}
		} elseif ($tag == '0128' || $tag == 'a210' || $tag == '0128') {
			//ResolutionUnit and FocalPlaneResolutionUnit and ThumbnailResolutionUnit
			if ($data == 1) {
				$data = 'No Unit';
			} elseif ($data == 2) {
				$data = 'Inch';
			} elseif ($data == 3) {
				$data = 'Centimeter';
			}
		} elseif ($tag == '0213') {
			//YCbCrPositioning
			if ($data == 1) {
				$data = 'Center of Pixel Array';
			} elseif ($data == 2) {
				$data = 'Datum Point';
			}
		} elseif ($tag == '8822') {
			//ExposureProgram
			if ($data == 1) {
				$data = 'Manual';
			} elseif ($data == 2) {
				$data = 'Program';
			} elseif ($data == 3) {
				$data = 'Aperture Priority';
			} elseif ($data == 4) {
				$data = 'Shutter Priority';
			} elseif ($data == 5) {
				$data = 'Program Creative';
			} elseif ($data == 6) {
				$data = 'Program Action';
			} elseif ($data == 7) {
				$data = 'Portrait';
			} elseif ($data == 8) {
				$data = 'Landscape';
			} else {
				$data = 'Unknown: ' . $data;
			}
		} elseif ($tag == '9207') {
			//MeteringMode
			if ($data == 0) {
				$data = 'Unknown';
			} elseif ($data == 1) {
				$data = 'Average';
			} elseif ($data == 2) {
				$data = 'Center Weighted Average';
			} elseif ($data == 3) {
				$data = 'Spot';
			} elseif ($data == 4) {
				$data = 'Multi-Spot';
			} elseif ($data == 5) {
				$data = 'Multi-Segment';
			} elseif ($data == 6) {
				$data = 'Partial';
			} elseif ($data == 255) {
				$data = 'Other';
			} else {
				$data = 'Unknown: ' . $data;
			}
		} elseif ($tag == '9208') {
			//LightSource
			if ($data == 0) {
				$data = 'Unknown or Auto';
			} elseif ($data == 1) {
				$data = 'Daylight';
			} elseif ($data == 2) {
				$data = 'Fluorescent';
			} elseif ($data == 3) {
				$data = 'Tungsten (Incandescent light)';
			} elseif ($data == 4) {
				$data = 'Flash';
			} elseif ($data == 9) {
				$data = 'Fine Weather';
			} elseif ($data == 10) {
				$data = 'Cloudy Weather';
			} elseif ($data == 11) {
				$data = 'Shade';
			} elseif ($data == 12) {
				$data = 'Daylight Fluorescent (D 5700 - 7100K)';
			} elseif ($data == 13) {
				$data = 'Day White Fluorescent (N 4600 - 5400K)';
			} elseif ($data == 14) {
				$data = 'Cool White Fluorescent (W 3900 -4500K)';
			} elseif ($data == 15) {
				$data = 'White Fluorescent (WW 3200 - 3700K)';
			} elseif ($data == 10) {
				$data = 'Flash';
			} elseif ($data == 17) {
				$data = 'Standard Light A';
			} elseif ($data == 18) {
				$data = 'Standard Light B';
			} elseif ($data == 19) {
				$data = 'Standard Light C';
			} elseif ($data == 20) {
				$data = 'D55';
			} elseif ($data == 21) {
				$data = 'D65';
			} elseif ($data == 22) {
				$data = 'D75';
			} elseif ($data == 23) {
				$data = 'D50';
			} elseif ($data == 24) {
				$data = 'ISO Studio Tungsten';
			} elseif ($data == 255) {
				$data = 'Other';
			} else {
				$data = 'Unknown: ' . $data;
			}
		} elseif ($tag == '9209') {
			//Flash
			if ($data == 0) {
				$data = 'No Flash';
			} elseif ($data == 1) {
				$data = 'Flash';
			} elseif ($data == 5) {
				$data = 'Flash, strobe return light not detected';
			} elseif ($data == 7) {
				$data = 'Flash, strobe return light detected';
			} elseif ($data == 9) {
				$data = 'Compulsory Flash';
			} elseif ($data == 13) {
				$data = 'Compulsory Flash, Return light not detected';
			} elseif ($data == 15) {
				$data = 'Compulsory Flash, Return light detected';
			} elseif ($data == 16) {
				$data = 'No Flash';
			} elseif ($data == 24) {
				$data = 'No Flash';
			} elseif ($data == 25) {
				$data = 'Flash, Auto-Mode';
			} elseif ($data == 29) {
				$data = 'Flash, Auto-Mode, Return light not detected';
			} elseif ($data == 31) {
				$data = 'Flash, Auto-Mode, Return light detected';
			} elseif ($data == 32) {
				$data = 'No Flash';
			} elseif ($data == 65) {
				$data = 'Red Eye';
			} elseif ($data == 69) {
				$data = 'Red Eye, Return light not detected';
			} elseif ($data == 71) {
				$data = 'Red Eye, Return light detected';
			} elseif ($data == 73) {
				$data = 'Red Eye, Compulsory Flash';
			} elseif ($data == 77) {
				$data = 'Red Eye, Compulsory Flash, Return light not detected';
			} elseif ($data == 79) {
				$data = 'Red Eye, Compulsory Flash, Return light detected';
			} elseif ($data == 89) {
				$data = 'Red Eye, Auto-Mode';
			} elseif ($data == 93) {
				$data = 'Red Eye, Auto-Mode, Return light not detected';
			} elseif ($data == 95) {
				$data = 'Red Eye, Auto-Mode, Return light detected';
			} else {
				$data = 'Unknown: ' . $data;
			}
		} elseif ($tag == 'a001') {
			//ColorSpace
			if ($data == 1) {
				$data = 'sRGB';
			} else {
				$data = 'Uncalibrated';
			}
		} elseif ($tag == 'a002' || $tag == 'a003') {
			//ExifImageWidth/Height
			$data = $data . ' pixels';
		} elseif ($tag == '0103') {
			//Compression
			if ($data == 1) {
				$data = 'No Compression';
			} elseif ($data == 6) {
				$data = 'Jpeg Compression';
			} else {
				$data = 'Unknown: ' . $data;
			}
		} elseif ($tag == 'a217') {
			//SensingMethod
			if ($data == 1) {
				$data = 'Not defined';
			} elseif ($data == 2) {
				$data = 'One Chip Color Area Sensor';
			} elseif ($data == 3) {
				$data = 'Two Chip Color Area Sensor';
			} elseif ($data == 4) {
				$data = 'Three Chip Color Area Sensor';
			} elseif ($data == 5) {
				$data = 'Color Sequential Area Sensor';
			} elseif ($data == 7) {
				$data = 'Trilinear Sensor';
			} elseif ($data == 8) {
				$data = 'Color Sequential Linear Sensor';
			} else {
				$data = 'Unknown: ' . $data;
			}
		} elseif ($tag == '0106') {
			//PhotometricInterpretation
			if ($data == 1) {
				$data = 'Monochrome';
			} elseif ($data == 2) {
				$data = 'RGB';
			} elseif ($data == 6) {
				$data = 'YCbCr';
			} else {
				$data = 'Unknown: ' . $data;
			}
		}
	} elseif ($type == 'UNDEFINED') {
		if ($tag == '9000' || $tag == 'a000' || $tag == '0002') {
			//ExifVersion,FlashPixVersion,InteroperabilityVersion
			$data = 'version ' . $data / 100;
		}

		if ($tag == 'a300') {
			//FileSource
			$data = bin2hex($data);
			$data = str_replace('00', '', $data);
			$data = str_replace('03', 'Digital Still Camera', $data);
		}

		if ($tag == 'a301') {
			//SceneType
			$data = bin2hex($data);
			$data = str_replace('00', '', $data);
			$data = str_replace('01', 'Directly Photographed', $data);
		}

		if ($tag == '9101') {
			//ComponentsConfiguration
			$data = bin2hex($data);
			$data = str_replace('01', 'Y', $data);
			$data = str_replace('02', 'Cb', $data);
			$data = str_replace('03', 'Cr', $data);
			$data = str_replace('04', 'R', $data);
			$data = str_replace('05', 'G', $data);
			$data = str_replace('06', 'B', $data);
			$data = str_replace('00', '', $data);
		}

		if ($tag == '9286') {
			//UserComment
			$encoding = rtrim(substr($data, 0, 8));
			$data     = rtrim(substr($data, 8));
		}
	} else {
		$data = bin2hex($data);

		if ($intel == 1) {
			$data = intel2Moto($data);
		}
	}

	return $data;
}

//================================================================================================
//================================================================================================
// Reads one standard IFD entry
//================================================================================================
//================================================================================================
function read_entry(&$result, $in, $seek, $intel, $ifd_name, $globalOffset) {
	if (feof($in)) {
		//test to make sure we can still read.
		$result['Errors'] = $result['Errors'] + 1;

		return;
	}

	//2 byte tag
	$tag = bin2hex(fread($in, 2));

	if ($intel == 1) {
		$tag = intel2Moto($tag);
	}

	$tag_name = lookup_tag($tag);

	//2 byte datatype
	$type = bin2hex(fread($in, 2));

	if ($intel == 1) {
		$type = intel2Moto($type);
	}

	lookup_type($type, $size);

	//4 byte number of elements
	$count = bin2hex(fread($in, 4));

	if ($intel == 1) {
		$count = intel2Moto($count);
	}

	$bytesofdata = $size * hexdec($count);

	//4 byte value or pointer to value if larger than 4 bytes
	$value = fread($in, 4);

	if ($bytesofdata <= 4) {
		//if datatype is 4 bytes or less, its the value
		$data = $value;
	} elseif ($bytesofdata < 100000) {
		//otherwise its a pointer to the value, so lets go get it
		$value = bin2hex($value);

		if ($intel == 1) {
			$value = intel2Moto($value);
		}

		//offsets are from TIFF header which is 12 bytes from the start of the file
		$v = fseek($seek, $globalOffset + hexdec($value));

		if ($v == 0) {
			$data = fread($seek, $bytesofdata);
		} elseif ($v == -1) {
			$result['Errors'] = $result['Errors'] + 1;
		}
	} else {
		//bytesofdata was too big, so the exif had an error
		$result['Errors'] = $result['Errors'] + 1;

		return;
	}

	if ($tag_name == 'MakerNote') {
		//if its a maker tag, we need to parse this specially
		$make = $result['IFD0']['Make'];

		if ($result['VerboseOutput'] == 1) {
			$result[$ifd_name]['MakerNote']['RawData'] = $data;
		}

		// not using stripos for php4 compatibility
		$capmake = strtoupper($make);

		if (strpos($capmake, 'NIKON') !== false) {
			include_once __DIR__ . '/makers/nikon.inc';

			parseNikon($data, $result);
			$result[$ifd_name]['KnownMaker'] = 1;
		} elseif (strpos($capmake, 'OLYMPUS') !== false) {
			include_once __DIR__ . '/makers/olympus.inc';

			parseOlympus($data, $result, $seek, $globalOffset);
			$result[$ifd_name]['KnownMaker'] = 1;
		} elseif (strpos($capmake, 'Canon') !== false) {
			include_once __DIR__ . '/makers/canon.inc';

			parseCanon($data, $result, $seek, $globalOffset);
			$result[$ifd_name]['KnownMaker'] = 1;
		} elseif (strpos($capmake, 'FUJIFILM') !== false) {
			include_once __DIR__ . '/makers/fujifilm.inc';

			parseFujifilm($data, $result);
			$result[$ifd_name]['KnownMaker'] = 1;
		} elseif (strpos($capmake, 'SANYO') !== false) {
			include_once __DIR__ . '/makers/sanyo.inc';

			parseSanyo($data, $result, $seek, $globalOffset);
			$result[$ifd_name]['KnownMaker'] = 1;
		} else {
			$result[$ifd_name]['KnownMaker'] = 0;
		}
	} elseif ($tag_name == 'GPSInfoOffset') {
		include_once __DIR__ . '/makers/gps.inc';

		$formated_data                = formatData($type, $tag, $intel, $data);
		$result[$ifd_name]['GPSInfo'] = $formated_data;
		parseGPS($data, $result, $formated_data, $seek, $globalOffset);
	} else {
		//Format the data depending on the type and tag
		$formated_data                = formatData($type, $tag, $intel, $data);
		$result[$ifd_name][$tag_name] = $formated_data;

		if ($result['VerboseOutput'] == 1) {
			if ($type == 'URATIONAL' || $type == 'SRATIONAL' || $type == 'USHORT' || $type == 'SSHORT' || $type == 'ULONG' || $type == 'SLONG' || $type == 'FLOAT' || $type == 'DOUBLE') {
				$data = bin2hex($data);

				if ($intel == 1) {
					$data = intel2Moto($data);
				}
			}

			$result[$ifd_name][$tag_name . '_Verbose']['RawData'] = $data;
			$result[$ifd_name][$tag_name . '_Verbose']['Type']    = $type;
			$result[$ifd_name][$tag_name . '_Verbose']['Bytes']   = $bytesofdata;
		}
	}
}

//================================================================================================
//================================================================================================
// Pass in a file and this reads the EXIF data
// Usefull resources
// http://www.ba.wakwak.com/~tsuruzoh/Computer/Digicams/exif-e.html
// http://www.w3.org/Graphics/JPEG/jfif.txt
// http://exif.org/
// http://www.ozhiker.com/electronics/pjmt/library/list_contents.php4
// http://www.ozhiker.com/electronics/pjmt/jpeg_info/makernotes.html
// http://pel.sourceforge.net/
// http://us2.php.net/manual/en/function.exif-read-data.php
//================================================================================================
//================================================================================================
function read_exif_data_raw($path, $verbose) {
	if ($path == '' || $path == 'none') {
		return;
	}

	//the b is for windows machines to open in binary mode
	$in = @fopen($path, 'rb');

	//There may be an elegant way to do this with one file handle.
	$seek         = @fopen($path, 'rb');
	$globalOffset = 0;

	if (!isset($verbose)) {
		$verbose = 0;
	}

	$result['VerboseOutput'] = $verbose;
	$result['Errors']        = 0;

	if (!$in || !$seek) {
		//if the path was invalid, this error will catch it
		$result['Errors']                   = 1;
		$result['Error'][$result['Errors']] = 'The file could not be found.';

		return $result;
	}

	$GLOBALS['exiferFileSize'] = filesize($path);

	//First 2 bytes of JPEG are 0xFFD8
	$data = bin2hex(fread($in, 2));

	if ($data == 'ffd8') {
		$result['ValidJpeg'] = 1;
	} else {
		$result['ValidJpeg'] = 0;
		fclose($in);
		fclose($seek);

		return $result;
	}

	$result['ValidIPTCData'] = 0;
	$result['ValidJFIFData'] = 0;
	$result['ValidEXIFData'] = 0;
	$result['ValidAPP2Data'] = 0;
	$result['ValidCOMData']  = 0;

	//Next 2 bytes are MARKER tag (0xFFE#)
	$data = bin2hex(fread($in, 2));
	$size = bin2hex(fread($in, 2));

	//LOOP THROUGH MARKERS TILL YOU GET TO FFE1	(exif marker)
	$abortCount = 0;

	while (!feof($in) && $data != 'ffe1' && $data != 'ffc0' && $data != 'ffd9' && ++$abortCount < 200) {
		if ($data == 'ffe0') {
			//JFIF Marker
			$result['ValidJFIFData'] = 1;
			$result['JFIF']['Size']  = hexdec($size);

			if (hexdec($size) - 2 > 0) {
				$data                   = fread($in, hexdec($size) - 2);
				$result['JFIF']['Data'] = $data;
			}

			$result['JFIF']['Identifier']    = substr($data, 0, 5);
			$result['JFIF']['ExtensionCode'] = bin2hex(substr($data, 6, 1));
			$globalOffset                   += hexdec($size) + 2;
		} elseif ($data == 'ffed') {
			//IPTC Marker
			$result['ValidIPTCData'] = 1;
			$result['IPTC']['Size']  = hexdec($size);

			if (hexdec($size) - 2 > 0) {
				$data                   = fread($in, hexdec($size) - 2);
				$result['IPTC']['Data'] = $data;
			}

			$globalOffset += hexdec($size) + 2;
		} elseif ($data == 'ffe2') {
			//EXIF extension Marker
			$result['ValidAPP2Data'] = 1;
			$result['APP2']['Size']  = hexdec($size);

			if (hexdec($size) - 2 > 0) {
				$data                   = fread($in, hexdec($size) - 2);
				$result['APP2']['Data'] = $data;
			}

			$globalOffset += hexdec($size) + 2;
		} elseif ($data == 'fffe') {
			//COM extension Marker
			$result['ValidCOMData'] = 1;
			$result['COM']['Size']  = hexdec($size);

			if (hexdec($size) - 2 > 0) {
				$data                  = fread($in, hexdec($size) - 2);
				$result['COM']['Data'] = $data;
			}

			$globalOffset += hexdec($size) + 2;
		} elseif ($data == 'ffe1') {
			$result['ValidEXIFData'] = 1;
		}

		$data = bin2hex(fread($in, 2));
		$size = bin2hex(fread($in, 2));
	}

	//END MARKER LOOP
	if ($data == 'ffe1') {
		$result['ValidEXIFData'] = 1;
	} else {
		fclose($in);
		fclose($seek);

		return $result;
	}

	//Size of APP1
	$result['APP1Size'] = hexdec($size);

	//Start of APP1 block starts with "Exif" header (6 bytes)
	$header = fread($in, 6);

	//Then theres a TIFF header with 2 bytes of endieness (II or MM)
	$header = fread($in, 2);

	if ($header === 'II') {
		$intel            = 1;
		$result['Endien'] = 'Intel';
	} elseif ($header === 'MM') {
		$intel            = 0;
		$result['Endien'] = 'Motorola';
	} else {
		//not sure what the default should be, but this seems reasonable
		$intel            = 1;
		$result['Endien'] = 'Unknown';
	}

	//2 bytes of 0x002a
	$tag = bin2hex(fread($in, 2));

	//Then 4 bytes of offset to IFD0 (usually 8 which includes all 8 bytes of TIFF header)
	$offset = bin2hex(fread($in, 4));

	if ($intel == 1) {
		$offset = intel2Moto($offset);
	}

	// Check for extremely large values here
	if (hexdec($offset) > 100000) {
		$result['ValidEXIFData'] = 0;
		fclose($in);
		fclose($seek);

		return $result;
	}

	if (hexdec($offset) > 8) {
		$unknown = fread($in, hexdec($offset) - 8);
	} //fixed this bug in 1.3

	//add 12 to the offset to account for TIFF header
	$globalOffset += 12;

	//===========================================================Start of IFD0
	$num = bin2hex(fread($in, 2));

	if ($intel == 1) {
		$num = intel2Moto($num);
	}

	$num                   = hexdec($num);
	$result['IFD0NumTags'] = $num;

	if ($num < 1000) {
		//1000 entries is too much and is probably an error.
		for ($i = 0; $i < $num; $i++) {
			read_entry($result, $in, $seek, $intel, 'IFD0', $globalOffset);
		}
	} else {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Illegal size for IFD0';
	}

	//store offset to IFD1
	$offset = bin2hex(fread($in, 4));

	if ($intel == 1) {
		$offset = intel2Moto($offset);
	}

	$result['IFD1Offset'] = hexdec($offset);

	//Check for SubIFD
	if (!isset($result['IFD0']['ExifOffset']) || $result['IFD0']['ExifOffset'] == 0) {
		fclose($in);
		fclose($seek);

		return $result;
	}

	//seek to SubIFD (Value of ExifOffset tag) above.
	$ExitOffset = $result['IFD0']['ExifOffset'];
	$v          = fseek($in, $globalOffset + $ExitOffset);

	if ($v == -1) {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Couldnt Find SubIFD';
	}

	//===========================================================Start of SubIFD
	$num = bin2hex(fread($in, 2));

	if ($intel == 1) {
		$num = intel2Moto($num);
	}

	$num                     = hexdec($num);
	$result['SubIFDNumTags'] = $num;

	if ($num < 1000) {
		//1000 entries is too much and is probably an error.
		for ($i = 0; $i < $num; $i++) {
			read_entry($result, $in, $seek, $intel, 'SubIFD', $globalOffset);
		}
	} else {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Illegal size for SubIFD';
	}

	//Check for IFD1
	if (!isset($result['IFD1Offset']) || $result['IFD1Offset'] == 0) {
		fclose($in);
		fclose($seek);

		return $result;
	}

	//seek to IFD1
	$v = fseek($in, $globalOffset + $result['IFD1Offset']);

	if ($v == -1) {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Couldnt Find IFD1';
	}

	//===========================================================Start of IFD1
	$num = bin2hex(fread($in, 2));

	if ($intel == 1) {
		$num = intel2Moto($num);
	}

	$num                   = hexdec($num);
	$result['IFD1NumTags'] = $num;

	if ($num < 1000) {
		//1000 entries is too much and is probably an error.
		for ($i = 0; $i < $num; $i++) {
			read_entry($result, $in, $seek, $intel, 'IFD1', $globalOffset);
		}
	} else {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Illegal size for IFD1';
	}

	//if verbose output is on, stick in the thumbnail raw data
	if ($result['VerboseOutput'] == 1 && $result['IFD1']['JpegIFOffset'] > 0 && $result['IFD1']['JpegIFByteCount'] > 0) {
		$v = fseek($seek, $globalOffset + $result['IFD1']['JpegIFOffset']);

		if ($v == 0) {
			$data = fread($seek, $result['IFD1']['JpegIFByteCount']);
		} elseif ($v == -1) {
			$result['Errors'] = $result['Errors'] + 1;
		}

		$result['IFD1']['ThumbnailData'] = $data;
	}

	//Check for Interoperability IFD
	if (!isset($result['SubIFD']['ExifInteroperabilityOffset']) || $result['SubIFD']['ExifInteroperabilityOffset'] == 0) {
		fclose($in);
		fclose($seek);

		return $result;
	}

	//seek to InteroperabilityIFD
	$v = fseek($in, $globalOffset + $result['SubIFD']['ExifInteroperabilityOffset']);

	if ($v == -1) {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Couldnt Find InteroperabilityIFD';
	}

	//===========================================================Start of InteroperabilityIFD
	$num = bin2hex(fread($in, 2));

	if ($intel == 1) {
		$num = intel2Moto($num);
	}

	$num                                  = hexdec($num);
	$result['InteroperabilityIFDNumTags'] = $num;

	if ($num < 1000) {
		//1000 entries is too much and is probably an error.
		for ($i = 0; $i < $num; $i++) {
			read_entry($result, $in, $seek, $intel, 'InteroperabilityIFD', $globalOffset);
		}
	} else {
		$result['Errors']                   = $result['Errors'] + 1;
		$result['Error'][$result['Errors']] = 'Illegal size for InteroperabilityIFD';
	}

	fclose($in);
	fclose($seek);

	return $result;
}

//================================================================================================
//================================================================================================
// Converts a floating point number into a fraction.  Many thanks to Matthieu Froment for this code
//================================================================================================
//================================================================================================
function ConvertToFraction($v, &$n, &$d) {
	//Limit to prevent infinite loop
	$MaxTerms = 15;

	//Limit to prevent divide by zero
	$MinDivisor = 1E-6;

	//How close is enough
	$MaxError = 1E-8;

	//Initialize fraction being converted
	$f = $v;

	//Initialize fractions with 1/0, 0/1
	$n_un   = 1;
	$d_un   = 0;
	$n_deux = 0;
	$d_deux = 1;

	for ($i = 0; $i < $MaxTerms; $i++) {
		//Get next term
		$a = floor($f);

		//Get new divisor
		$f = $f - $a;

		//Calculate new fraction
		$n = $n_un * $a + $n_deux;
		$d = $d_un * $a + $d_deux;

		//Save last two fractions
		$n_deux = $n_un;
		$d_deux = $d_un;
		$n_un   = $n;
		$d_un   = $d;

		if ($f < $MinDivisor) {
			//Quit if dividing by zero
			break;
		}

		if (abs($v - $n / $d) < $MaxError) {
			break;
		}

		//Take reciprocal
		$f = 1 / $f;
	}
}
