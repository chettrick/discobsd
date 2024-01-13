#!/bin/sh -
#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#

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

echo $GITREV $CV ${USER-root} `hostname` ${PWD#$GITDIR}| \
awk ' {
    version = $1;
    cv = $2;
    user = $3;
    host = $4;
    dir = $5;
    date = strftime();
    printf "const char version[] = \"2.11 BSD UNIX for PIC32, rev G%s #%d: %s\\n", version, cv, date;
    printf "     %s@%s:%s\\n\";\n", user, host, dir;
}'
