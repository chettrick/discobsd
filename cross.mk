# Override the default port with:
# $ gmake MACHINE=pic32 MACHINE_ARCH=mips
#
MACHINE=	stm32
MACHINE_ARCH=	arm

DESTDIR		= /usr/local/retrobsd

UNAME_S		= $(shell uname -s)

CC		= gcc -m32

AS		= $(CC) -x assembler-with-cpp
LD		= ld
AR		= ar
RANLIB		= ranlib

ifeq ($(UNAME_S), Linux)
YACC            = byacc
else
YACC            = yacc
endif

LEX             = flex
SIZE		= size
#OBJDUMP		= objdump
OBJDUMP		= sync --
INSTALL		= install -m 644
INSTALLDIR	= install -m 755 -d
TAGSFILE	= tags
MANROFF		= nroff -man -h
ELF2AOUT	= cp

CFLAGS		= -O -DCROSS
LDFLAGS		=
LIBS		=

# Add system include path
ifeq (,$(wildcard /usr/include/i386-linux-gnu))
    CFLAGS      += -I/usr/include
else
    CFLAGS      += -I/usr/include/i386-linux-gnu
endif
CFLAGS		+= -I$(TOPSRC)/include
