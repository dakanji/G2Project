#!/bin/sh

if [ ! -L .git/hooks ];
then
	if [ -x .git/hooks ];
	then
		echo "copying '.git/hooks' to '.git/old_hooks'"
		mv .git/hooks .git/old_hooks
	fi

	echo "symlinking '.git/hooks' to '/dev_scripts/hooks'"
	ln -s ../dev_scripts/hooks .git/hooks
fi
