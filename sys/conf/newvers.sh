#!/bin/sh -
#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#

# Things which need version number upgrades:
#	sys/conf/newvers.sh:
#		OSR
#	sys/sys/param.h:
#		DiscoBSD
#		DiscoBSD_X_X
#	share/mk/sys.mk:
#		OSMAJOR
#		OSMINOR
#
# After a release, move to S="-current" and keep same version number.
# Before a release, move to S="-beta" and increment version number.
# For a release, move to S="" and keep same version number.

OST="DiscoBSD"
OSR="2.5"

S="-current"
#S="-beta"
#S=""

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

echo $GITREV $CV ${USER-root} $H $D $OST $OSR $ID $S| \
awk '{
    gitrev = $1;
    cv = $2;
    user = $3;
    host = $4;
    dir = $5;
    date = strftime();
    ost = $6;
    osr = $7;
    id = toupper($8);
    status = $9;
    printf "const char version[] = \"%s %s%s (%s) #%d %s: %s\\n", \
        ost, osr, status, id, cv, gitrev, date;
    printf "     %s@%s:%s\\n\";\n", user, host, dir;
    printf "const char ostype[] = \"%s\";\n", ost;
    printf "const char osversion[] = \"%s#%d\";\n", id, cv;
    printf "const char osrelease[] = \"%s\";\n", osr;
}'
