ONLY_FOR_ARCHS=	amd64 i386 powerpc sparc64

USE_WXNEEDED=	Yes

COMMENT=	multi system emulator, Arm Cortex-M version

DISTNAME=	qemu-arm-2.8.0.6.20190517
REVISION=	0

CATEGORIES=	emulators

HOMEPAGE=	https://github.com/ilg-archived/qemu

# GPLv2, LGPLv2 and BSD
PERMIT_PACKAGE=	Yes

GH_ACCOUNT=	ilg-archived
GH_PROJECT=	qemu
GH_TAGNAME=	v2.8.0-6-20190517

WANTLIB=	SDL2 SDL2_image X11 c drm epoxy fdt \
		gbm glib-2.0 gthread-2.0 intl iscsi m \
		atk-1.0 cairo cairo-gobject gdk-3 \
		gdk_pixbuf-2.0 gio-2.0 gobject-2.0 gtk-3 \
		harfbuzz pango-1.0 pangocairo-1.0 vte-2.91 \
		pixman-1 util z ${COMPILER_LIBCXX}

# Using TLS emulation layer
COMPILER=	base-clang ports-gcc

MODULES=	lang/python
MODPY_VERSION=	${MODPY_DEFAULT_VERSION_2}

BUILD_DEPENDS=	devel/gettext,-tools \
		textproc/py-sphinx${MODPY_FLAVOR} \
		textproc/texi2html

LIB_DEPENDS=	devel/dtc \
		devel/glib2 \
		devel/libiscsi \
		devel/pango \
		devel/sdl2 \
		devel/sdl2-image \
		devel/vte3 \
		graphics/cairo

MODPY_RUNDEP=	No

MAKE_ENV=	V=1
FAKE_FLAGS=	qemu_confdir=${PREFIX}/share/examples/qemu

EXTRA_CFLAGS=	-I${LOCALBASE}/include -I${X11BASE}/include
EXTRA_LDFLAGS=	-L${LOCALBASE}/lib -L${X11BASE}/lib

# fix build with ancient ncurses. ncurses needs to be updated to 5.9 or newer.
EXTRA_CFLAGS+=	-D_XOPEN_SOURCE_EXTENDED

# until the system headers are fixed properly.
EXTRA_CFLAGS+=	-Wno-redundant-decls

SEPARATE_BUILD=	Yes

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
