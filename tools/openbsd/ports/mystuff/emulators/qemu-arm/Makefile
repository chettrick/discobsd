# $OpenBSD: Makefile,v 1.190 2019/08/18 06:56:07 ajacoutot Exp $

ONLY_FOR_ARCHS=	amd64 i386 powerpc sparc64

USE_WXNEEDED=	Yes

COMMENT=	multi system emulator, Arm Cortex-M version

DISTNAME=	qemu-arm-2.8.0.6.20190517
CATEGORIES=	emulators

HOMEPAGE=	https://github.com/ilg-archived/qemu

# GPLv2, LGPLv2 and BSD
PERMIT_PACKAGE=	Yes

GH_ACCOUNT=	ilg-archived
GH_PROJECT=	qemu
GH_TAGNAME=	v2.8.0-6-20190517

WANTLIB=	SDL2 SDL2_image X11 c drm epoxy fdt>=4.1 \
		gbm glib-2.0 gthread-2.0 intl iscsi m \
		pixman-1 util z ${COMPILER_LIBCXX}

# Using TLS emulation layer
COMPILER=	base-clang ports-gcc

MODULES=	lang/python

BUILD_DEPENDS=	devel/gettext,-tools \
		textproc/py-sphinx \
		textproc/texi2html

LIB_DEPENDS=	devel/dtc>=1.5.0p0 \
		devel/glib2 \
		devel/libiscsi \
		devel/sdl2 \
		devel/sdl2-image

MODPY_RUNDEP=	No

MAKE_ENV=	V=1
FAKE_FLAGS=	qemu_confdir=${PREFIX}/share/examples/qemu

EXTRA_CFLAGS=	-I${LOCALBASE}/include -I${X11BASE}/include
EXTRA_LDFLAGS=	-L${LOCALBASE}/lib -L${X11BASE}/lib

# fix build with ancient ncurses. ncurses needs to be updated to 5.9 or newer.
EXTRA_CFLAGS+=	-D_XOPEN_SOURCE_EXTENDED

# until the system headers are fixed properly.
EXTRA_CFLAGS+=	-Wno-redundant-decls

USE_GMAKE=	Yes
CONFIGURE_STYLE=simple
CONFIGURE_ARGS=	--prefix=${PREFIX} \
		--sysconfdir=${SYSCONFDIR} \
		--mandir=${PREFIX}/man \
		--python=${MODPY_BIN} \
		--cc="${CC}" \
		--cxx="${CXX}" \
		--host-cc="${CC}" \
		--extra-cflags="${EXTRA_CFLAGS}" \
		--extra-ldflags="${EXTRA_LDFLAGS}" \
		--target-list="gnuarmeclipse-softmmu" \
		--with-sdlabi="2.0" \
		--disable-bsd-user \
		--disable-werror \
		--disable-bluez \
		--disable-gcrypt \
		--disable-gnutls \
		--disable-libnfs \
		--disable-libssh2 \
		--disable-linux-aio \
		--disable-lzo \
		--disable-nettle \
		--disable-seccomp \
		--disable-smartcard \
		--disable-snappy \
		--disable-spice \
		--disable-usb-redir \
		--disable-vnc-sasl

FLAVORS=	debug
FLAVOR?=

.if ${FLAVOR:Mdebug}
CFLAGS+=	-O0
CONFIGURE_ARGS+=--enable-debug
INSTALL_STRIP=
.else
CONFIGURE_ARGS+=--disable-debug-info
.endif

TEST_TARGET=	check

# Install the json scripts and board graphics.
post-install:
	${INSTALL_DATA_DIR} ${PREFIX}/share/qemu/devices
	${INSTALL_DATA} ${WRKSRC}/gnu-mcu-eclipse/devices/*.json \
	    ${PREFIX}/share/qemu/devices
	${INSTALL_DATA} ${WRKSRC}/gnu-mcu-eclipse/devices/README.md \
	    ${PREFIX}/share/qemu/devices
	${INSTALL_DATA_DIR} ${PREFIX}/share/qemu/graphics
	${INSTALL_DATA} ${WRKSRC}/gnu-mcu-eclipse/graphics/*.jpg \
	    ${PREFIX}/share/qemu/graphics

.include <bsd.port.mk>
