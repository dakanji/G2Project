#
# Gallery .mo files makefile
#
# This makefile is based on the one provided with the Horde project
# http://www.horde.org/.  As such, it inherits the license from the
# original version.  You can find that license here:
#
# http://cvs.horde.org/co.php/horde/COPYING?r=2.1
#
# I'm not exactly sure what the license restrictions are in this case,
# but I want to give full credit to the original authors:
#
# Copyright 2000-2002 Joris Braakman <jbraakman@yahoo.com>
# Copyright 2001-2002 Chuck Hagenbuch <chuck@horde.org>
# Copyright 2001-2002 Jan Schneider <jan@horde.org>
#
# $Id: GNUmakefile.inc 16027 2007-03-16 18:55:07Z mindless $
#
MODULE_DIR = $(shell dirname `pwd`)
TYPE_DIR = $(shell dirname $(MODULE_DIR))
APPLICATION ?= $(shell basename $(TYPE_DIR))_$(shell basename $(MODULE_DIR))

ifdef QUIET
    MSGFMT_FLAGS =
else
    MSGFMT_FLAGS = -v
endif

MSGFMT = msgfmt $(MSGFMT_FLAGS) -c -o
MSGFMTSOL = msgfmt $(MSGFMT_FLAGS) -o

PO ?= *
LANGPO = $(shell echo $@ | perl -ne 'chomp; print if (s/^(..)_..\.po/$$1.po/ and -s)')
COMPENDIUM = $(shell echo $@ | perl -ne 'chomp; print "-C $$_" if (s/^(..)_..\.po/$$1.po/ and -s)')

all: install

all-remove-obsolete: update remove-obsolete install

messages.po: alwaysbuild
	@php -f $(TOOLDIR)/po/extract.php $(MODULE_DIR) > strings.raw.tmp
	@# Don't extract right to strings.raw because extract.php tries to preserve
	@# some old data from strings.raw and the > redirect blanks it out.
	@mv strings.raw.tmp strings.raw
	@xgettext --keyword=_ -C --no-location \
		  --msgid-bugs-address=gallery-translations@lists.sourceforge.net strings.raw
	@sed '1,4 d' < messages.po > messages.tmp
	@cat $(TOOLDIR)/po/header.txt messages.tmp > messages.po
	@$(RM) messages.tmp

%.po: messages.po
	@perl $(TOOLDIR)/po/premerge-messages.pl $@ messages.po > messages.tmp
	@# ignore errors from msgmerge, we'll catch them on the msgfmt command:
	@-if [ -s messages.tmp ]; then \
	    msgmerge --quiet --width=95 $(COMPENDIUM) $@ messages.tmp > $@.tmp; \
	    if [ -s $@.tmp ]; then mv $@.tmp $@; else $(RM) $@.tmp; fi; \
	    perl -pi $(TOOLDIR)/po/header.pl $@; \
	  elif [ -z "$(NOCREATE)" ]; then \
	    if [ -n "$(LANGPO)" ]; then \
	      perl $(TOOLDIR)/po/premerge-messages.pl -2 messages.po $(LANGPO) > $@; \
	    else cp messages.po $@; fi; \
	    perl -pi $(TOOLDIR)/po/header.pl $@; \
	  fi
	@$(RM) messages.tmp

alwaysbuild:

clean:
	@$(RM) messages.po

update: $(PO).po

install: update
	@if test "$(OSTYPE)" = "solaris"; then \
	    echo "You'll probably get some warnings on Solaris. This is normal."; \
	fi; \
	FAIL=0; \
	echo "Updating $(PO)"; \
	for LOCALE in `ls $(PO).po | grep -v messages.po | sed 's/\.[^.]*$$//g'`; do \
	    if test -z "$${QUIET}"; then echo $${LOCALE}; fi; \
	    if $(TOOLDIR)/po/shtool mkdir -p ../locale/$${LOCALE}/LC_MESSAGES; then \
		php -f $(TOOLDIR)/po/trim-po.php `pwd`/$${LOCALE}.po > tmp.po; \
		if test "$(OSTYPE)" = "solaris"; then \
		    if {MSGFMTSOL} ../locale/$${LOCALE}/LC_MESSAGES/$(APPLICATION).mo tmp.po; then \
			if test -n "$${QUIET}"; then \
			    echo -n '.'; \
			fi \
		    else \
			echo -n "Fail: $${LOCALE}"; \
			FAIL=1; \
		    fi \
		else \
		    if $(MSGFMT) ../locale/$${LOCALE}/LC_MESSAGES/$(APPLICATION).mo tmp.po; then \
			if test -n "$${QUIET}"; then \
			    echo -n '.'; \
			fi \
		    else \
			echo -n "Fail: $${LOCALE}"; \
			FAIL=1; \
		    fi \
		fi; \
		$(RM) tmp.po; \
	    else \
		echo "Could not create locale directory for $${LOCALE}."; \
		FAIL=1; \
	    fi; \
	    if test -z "$${QUIET}"; then echo; fi; \
	done; \
	if test $${FAIL} -gt 0; then exit 1; fi;
	@echo '';

compendium: messages.po
	@perl -pi -e 's|^("Content-Type: text/plain; charset)=CHARSET|$$1=UTF-8|' messages.po
	@for POFILE in `ls $(PO).po | grep -v messages.po`; do \
	    CORE=`perl -e '$$_="$(TOOLDIR)/../../modules/core/po/'$${POFILE}'"; \
			   print "-C $$_" if -s'`; \
	    MATRIX=`perl -e '$$_="$(TOOLDIR)/../../themes/matrix/po/'$${POFILE}'"; \
			     print "-C $$_" if -s'`; \
	    if [ -n "$${CORE}" -o -n "$${MATRIX}" ]; then \
		echo -n "$${POFILE} "; \
		msgmerge --width=95 -N $${CORE} $${MATRIX} \
			 --suffix= --update $${POFILE} messages.po; \
		if [ `svn diff $${POFILE} | grep -c '^+msgstr'` -eq 0 ]; then \
		    svn revert --quiet $${POFILE}; \
		fi; \
	    else \
		echo "No compendium for $${POFILE}"; \
	    fi; \
	done;

create-from-core:
	@for POFILE in `ls $(TOOLDIR)/../../modules/core/po/$(PO).po | grep -v messages.po`; do \
	    PO=`basename $${POFILE}`; \
	    if [ ! -f $${PO} ]; then \
		perl -p -e '$$x++ if /^msgid/; exit if $$x eq 2' $${POFILE} > $${PO}; \
	    fi; \
	done;

remove-obsolete:
	@for LOCALE in `ls $(PO).po | grep -v messages.po | sed 's/\.[^.]*$$//g'`; do \
		msgattrib --no-obsolete --output-file=$${LOCALE}.po $${LOCALE}; \
	done;
