#!/bin/sh

if [ ! -L .git/hooks ];
then
	if [ -x .git/hooks ];
	then
		echo "copying '.git/hooks' to '.git/old_hooks'"
		mv .git/hooks .git/old_hooks
	fi

	echo "symlinking '.git/hooks' to '/dev_scripts/hooks'"
	ln -s dev_scripts/hooks .git/hooks
fi

#####################################################
# Code below is to update the developer environment #
#####################################################
if [ -d "dev_vendor/php-cs-fixer" ]; then
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

	# Set strategy for merge conflicts
	echo "Setting Merge Strategy"
	git config merge.theirs.driver true



echo "Developer Environment Update Ended"
echo ""
echo ""
fi