<?php
//================================================================================================
//================================================================================================
//================================================================================================
/*
	Exifer
	Extracts EXIF information from digital photos.
	
	Copyright � 2003 Jake Olefsky
	http://www.offsky.com/software/exif/index.php
	jake@olefsky.com
	
	Please see exif.php for the complete information about this software.
	
	------------
	
	This program is free software; you can redistribute it and/or modify it under the terms of 
	the GNU General Public License as published by the Free Software Foundation; either version 2 
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
	See the GNU General Public License for more details. http://www.gnu.org/copyleft/gpl.html
*/
//================================================================================================
//================================================================================================
//================================================================================================


//=================
// Looks up the name of the tag for the MakerNote (Depends on Manufacturer)
//====================================================================
function lookup_Canon_tag($tag) {
	
	switch($tag) {
		case "0001": $tag = "Settings 1";break;
		case "0004": $tag = "Settings 4";break;
		case "0006": $tag = "ImageType";break;
		case "0007": $tag = "FirmwareVersion";break;
		case "0008": $tag = "ImageNumber";break;
		case "0009": $tag = "OwnerName";break;
		case "000c": $tag = "CameraSerialNumber";break;	
		case "000f": $tag = "CustomFunctions";break;	
		
		default: $tag = "unknown:".$tag;break;
	}
	
	return $tag;
}

//=================
// Formats Data for the data type
//====================================================================
function formatCanonData($type,$tag,$intel,$data,$exif,&$result) {
	$place = 0;
	
	
	if($type=="ASCII") {
		$result = $data = str_replace("\0", "", $data);
	} else if($type=="URATIONAL" || $type=="SRATIONAL") {
		$data = bin2hex($data);
		if($intel==1) $data = intel2Moto($data);
		$top = hexdec(substr($data,8,8));
		$bottom = hexdec(substr($data,0,8));
		if($bottom!=0) $data=$top/$bottom;
		else if($top==0) $data = 0;
		else $data=$top."/".$bottom;
	
		if($tag=="0204") { //DigitalZoom
			$data=$data."x";
		} 
		
	} else if($type=="USHORT" || $type=="SSHORT" || $type=="ULONG" || $type=="SLONG" || $type=="FLOAT" || $type=="DOUBLE") {
		
		$data = bin2hex($data);
		$result['RAWDATA'] = $data;
	
		if($tag=="0001") { //first chunk
			$result['Bytes']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//0
			if ($result['Bytes'] != strlen($data) / 2) return $result; //Bad chunk
			$result['Macro']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//1
			switch($result['Macro']) {
				case 1: $result['Macro'] = "Macro"; break;
				case 2: $result['Macro'] = "Normal"; break;
				default: $result['Macro'] = "Unknown";
			}
			$result['SelfTimer']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//2
			switch($result['SelfTimer']) {
				case 0: $result['SelfTimer'] = "Off"; break;
				default: $result['SelfTimer'] .= "/10s";
			}
			$result['Quality']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//3
			switch($result['Quality']) {
				case 2: $result['Quality'] = "Normal"; break;
				case 3: $result['Quality'] = "Fine"; break;
				case 5: $result['Quality'] = "Superfine"; break;
				default: $result['Quality'] = "Unknown";
			}
			$result['Flash']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//4
			switch($result['Flash']) {
				case 0: $result['Flash'] = "Off"; break;
				case 1: $result['Flash'] = "Auto"; break;
				case 2: $result['Flash'] = "On"; break;
				case 3: $result['Flash'] = "Red Eye Reduction"; break;
				case 4: $result['Flash'] = "Slow Synchro"; break;
				case 5: $result['Flash'] = "Auto + Red Eye Reduction"; break;
				case 6: $result['Flash'] = "On + Red Eye Reduction"; break;
				case 16: $result['Flash'] = "External Flash"; break;
				default: $result['Flash'] = "Unknown";
			}
			$result['DriveMode']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//5
			switch($result['DriveMode']) {
				case 0: $result['DriveMode'] = "Single/Timer"; break;
				case 1: $result['DriveMode'] = "Continuous"; break;
				default: $result['DriveMode'] = "Unknown";
			}
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//6
			$result['FocusMode']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//7
			switch($result['FocusMode']) {
				case 0: $result['FocusMode'] = "One-Shot"; break;
				case 1: $result['FocusMode'] = "AI Servo"; break;
				case 2: $result['FocusMode'] = "AI Focus"; break;
				case 3: $result['FocusMode'] = "Manual Focus"; break;
				case 4: $result['FocusMode'] = "Single"; break;
				case 5: $result['FocusMode'] = "Continuous"; break;
				case 6: $result['FocusMode'] = "Manual Focus"; break;
				default: $result['FocusMode'] = "Unknown";
			}
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//8
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//9
			$result['ImageSize']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//10
			switch($result['ImageSize']) {
				case 0: $result['ImageSize'] = "Large"; break;
				case 1: $result['ImageSize'] = "Medium"; break;
				case 2: $result['ImageSize'] = "Small"; break;
				default: $result['ImageSize'] = "Unknown";
			}
			$result['EasyShooting']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//11
			switch($result['EasyShooting']) {
				case 0: $result['EasyShooting'] = "Full Auto"; break;
				case 1: $result['EasyShooting'] = "Manual"; break;
				case 2: $result['EasyShooting'] = "Landscape"; break;
				case 3: $result['EasyShooting'] = "Fast Shutter"; break;
				case 4: $result['EasyShooting'] = "Slow Shutter"; break;
				case 5: $result['EasyShooting'] = "Night"; break;
				case 6: $result['EasyShooting'] = "Black & White"; break;
				case 7: $result['EasyShooting'] = "Sepia"; break;
				case 8: $result['EasyShooting'] = "Portrait"; break;
				case 9: $result['EasyShooting'] = "Sport"; break;
				case 10: $result['EasyShooting'] = "Macro/Close-Up"; break;
				case 11: $result['EasyShooting'] = "Pan Focus"; break;
				default: $result['EasyShooting'] = "Unknown";
			}
			$result['DigitalZoom']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//12
			switch($result['DigitalZoom']) {
				case 0:
				case 65535: $result['DigitalZoom'] = "None"; break;
				case 1: $result['DigitalZoom'] = "2x"; break;
				case 2: $result['DigitalZoom'] = "4x"; break;
				default: $result['DigitalZoom'] = "Unknown";
			}
			$result['Contrast']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//13
			switch($result['Contrast']) {
				case 0: $result['Contrast'] = "Normal"; break;
				case 1: $result['Contrast'] = "High"; break;
				case 65535: $result['Contrast'] = "Low"; break;
				default: $result['Contrast'] = "Unknown";
			}
			$result['Saturation']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//14
			switch($result['Saturation']) {
				case 0: $result['Saturation'] = "Normal"; break;
				case 1: $result['Saturation'] = "High"; break;
				case 65535: $result['Saturation'] = "Low"; break;
				default: $result['Saturation'] = "Unknown";
			}
			$result['Sharpness']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//15
			switch($result['Sharpness']) {
				case 0: $result['Sharpness'] = "Normal"; break;
				case 1: $result['Sharpness'] = "High"; break;
				case 65535: $result['Sharpness'] = "Low"; break;
				default: $result['Sharpness'] = "Unknown";
			}
			$result['ISO']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//16
			switch($result['ISO']) {
				case 32767:
				case 0: $result['ISO'] = isset($exif['SubIFD']['ISOSpeedRatings'])
					? $exif['SubIFD']['ISOSpeedRatings'] : 'Unknown'; break;
				case 15: $result['ISO'] = "Auto"; break;
				case 16: $result['ISO'] = "50"; break;
				case 17: $result['ISO'] = "100"; break;
				case 18: $result['ISO'] = "200"; break;
				case 19: $result['ISO'] = "400"; break;
				default: $result['ISO'] = "Unknown";
			}
			$result['MeteringMode']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//17
			switch($result['MeteringMode']) {
				case 3: $result['MeteringMode'] = "Evaluative"; break;
				case 4: $result['MeteringMode'] = "Partial"; break;
				case 5: $result['MeteringMode'] = "Center-weighted"; break;
				default: $result['MeteringMode'] = "Unknown";
			}
			$result['FocusType']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//18
			switch($result['FocusType']) {
				case 0: $result['FocusType'] = "Manual"; break;
				case 1: $result['FocusType'] = "Auto"; break;
				case 3: $result['FocusType'] = "Close-up (Macro)"; break;
				case 8: $result['FocusType'] = "Locked (Pan Mode)"; break;
				default: $result['FocusType'] = "Unknown";
			}
			$result['AFPointSelected']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//19
			switch($result['AFPointSelected']) {
				case 12288: $result['AFPointSelected'] = "Manual Focus"; break;
				case 12289: $result['AFPointSelected'] = "Auto Selected"; break;
				case 12290: $result['AFPointSelected'] = "Right"; break;
				case 12291: $result['AFPointSelected'] = "Center"; break;
				case 12292: $result['AFPointSelected'] = "Left"; break;
				default: $result['AFPointSelected'] = "Unknown";
			}
			$result['ExposureMode']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//20
			switch($result['ExposureMode']) {
				case 0: $result['ExposureMode'] = "EasyShoot"; break;
				case 1: $result['ExposureMode'] = "Program"; break;
				case 2: $result['ExposureMode'] = "Tv"; break;
				case 3: $result['ExposureMode'] = "Av"; break;
				case 4: $result['ExposureMode'] = "Manual"; break;
				case 5: $result['ExposureMode'] = "Auto-DEP"; break;
				default: $result['ExposureMode'] = "Unknown";
			}
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//21
			$result['LensType']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//22
				if($result['LensType']==1) $result['LensType'] = "Canon EF 50mm f/1.8";
				if($result['LensType']==2) $result['LensType'] = "Canon EF 28mm f/2.8";
				if($result['LensType']==3) $result['LensType'] = "Canon EF 135mm f/2.8 Soft";
				if($result['LensType']==4) $result['LensType'] = "Canon EF 35-105mm f/3.5-4.5 or Sigma Lens";
				if($result['LensType']==4.1) $result['LensType'] = "Sigma UC Zoom 35-135mm f/4-5.6";
				if($result['LensType']==5) $result['LensType'] = "Canon EF 35-70mm f/3.5-4.5";
				if($result['LensType']==6) $result['LensType'] = "Canon EF 28-70mm f/3.5-4.5 or Sigma or Tokina Lens";
				if($result['LensType']==6.1) $result['LensType'] = "Sigma 18-50mm f/3.5-5.6 DC";
				if($result['LensType']==6.2) $result['LensType'] = "Sigma 18-125mm f/3.5-5.6 DC IF ASP";
				if($result['LensType']==6.3) $result['LensType'] = "Tokina AF193-2 19-35mm f/3.5-4.5";
				if($result['LensType']==6.4) $result['LensType'] = "Sigma 28-80mm f/3.5-5.6 II Macro";
				if($result['LensType']==7) $result['LensType'] = "Canon EF 100-300mm f/5.6L";
				if($result['LensType']==8) $result['LensType'] = "Canon EF 100-300mm f/5.6 or Sigma or Tokina Lens";
				if($result['LensType']==8.1) $result['LensType'] = "Sigma 70-300mm f/4-5.6 [APO] DG Macro";
				if($result['LensType']==8.2) $result['LensType'] = "Tokina AT-X242AF 24-200mm f/3.5-5.6";
				if($result['LensType']==9) $result['LensType'] = "Canon EF 70-210mm f/4";
				if($result['LensType']==9.1) $result['LensType'] = "Sigma 55-200mm f/4-5.6 DC";
				if($result['LensType']==10) $result['LensType'] = "Canon EF 50mm f/2.5 Macro or Sigma Lens";
				if($result['LensType']==10.1) $result['LensType'] = "Sigma 50mm f/2.8 EX";
				if($result['LensType']==10.2) $result['LensType'] = "Sigma 28mm f/1.8";
				if($result['LensType']==10.3) $result['LensType'] = "Sigma 105mm f/2.8 Macro EX";
				if($result['LensType']==10.4) $result['LensType'] = "Sigma 70mm f/2.8 EX DG Macro EF";
				if($result['LensType']==11) $result['LensType'] = "Canon EF 35mm f/2";
				if($result['LensType']==13) $result['LensType'] = "Canon EF 15mm f/2.8 Fisheye";
				if($result['LensType']==14) $result['LensType'] = "Canon EF 50-200mm f/3.5-4.5L";
				if($result['LensType']==15) $result['LensType'] = "Canon EF 50-200mm f/3.5-4.5";
				if($result['LensType']==16) $result['LensType'] = "Canon EF 35-135mm f/3.5-4.5";
				if($result['LensType']==17) $result['LensType'] = "Canon EF 35-70mm f/3.5-4.5A";
				if($result['LensType']==18) $result['LensType'] = "Canon EF 28-70mm f/3.5-4.5";
				if($result['LensType']==20) $result['LensType'] = "Canon EF 100-200mm f/4.5A";
				if($result['LensType']==21) $result['LensType'] = "Canon EF 80-200mm f/2.8L";
				if($result['LensType']==22) $result['LensType'] = "Canon EF 20-35mm f/2.8L or Tokina Lens";
				if($result['LensType']==22.1) $result['LensType'] = "Tokina AT-X280AF PRO 28-80mm f/2.8 Aspherical";
				if($result['LensType']==23) $result['LensType'] = "Canon EF 35-105mm f/3.5-4.5";
				if($result['LensType']==24) $result['LensType'] = "Canon EF 35-80mm f/4-5.6 Power Zoom";
				if($result['LensType']==25) $result['LensType'] = "Canon EF 35-80mm f/4-5.6 Power Zoom";
				if($result['LensType']==26) $result['LensType'] = "Canon EF 100mm f/2.8 Macro or Other Lens";
				if($result['LensType']==26.1) $result['LensType'] = "Cosina 100mm f/3.5 Macro AF";
				if($result['LensType']==26.2) $result['LensType'] = "Tamron SP AF 90mm f/2.8 Di Macro";
				if($result['LensType']==26.3) $result['LensType'] = "Tamron SP AF 180mm f/3.5 Di Macro";
				if($result['LensType']==26.4) $result['LensType'] = "Carl Zeiss Planar T* 50mm f/1.4";
				if($result['LensType']==27) $result['LensType'] = "Canon EF 35-80mm f/4-5.6";
				if($result['LensType']==28) $result['LensType'] = "Canon EF 80-200mm f/4.5-5.6 or Tamron Lens";
				if($result['LensType']==28.1) $result['LensType'] = "Tamron SP AF 28-105mm f/2.8 LD Aspherical IF";
				if($result['LensType']==28.2) $result['LensType'] = "Tamron SP AF 28-75mm f/2.8 XR Di LD Aspherical [IF] Macro";
				if($result['LensType']==28.3) $result['LensType'] = "Tamron AF 70-300mm f/4.5-5.6 Di LD 1:2 Macro Zoom";
				if($result['LensType']==28.4) $result['LensType'] = "Tamron AF Aspherical 28-200mm f/3.8-5.6";
				if($result['LensType']==29) $result['LensType'] = "Canon EF 50mm f/1.8 II";
				if($result['LensType']==30) $result['LensType'] = "Canon EF 35-105mm f/4.5-5.6";
				if($result['LensType']==31) $result['LensType'] = "Canon EF 75-300mm f/4-5.6 or Tamron Lens";
				if($result['LensType']==31.1) $result['LensType'] = "Tamron SP AF 300mm f/2.8 LD IF";
				if($result['LensType']==32) $result['LensType'] = "Canon EF 24mm f/2.8 or Sigma Lens";
				if($result['LensType']==32.1) $result['LensType'] = "Sigma 15mm f/2.8 EX Fisheye";
				if($result['LensType']==33) $result['LensType'] = "Voigtlander or Zeiss Lens";
				if($result['LensType']==33.1) $result['LensType'] = "Voigtlander Ultron 40mm f/2 SLII Aspherical";
				if($result['LensType']==33.2) $result['LensType'] = "Zeiss Distagon 35mm T* f/2 ZE";
				if($result['LensType']==35) $result['LensType'] = "Canon EF 35-80mm f/4-5.6";
				if($result['LensType']==36) $result['LensType'] = "Canon EF 38-76mm f/4.5-5.6";
				if($result['LensType']==37) $result['LensType'] = "Canon EF 35-80mm f/4-5.6 or Tamron Lens";
				if($result['LensType']==37.1) $result['LensType'] = "Tamron 70-200mm f/2.8 Di LD IF Macro";
				if($result['LensType']==37.2) $result['LensType'] = "Tamron AF 28-300mm f/3.5-6.3 XR Di VC LD Aspherical [IF] Macro Model A20";
				if($result['LensType']==37.3) $result['LensType'] = "Tamron SP AF 17-50mm f/2.8 XR Di II VC LD Aspherical [IF]";
				if($result['LensType']==37.4) $result['LensType'] = "Tamron AF 18-270mm f/3.5-6.3 Di II VC LD Aspherical [IF] Macro";
				if($result['LensType']==38) $result['LensType'] = "Canon EF 80-200mm f/4.5-5.6";
				if($result['LensType']==39) $result['LensType'] = "Canon EF 75-300mm f/4-5.6";
				if($result['LensType']==40) $result['LensType'] = "Canon EF 28-80mm f/3.5-5.6";
				if($result['LensType']==41) $result['LensType'] = "Canon EF 28-90mm f/4-5.6";
				if($result['LensType']==42) $result['LensType'] = "Canon EF 28-200mm f/3.5-5.6 or Tamron Lens";
				if($result['LensType']==42.1) $result['LensType'] = "Tamron AF 28-300mm f/3.5-6.3 XR Di VC LD Aspherical [IF] Macro Model A20";
				if($result['LensType']==43) $result['LensType'] = "Canon EF 28-105mm f/4-5.6";
				if($result['LensType']==44) $result['LensType'] = "Canon EF 90-300mm f/4.5-5.6";
				if($result['LensType']==45) $result['LensType'] = "Canon EF-S 18-55mm f/3.5-5.6 [II]";
				if($result['LensType']==46) $result['LensType'] = "Canon EF 28-90mm f/4-5.6";
				if($result['LensType']==48) $result['LensType'] = "Canon EF-S 18-55mm f/3.5-5.6 IS";
				if($result['LensType']==49) $result['LensType'] = "Canon EF-S 55-250mm f/4-5.6 IS";
				if($result['LensType']==50) $result['LensType'] = "Canon EF-S 18-200mm f/3.5-5.6 IS";
				if($result['LensType']==51) $result['LensType'] = "Canon EF-S 18-135mm f/3.5-5.6 IS";
				if($result['LensType']==52) $result['LensType'] = "Canon EF-S 18-55mm f/3.5-5.6 IS II";
				if($result['LensType']==94) $result['LensType'] = "Canon TS-E 17mm f/4L";
				if($result['LensType']==95) $result['LensType'] = "Canon TS-E 24.0mm f/3.5 L II";
				if($result['LensType']==124) $result['LensType'] = "Canon MP-E 65mm f/2.8 1-5x Macro Photo";
				if($result['LensType']==125) $result['LensType'] = "Canon TS-E 24mm f/3.5L";
				if($result['LensType']==126) $result['LensType'] = "Canon TS-E 45mm f/2.8";
				if($result['LensType']==127) $result['LensType'] = "Canon TS-E 90mm f/2.8";
				if($result['LensType']==129) $result['LensType'] = "Canon EF 300mm f/2.8L";
				if($result['LensType']==130) $result['LensType'] = "Canon EF 50mm f/1.0L";
				if($result['LensType']==131) $result['LensType'] = "Canon EF 28-80mm f/2.8-4L or Sigma Lens";
				if($result['LensType']==131.1) $result['LensType'] = "Sigma 8mm f/3.5 EX DG Circular Fisheye";
				if($result['LensType']==131.2) $result['LensType'] = "Sigma 17-35mm f/2.8-4 EX DG Aspherical HSM";
				if($result['LensType']==131.3) $result['LensType'] = "Sigma 17-70mm f/2.8-4.5 DC Macro";
				if($result['LensType']==131.4) $result['LensType'] = "Sigma APO 50-150mm f/2.8 [II] EX DC HSM";
				if($result['LensType']==131.5) $result['LensType'] = "Sigma APO 120-300mm f/2.8 EX DG HSM";
				if($result['LensType']==131.6) $result['LensType'] = "Sigma 4.5mm F2.8 EX DC HSM Circular Fisheye";
				if($result['LensType']==132) $result['LensType'] = "Canon EF 1200mm f/5.6L";
				if($result['LensType']==134) $result['LensType'] = "Canon EF 600mm f/4L IS";
				if($result['LensType']==135) $result['LensType'] = "Canon EF 200mm f/1.8L";
				if($result['LensType']==136) $result['LensType'] = "Canon EF 300mm f/2.8L";
				if($result['LensType']==137) $result['LensType'] = "Canon EF 85mm f/1.2L or Sigma or Tamron Lens";
				if($result['LensType']==137.1) $result['LensType'] = "Sigma 18-50mm f/2.8-4.5 DC OS HSM";
				if($result['LensType']==137.2) $result['LensType'] = "Sigma 50-200mm f/4-5.6 DC OS HSM";
				if($result['LensType']==137.3) $result['LensType'] = "Sigma 18-250mm f/3.5-6.3 DC OS HSM";
				if($result['LensType']==137.4) $result['LensType'] = "Sigma 24-70mm f/2.8 IF EX DG HSM";
				if($result['LensType']==137.5) $result['LensType'] = "Sigma 18-125mm f/3.8-5.6 DC OS HSM";
				if($result['LensType']==137.6) $result['LensType'] = "Sigma 17-70mm f/2.8-4 DC Macro OS HSM";
				if($result['LensType']==137.7) $result['LensType'] = "Sigma 17-50mm f/2.8 OS HSM";
				if($result['LensType']==137.8) $result['LensType'] = "Tamron AF 18-270mm f/3.5-6.3 Di II VC PZD";
				if($result['LensType']==138) $result['LensType'] = "Canon EF 28-80mm f/2.8-4L";
				if($result['LensType']==139) $result['LensType'] = "Canon EF 400mm f/2.8L";
				if($result['LensType']==140) $result['LensType'] = "Canon EF 500mm f/4.5L";
				if($result['LensType']==141) $result['LensType'] = "Canon EF 500mm f/4.5L";
				if($result['LensType']==142) $result['LensType'] = "Canon EF 300mm f/2.8L IS";
				if($result['LensType']==143) $result['LensType'] = "Canon EF 500mm f/4L IS";
				if($result['LensType']==144) $result['LensType'] = "Canon EF 35-135mm f/4-5.6 USM";
				if($result['LensType']==145) $result['LensType'] = "Canon EF 100-300mm f/4.5-5.6 USM";
				if($result['LensType']==146) $result['LensType'] = "Canon EF 70-210mm f/3.5-4.5 USM";
				if($result['LensType']==147) $result['LensType'] = "Canon EF 35-135mm f/4-5.6 USM";
				if($result['LensType']==148) $result['LensType'] = "Canon EF 28-80mm f/3.5-5.6 USM";
				if($result['LensType']==149) $result['LensType'] = "Canon EF 100mm f/2 USM";
				if($result['LensType']==150) $result['LensType'] = "Canon EF 14mm f/2.8L or Sigma Lens";
				if($result['LensType']==150.1) $result['LensType'] = "Sigma 20mm EX f/1.8";
				if($result['LensType']==150.2) $result['LensType'] = "Sigma 30mm f/1.4 DC HSM";
				if($result['LensType']==150.3) $result['LensType'] = "Sigma 24mm f/1.8 DG Macro EX";
				if($result['LensType']==151) $result['LensType'] = "Canon EF 200mm f/2.8L";
				if($result['LensType']==152) $result['LensType'] = "Canon EF 300mm f/4L IS or Sigma Lens";
				if($result['LensType']==152.1) $result['LensType'] = "Sigma 12-24mm f/4.5-5.6 EX DG ASPHERICAL HSM";
				if($result['LensType']==152.2) $result['LensType'] = "Sigma 14mm f/2.8 EX Aspherical HSM";
				if($result['LensType']==152.3) $result['LensType'] = "Sigma 10-20mm f/4-5.6";
				if($result['LensType']==152.4) $result['LensType'] = "Sigma 100-300mm f/4";
				if($result['LensType']==153) $result['LensType'] = "Canon EF 35-350mm f/3.5-5.6L or Sigma or Tamron Lens";
				if($result['LensType']==153.1) $result['LensType'] = "Sigma 50-500mm f/4-6.3 APO HSM EX";
				if($result['LensType']==153.2) $result['LensType'] = "Tamron AF 28-300mm f/3.5-6.3 XR LD Aspherical [IF] Macro";
				if($result['LensType']==153.3) $result['LensType'] = "Tamron AF 18-200mm f/3.5-6.3 XR Di II LD Aspherical [IF] Macro Model A14";
				if($result['LensType']==153.4) $result['LensType'] = "Tamron 18-250mm f/3.5-6.3 Di II LD Aspherical [IF] Macro";
				if($result['LensType']==154) $result['LensType'] = "Canon EF 20mm f/2.8 USM";
				if($result['LensType']==155) $result['LensType'] = "Canon EF 85mm f/1.8 USM";
				if($result['LensType']==156) $result['LensType'] = "Canon EF 28-105mm f/3.5-4.5 USM or Tamron Lens";
				if($result['LensType']==156.1) $result['LensType'] = "Tamron SP 70-300mm f/4.0-5.6 Di VC USD";
				if($result['LensType']==160) $result['LensType'] = "Canon EF 20-35mm f/3.5-4.5 USM or Tamron or Tokina Lens";
				if($result['LensType']==160.1) $result['LensType'] = "Tamron AF 19-35mm f/3.5-4.5";
				if($result['LensType']==160.2) $result['LensType'] = "Tokina AT-X 124 AF 12-24mm f/4 DX";
				if($result['LensType']==160.3) $result['LensType'] = "Tokina AT-X 107 AF DX 10-17mm f/3.5-4.5 Fisheye";
				if($result['LensType']==161) $result['LensType'] = "Canon EF 28-70mm f/2.8L or Sigma or Tamron Lens";
				if($result['LensType']==161.1) $result['LensType'] = "Sigma 24-70mm f/2.8 EX";
				if($result['LensType']==161.2) $result['LensType'] = "Sigma 28-70mm f/2.8 EX";
				if($result['LensType']==161.3) $result['LensType'] = "Tamron AF 17-50mm f/2.8 Di-II LD Aspherical";
				if($result['LensType']==161.4) $result['LensType'] = "Tamron 90mm f/2.8";
				if($result['LensType']==162) $result['LensType'] = "Canon EF 200mm f/2.8L";
				if($result['LensType']==163) $result['LensType'] = "Canon EF 300mm f/4L";
				if($result['LensType']==164) $result['LensType'] = "Canon EF 400mm f/5.6L";
				if($result['LensType']==165) $result['LensType'] = "Canon EF 70-200mm f/2.8 L";
				if($result['LensType']==166) $result['LensType'] = "Canon EF 70-200mm f/2.8 L + 1.4x";
				if($result['LensType']==167) $result['LensType'] = "Canon EF 70-200mm f/2.8 L + 2x";
				if($result['LensType']==168) $result['LensType'] = "Canon EF 28mm f/1.8 USM";
				if($result['LensType']==169) $result['LensType'] = "Canon EF 17-35mm f/2.8L or Sigma Lens";
				if($result['LensType']==169.1) $result['LensType'] = "Sigma 18-200mm f/3.5-6.3 DC OS";
				if($result['LensType']==169.2) $result['LensType'] = "Sigma 15-30mm f/3.5-4.5 EX DG Aspherical";
				if($result['LensType']==169.3) $result['LensType'] = "Sigma 18-50mm f/2.8 Macro";
				if($result['LensType']==169.4) $result['LensType'] = "Sigma 50mm f/1.4 EX DG HSM";
				if($result['LensType']==169.5) $result['LensType'] = "Sigma 85mm f/1.4 EX DG HSM";
				if($result['LensType']==169.6) $result['LensType'] = "Sigma 30mm f/1.4 EX DC HSM";
				if($result['LensType']==170) $result['LensType'] = "Canon EF 200mm f/2.8L II";
				if($result['LensType']==171) $result['LensType'] = "Canon EF 300mm f/4L";
				if($result['LensType']==172) $result['LensType'] = "Canon EF 400mm f/5.6L";
				if($result['LensType']==173) $result['LensType'] = "Canon EF 180mm Macro f/3.5L or Sigma Lens";
				if($result['LensType']==173.1) $result['LensType'] = "Sigma 180mm EX HSM Macro f/3.5";
				if($result['LensType']==173.2) $result['LensType'] = "Sigma APO Macro 150mm f/2.8 EX DG HSM";
				if($result['LensType']==174) $result['LensType'] = "Canon EF 135mm f/2L or Sigma Lens";
				if($result['LensType']==174.1) $result['LensType'] = "Sigma 70-200mm f/2.8 EX DG APO OS HSM";
				if($result['LensType']==175) $result['LensType'] = "Canon EF 400mm f/2.8L";
				if($result['LensType']==176) $result['LensType'] = "Canon EF 24-85mm f/3.5-4.5 USM";
				if($result['LensType']==177) $result['LensType'] = "Canon EF 300mm f/4L IS";
				if($result['LensType']==178) $result['LensType'] = "Canon EF 28-135mm f/3.5-5.6 IS";
				if($result['LensType']==179) $result['LensType'] = "Canon EF 24mm f/1.4L";
				if($result['LensType']==180) $result['LensType'] = "Canon EF 35mm f/1.4L";
				if($result['LensType']==181) $result['LensType'] = "Canon EF 100-400mm f/4.5-5.6L IS + 1.4x";
				if($result['LensType']==182) $result['LensType'] = "Canon EF 100-400mm f/4.5-5.6L IS + 2x";
				if($result['LensType']==183) $result['LensType'] = "Canon EF 100-400mm f/4.5-5.6L IS";
				if($result['LensType']==184) $result['LensType'] = "Canon EF 400mm f/2.8L + 2x";
				if($result['LensType']==185) $result['LensType'] = "Canon EF 600mm f/4L IS";
				if($result['LensType']==186) $result['LensType'] = "Canon EF 70-200mm f/4L";
				if($result['LensType']==187) $result['LensType'] = "Canon EF 70-200mm f/4L + 1.4x";
				if($result['LensType']==188) $result['LensType'] = "Canon EF 70-200mm f/4L + 2x";
				if($result['LensType']==189) $result['LensType'] = "Canon EF 70-200mm f/4L + 2.8x";
				if($result['LensType']==190) $result['LensType'] = "Canon EF 100mm f/2.8 Macro";
				if($result['LensType']==191) $result['LensType'] = "Canon EF 400mm f/4 DO IS";
				if($result['LensType']==193) $result['LensType'] = "Canon EF 35-80mm f/4-5.6 USM";
				if($result['LensType']==194) $result['LensType'] = "Canon EF 80-200mm f/4.5-5.6 USM";
				if($result['LensType']==195) $result['LensType'] = "Canon EF 35-105mm f/4.5-5.6 USM";
				if($result['LensType']==196) $result['LensType'] = "Canon EF 75-300mm f/4-5.6 USM";
				if($result['LensType']==197) $result['LensType'] = "Canon EF 75-300mm f/4-5.6 IS USM";
				if($result['LensType']==198) $result['LensType'] = "Canon EF 50mm f/1.4 USM";
				if($result['LensType']==199) $result['LensType'] = "Canon EF 28-80mm f/3.5-5.6 USM";
				if($result['LensType']==200) $result['LensType'] = "Canon EF 75-300mm f/4-5.6 USM";
				if($result['LensType']==201) $result['LensType'] = "Canon EF 28-80mm f/3.5-5.6 USM";
				if($result['LensType']==202) $result['LensType'] = "Canon EF 28-80mm f/3.5-5.6 USM IV";
				if($result['LensType']==208) $result['LensType'] = "Canon EF 22-55mm f/4-5.6 USM";
				if($result['LensType']==209) $result['LensType'] = "Canon EF 55-200mm f/4.5-5.6";
				if($result['LensType']==210) $result['LensType'] = "Canon EF 28-90mm f/4-5.6 USM";
				if($result['LensType']==211) $result['LensType'] = "Canon EF 28-200mm f/3.5-5.6 USM";
				if($result['LensType']==212) $result['LensType'] = "Canon EF 28-105mm f/4-5.6 USM";
				if($result['LensType']==213) $result['LensType'] = "Canon EF 90-300mm f/4.5-5.6 USM";
				if($result['LensType']==214) $result['LensType'] = "Canon EF-S 18-55mm f/3.5-5.6 USM";
				if($result['LensType']==215) $result['LensType'] = "Canon EF 55-200mm f/4.5-5.6 II USM";
				if($result['LensType']==224) $result['LensType'] = "Canon EF 70-200mm f/2.8L IS";
				if($result['LensType']==225) $result['LensType'] = "Canon EF 70-200mm f/2.8L IS + 1.4x";
				if($result['LensType']==226) $result['LensType'] = "Canon EF 70-200mm f/2.8L IS + 2x";
				if($result['LensType']==227) $result['LensType'] = "Canon EF 70-200mm f/2.8L IS + 2.8x";
				if($result['LensType']==228) $result['LensType'] = "Canon EF 28-105mm f/3.5-4.5 USM";
				if($result['LensType']==229) $result['LensType'] = "Canon EF 16-35mm f/2.8L";
				if($result['LensType']==230) $result['LensType'] = "Canon EF 24-70mm f/2.8L";
				if($result['LensType']==231) $result['LensType'] = "Canon EF 17-40mm f/4L";
				if($result['LensType']==232) $result['LensType'] = "Canon EF 70-300mm f/4.5-5.6 DO IS USM";
				if($result['LensType']==233) $result['LensType'] = "Canon EF 28-300mm f/3.5-5.6L IS";
				if($result['LensType']==234) $result['LensType'] = "Canon EF-S 17-85mm f4-5.6 IS USM";
				if($result['LensType']==235) $result['LensType'] = "Canon EF-S 10-22mm f/3.5-4.5 USM";
				if($result['LensType']==236) $result['LensType'] = "Canon EF-S 60mm f/2.8 Macro USM";
				if($result['LensType']==237) $result['LensType'] = "Canon EF 24-105mm f/4L IS";
				if($result['LensType']==238) $result['LensType'] = "Canon EF 70-300mm f/4-5.6 IS USM";
				if($result['LensType']==239) $result['LensType'] = "Canon EF 85mm f/1.2L II";
				if($result['LensType']==240) $result['LensType'] = "Canon EF-S 17-55mm f/2.8 IS USM";
				if($result['LensType']==241) $result['LensType'] = "Canon EF 50mm f/1.2L";
				if($result['LensType']==242) $result['LensType'] = "Canon EF 70-200mm f/4L IS";
				if($result['LensType']==243) $result['LensType'] = "Canon EF 70-200mm f/4L IS + 1.4x";
				if($result['LensType']==244) $result['LensType'] = "Canon EF 70-200mm f/4L IS + 2x";
				if($result['LensType']==245) $result['LensType'] = "Canon EF 70-200mm f/4L IS + 2.8x";
				if($result['LensType']==246) $result['LensType'] = "Canon EF 16-35mm f/2.8L II";
				if($result['LensType']==247) $result['LensType'] = "Canon EF 14mm f/2.8L II USM";
				if($result['LensType']==248) $result['LensType'] = "Canon EF 200mm f/2L IS";
				if($result['LensType']==249) $result['LensType'] = "Canon EF 800mm f/5.6L IS";
				if($result['LensType']==250) $result['LensType'] = "Canon EF 24 f/1.4L II";
				if($result['LensType']==251) $result['LensType'] = "Canon EF 70-200mm f/2.8L IS II USM";
				if($result['LensType']==254) $result['LensType'] = "Canon EF 100mm f/2.8L Macro IS USM";
				if($result['LensType']==488) $result['LensType'] = "Canon EF-S 15-85mm f/3.5-5.6 IS USM";
				if($result['LensType']==489) $result['LensType'] = "Canon EF 70-300mm f/4-5.6L IS USM";
				if($result['LensType']==65535) $result['LensType'] = "Unknown Lens or Lens with Teleconverter";
 			$result['LongFocalLength']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//23
				$result['LongFocalLength'] .= " focal units";
			$result['ShortFocalLength']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//24
				$result['ShortFocalLength'] .= " focal units";
			$result['FocalUnits']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//25
				$result['FocalUnits'] .= " per mm";
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//26
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//27
			$result['FlashActivity']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//28
			switch($result['FlashActivity']) {
				case 0: $result['FlashActivity'] = "Flash Did Not Fire"; break;
				case 1: $result['FlashActivity'] = "Flash Fired"; break;
				default: $result['FlashActivity'] = "Unknown";
			}
			$result['FlashDetails']=str_pad(base_convert(intel2Moto(substr($data,$place,4)), 16, 2), 16, "0", STR_PAD_LEFT);$place+=4;//29
				$flashDetails = array();
				if (substr($result['FlashDetails'], 1, 1) == 1) { $flashDetails[] = 'External E-TTL'; }
				if (substr($result['FlashDetails'], 2, 1) == 1) { $flashDetails[] = 'Internal Flash'; }
				if (substr($result['FlashDetails'], 4, 1) == 1) { $flashDetails[] = 'FP sync used'; }
				if (substr($result['FlashDetails'], 8, 1) == 1) { $flashDetails[] = '2nd(rear)-curtain sync used'; }
				if (substr($result['FlashDetails'], 12, 1) == 1) { $flashDetails[] = '1st curtain sync'; }
				$result['FlashDetails']=implode(",", $flashDetails);
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//30
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//31
			$anotherFocusMode=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//32
			if(strpos(strtoupper($exif['IFD0']['Model']), "G1") !== false) {
				switch($anotherFocusMode) {
					case 0: $result['FocusMode'] = "Single"; break;
					case 1: $result['FocusMode'] = "Continuous"; break;
					default: $result['FocusMode'] = "Unknown";
				}
			}
			
		} else if($tag=="0004") { //second chunk
			$result['Bytes']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//0
			if ($result['Bytes'] != strlen($data) / 2) return $result; //Bad chunk
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//1
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//2
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//3
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//4
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//5
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//6
			$result['WhiteBalance']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//7
			switch($result['WhiteBalance']) {
				case 0: $result['WhiteBalance'] = "Auto"; break;
				case 1: $result['WhiteBalance'] = "Sunny"; break;
				case 2: $result['WhiteBalance'] = "Cloudy"; break;
				case 3: $result['WhiteBalance'] = "Tungsten"; break;
				case 4: $result['WhiteBalance'] = "Fluorescent"; break;
				case 5: $result['WhiteBalance'] = "Flash"; break;
				case 6: $result['WhiteBalance'] = "Custom"; break;
				default: $result['WhiteBalance'] = "Unknown";
			}
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//8
			$result['SequenceNumber']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//9
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//10
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//11
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//12
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//13
			$result['AFPointUsed']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//14
				$afPointUsed = array();
				if ($result['AFPointUsed'] & 0x0001) $afPointUsed[] = "Right"; //bit 0
				if ($result['AFPointUsed'] & 0x0002) $afPointUsed[] = "Center"; //bit 1
				if ($result['AFPointUsed'] & 0x0004) $afPointUsed[] = "Left"; //bit 2
				if ($result['AFPointUsed'] & 0x0800) $afPointUsed[] = "12"; //bit 12
				if ($result['AFPointUsed'] & 0x1000) $afPointUsed[] = "13"; //bit 13
				if ($result['AFPointUsed'] & 0x2000) $afPointUsed[] = "14"; //bit 14
				if ($result['AFPointUsed'] & 0x4000) $afPointUsed[] = "15"; //bit 15
				$result['AFPointUsed'] = implode(",", $afPointUsed);
			$result['FlashBias']=intel2Moto(substr($data,$place,4));$place+=4;//15
			switch($result['FlashBias']) {
				case 'ffc0': $result['FlashBias'] = "-2 EV"; break;
				case 'ffcc': $result['FlashBias'] = "-1.67 EV"; break;
				case 'ffd0': $result['FlashBias'] = "-1.5 EV"; break;
				case 'ffd4': $result['FlashBias'] = "-1.33 EV"; break;
				case 'ffe0': $result['FlashBias'] = "-1 EV"; break;
				case 'ffec': $result['FlashBias'] = "-0.67 EV"; break;
				case 'fff0': $result['FlashBias'] = "-0.5 EV"; break;
				case 'fff4': $result['FlashBias'] = "-0.33 EV"; break;
				case '0000': $result['FlashBias'] = "0 EV"; break;
				case '000c': $result['FlashBias'] = "0.33 EV"; break;
				case '0010': $result['FlashBias'] = "0.5 EV"; break;
				case '0014': $result['FlashBias'] = "0.67 EV"; break;
				case '0020': $result['FlashBias'] = "1 EV"; break;
				case '002c': $result['FlashBias'] = "1.33 EV"; break;
				case '0030': $result['FlashBias'] = "1.5 EV"; break;
				case '0034': $result['FlashBias'] = "1.67 EV"; break;
				case '0040': $result['FlashBias'] = "2 EV"; break;
				default: $result['FlashBias'] = "Unknown";
			}
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//16
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//17
			$result['Unknown']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//18
			$result['SubjectDistance']=hexdec(intel2Moto(substr($data,$place,4)));$place+=4;//19
				$result['SubjectDistance'] .= "/100 m";
			
		} else if($tag=="0008") { //image number
			if($intel==1) $data = intel2Moto($data);
			$data=hexdec($data);
			$result = round($data/10000)."-".$data%10000;
		} else if($tag=="000c") { //camera serial number
			if($intel==1) $data = intel2Moto($data);
			$data=hexdec($data);
			$result = "#".bin2hex(substr($data,0,16)).substr($data,16,16);
		}
		
	} else if($type=="UNDEFINED") {
		
	
		
	} else {
		$data = bin2hex($data);
		if($intel==1) $data = intel2Moto($data);
	}
	
	return $data;
}



//=================
// Cannon Special data section
// Useful:  http://www.burren.cx/david/canon.html
// http://www.burren.cx/david/canon.html
// http://www.ozhiker.com/electronics/pjmt/jpeg_info/canon_mn.html
//====================================================================
function parseCanon($block,&$result,$seek, $globalOffset) {	
	$place = 0; //current place
		
	if($result['Endien']=="Intel") $intel=1;
	else $intel=0;
	
	$model = $result['IFD0']['Model'];
	
		//Get number of tags (2 bytes)
	$num = bin2hex(substr($block,$place,2));$place+=2;
	if($intel==1) $num = intel2Moto($num);
	$result['SubIFD']['MakerNote']['MakerNoteNumTags'] = hexdec($num);
	
	//loop thru all tags  Each field is 12 bytes
	for($i=0;$i<hexdec($num);$i++) {
		
			//2 byte tag
		$tag = bin2hex(substr($block,$place,2));$place+=2;
		if($intel==1) $tag = intel2Moto($tag);
		$tag_name = lookup_Canon_tag($tag);
		
			//2 byte type
		$type = bin2hex(substr($block,$place,2));$place+=2;
		if($intel==1) $type = intel2Moto($type);
		lookup_type($type,$size);
		
			//4 byte count of number of data units
		$count = bin2hex(substr($block,$place,4));$place+=4;
		if($intel==1) $count = intel2Moto($count);
		$bytesofdata = $size*hexdec($count);
	
		if($bytesofdata<=0) {
			return; //if this value is 0 or less then we have read all the tags we can
		}
		
			//4 byte value of data or pointer to data
		$value = substr($block,$place,4);$place+=4;
		
		if($bytesofdata<=4) {
			$data = $value;
		} else {
			$value = bin2hex($value);
			if($intel==1) $value = intel2Moto($value);
			$v = fseek($seek,$globalOffset+hexdec($value));  //offsets are from TIFF header which is 12 bytes from the start of the file
			if($v==0 && $bytesofdata < $GLOBALS['exiferFileSize']) {
				$data = fread($seek, $bytesofdata);
			} else if($v==-1) {
				$result['Errors'] = $result['Errors']++;
			}
		}
		$formated_data = formatCanonData($type,$tag,$intel,$data,$result,$result['SubIFD']['MakerNote'][$tag_name]);
		
		if($result['VerboseOutput']==1) {
			//$result['SubIFD']['MakerNote'][$tag_name] = $formated_data;
			if($type=="URATIONAL" || $type=="SRATIONAL" || $type=="USHORT" || $type=="SSHORT" || $type=="ULONG" || $type=="SLONG" || $type=="FLOAT" || $type=="DOUBLE") {
				$data = bin2hex($data);
				if($intel==1) $data = intel2Moto($data);
			}
			$result['SubIFD']['MakerNote'][$tag_name."_Verbose"]['RawData'] = $data;
			$result['SubIFD']['MakerNote'][$tag_name."_Verbose"]['Type'] = $type;
			$result['SubIFD']['MakerNote'][$tag_name."_Verbose"]['Bytes'] = $bytesofdata;
		} else {
			//$result['SubIFD']['MakerNote'][$tag_name] = $formated_data;
		}
	}
}


?>
