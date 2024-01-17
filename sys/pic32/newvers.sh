#!/bin/sh -
#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#

# Things which need version number upgrades:
#	sys/${MACHINE}/newvers.sh:
#		OSR
#	sys/include/param.h:
#		DiscoBSD
#		DiscoBSD_X_X
#	share/mk/sys.mk:
#		OSMAJOR
#		OSMINOR

OST="DiscoBSD"
OSR="2.1"

if [ ! -r .compileversion -o ! -s .compileversion ]
then
    echo 0 >.compileversion
fi
CV=`cat .compileversion`
CV=`expr $CV + 1`

if [ ! -r .oldversion -o ! -s .oldversion ]
then
    echo 0 >.oldversion
fi
OV=`cat .oldversion`

GITREV=`git rev-list HEAD --count`
GITDIR=`git rev-parse --show-toplevel`

if [ "x$GITREV" = "x" ]
then
    GITREV="Untracked"
fi

if [ "x$GITREV" != "x$OV" ]
then
    CV=1
fi
echo $CV >.compileversion
echo $GITREV >.oldversion

H=`hostname`
D=${PWD#$GITDIR}
ID=`basename "${D}"`

echo $GITREV $CV ${USER-root} $H $D $OST $ID $OSR| \
awk ' {
    version = $1;
    cv = $2;
    user = $3;
    host = $4;
    dir = $5;
    date = strftime();
    ost = $6;
    id = $7;
    osr = $8;
    printf "const char version[] = \"2.11 BSD UNIX for PIC32, rev G%s #%d: %s\\n", version, cv, date;
    printf "     %s@%s:%s\\n\";\n", user, host, dir;
    printf "const char ostype[] = \"%s\";\n", ost;
    printf "const char osversion[] = \"%s#%d\";\n", toupper(id), cv;
    printf "const char osrelease[] = \"%s\";\n", osr;
}'
