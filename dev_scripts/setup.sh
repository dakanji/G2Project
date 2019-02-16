#!/bin/sh

# Get key paths
ABSPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
HOOKSPATH=$ABSPATH/hooks
MAINPATH=$ABSPATH/..

if [ ! -L $MAINPATH/.git/hooks ]; then
	echo ""
	if [ -x $MAINPATH/.git/hooks ]; then
		echo "Copying '.git/hooks' to '.git/old_hooks'"
		mv $MAINPATH/.git/hooks $MAINPATH/.git/old_hooks
	fi

	echo "Symlinking '.git/hooks' to 'dev_scripts/hooks'"
	ln -s $HOOKSPATH $MAINPATH/.git/hooks
else
	if [ ! -d "$MAINPATH/dev_vendor/php-cs-fixer" ]; then
		echo ""
		echo "Removing symlink from '.git/hooks' to 'dev_scripts/hooks'"
		rm -f $MAINPATH/.git/hooks

		if [ -d "$MAINPATH/.git/old_hooks" ]; then
			echo "Restoring orignal '.git/hooks' folder"
			mv $MAINPATH/.git/old_hooks $MAINPATH/.git/hooks
		fi
		echo ""
	fi
fi

############################################
# Code Below Updates Developer Environment #
############################################
if [ -d "$MAINPATH/dev_vendor/php-cs-fixer" ]; then
echo ""
echo ""
echo "Developer Environment Update Started"



	# Update WordPress ArrayDeclarationSpacingSniff if required
	WP_AA_ArrSniff=$MAINPATH/dev_vendor/wp-coding-standards/wpcs/WordPress/Sniffs/Arrays/ArrayDeclarationSpacingSniff.php
	WP_BB_ArrSniff=$MAINPATH/dev_scripts/overrides/wpcs-ArrayDeclarationSpacingSniff.php
	WP_CC_ArrSniff=$MAINPATH/dev_vendor/wp-coding-standards/wpcs/WordPress/Sniffs/Arrays/updated
	if [ -e $WP_AA_ArrSniff ] && [ -e $WP_BB_ArrSniff ] && [ ! -e $WP_CC_ArrSniff ];
	then
		echo "Overriding WordPress ArrayDeclarationSpacingSniff CS"
		rm -f $WP_AA_ArrSniff
		cp $WP_BB_ArrSniff $WP_AA_ArrSniff
		touch $WP_CC_ArrSniff
	fi




echo "Developer Environment Update Ended"
echo ""
echo ""
fi
