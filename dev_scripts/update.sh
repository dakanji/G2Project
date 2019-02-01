#!/bin/sh
echo ""
echo ""
echo "Developer Environment Update Started"



# Update WordPress ArrayDeclarationSpacingSniff if required
WP_ArrayDeclarationSpacingSniff=dev_vendor/wp-coding-standards/wpcs/WordPress/Sniffs/Arrays/ArrayDeclarationSpacingSniff.php
WP_AA_ArrayDeclarationSpacingSniff=dev_scripts/overrides/wpcs-ArrayDeclarationSpacingSniff.php
WP_XX_ArrayDeclarationSpacingSniff=dev_vendor/wp-coding-standards/wpcs/WordPress/Sniffs/Arrays/updated
if [ -e $WP_ArrayDeclarationSpacingSniff ] && [ -e $WP_AA_ArrayDeclarationSpacingSniff ] && [ ! -e $WP_XX_ArrayDeclarationSpacingSniff ];
then
		echo "Overriding WordPress ArrayDeclarationSpacingSniff CS"
		rm -f $WP_ArrayDeclarationSpacingSniff
		cp $WP_AA_ArrayDeclarationSpacingSniff $WP_ArrayDeclarationSpacingSniff
		touch $WP_XX_ArrayDeclarationSpacingSniff
fi



echo "Developer Environment Update Ended"
echo ""
echo ""
