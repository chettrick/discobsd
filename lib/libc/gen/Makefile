#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= ${DEFS} -Os -Wno-attributes -Wno-attribute-alias

SRCS		= ${STDSRC}
OBJS		= ${STDOBJ}

STDSRC		= abort.c alarm.c atof.c atoi.c atol.c bcmp.c bcopy.c \
		  bzero.c calloc.c closedir.c crypt.c ctime.c ctype_.c \
		  daemon.c devname.c ecvt.c err.c execvp.c fakcu.c ffs.c \
		  frexp.c fstab.c gcvt.c getenv.c getgrent.c getgrgid.c \
		  getgrnam.c getgrouplist.c gethostname.c getloadavg.c \
		  getlogin.c getmntinfo.c getpagesize.c getpass.c \
		  getpwent.c getttyent.c getttynam.c getusershell.c \
		  getwd.c index.c initgroups.c isatty.c isinff.c isnanf.c \
		  knlist.c ldexp.c malloc.c mktemp.c modff.c ndbm.c \
		  nlist.c opendir.c perror.c popen.c psignal.c qsort.c \
		  random.c readdir.c regex.c rindex.c scandir.c seekdir.c \
		  setenv.c sethostname.c setmode.c siginterrupt.c \
		  siglist.c signal.c sigsetops.c sleep.c strcasecmp.c \
		  strcat.c strcmp.c strcpy.c strdup.c strftime.c strlen.c \
		  strncat.c strncmp.c strncpy.c swab.c sysctl.c syslog.c \
		  system.c telldir.c time.c timezone.c ttyname.c ttyslot.c \
		  ualarm.c uname.c usleep.c wait.c wait3.c waitpid.c

STDOBJ		= abort.o alarm.o atof.o atoi.o atol.o bcmp.o bcopy.o \
		  bzero.o calloc.o closedir.o crypt.o ctime.o ctype_.o \
		  daemon.o devname.o ecvt.o err.o execvp.o fakcu.o ffs.o \
		  frexp.o fstab.o gcvt.o getenv.o getgrent.o getgrgid.o \
		  getgrnam.o getgrouplist.o gethostname.o getloadavg.o \
		  getlogin.o getmntinfo.o getpagesize.o getpass.o \
		  getpwent.o getttyent.o getttynam.o getusershell.o \
		  getwd.o index.o initgroups.o isatty.o isinff.o isnanf.o \
		  knlist.o ldexp.o malloc.o mktemp.o modff.o ndbm.o \
		  nlist.o opendir.o perror.o popen.o psignal.o qsort.o \
		  random.o readdir.o regex.o rindex.o scandir.o seekdir.o \
		  setenv.o sethostname.o setmode.o siginterrupt.o \
		  siglist.o signal.o sigsetops.o sleep.o strcasecmp.o \
		  strcat.o strcmp.o strcpy.o strdup.o strftime.o strlen.o \
		  strncat.o strncmp.o strncpy.o swab.o sysctl.o syslog.o \
		  system.o telldir.o time.o timezone.o ttyname.o ttyslot.o \
		  ualarm.o uname.o usleep.o wait.o wait3.o waitpid.o

gen.a:		${OBJS}
		@echo "building gen.a"
		@${AR} cr gen.a ${OBJS}

tags:
		cwd=`pwd`; \
		for i in ${SRCS}; do \
			ctags -a -f ${TAGSFILE} $$cwd/$$i; \
		done

clean:
		rm -f gen.a *.o *~ profiled/*.o tags Makefile.bak

depend:
		for i in ${SRCS}; do \
		    cc -M ${CFLAGS} $$i | awk ' { if ($$1 != prev) \
			{ if (rec != "") print rec; rec = $$0; prev = $$1; } \
			else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
			else rec = rec " " $$2 } } \
			END { print rec } ' >> makedep; done
		echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile Makefile.bak
		ed - Makefile < eddep
		rm eddep makedep
		echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
		echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
		echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
