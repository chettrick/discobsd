CONFIG=		mips-elf
PKGNAME?=	${CONFIG}-${DISTNAME}
FULLPKGNAME?=	${PKGNAME}
CATEGORIES+=	devel

# GPLv3
# For newlib: multiple copyrights, but basically BSD.
#   See: ${PREFIX}/share/doc/newlib
PERMIT_PACKAGE=	Yes

# XXX at least gmkdir gets picked up if present
BUILD_DEPENDS +=	sysutils/coreutils

CONFIGURE_STYLE=gnu
CONFIGURE_ARGS+=--target=${CONFIG}  \
		--disable-shared    \
		--disable-nls	    \
		--disable-werror

EXTRACT_SUFX?=	.tar.xz
