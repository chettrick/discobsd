# $OpenBSD: Makefile.template,v 1.71 2014/07/10 12:33:17 sthen Exp $

COMMENT=		programmer for ST Microelectronics ST-Link V1/V2

V=			1.1.0
DISTNAME=		stlink-${V}

CATEGORIES=		devel

HOMEPAGE=		https://github.com/texane/stlink

# BSD
PERMIT_PACKAGE=		Yes

WANTLIB=		c pthread usb-1.0

GH_ACCOUNT=		texane
GH_PROJECT=		stlink
GH_TAGNAME=		v${V}

LIB_DEPENDS=		devel/libusb1

CONFIGURE_STYLE=	gnu

AUTOCONF_VERSION=	2.69
AUTOMAKE_VERSION=	1.15
BUILD_DEPENDS+=		${MODGNU_AUTOCONF_DEPENDS} \
			${MODGNU_AUTOMAKE_DEPENDS}

NO_TEST=		Yes

pre-configure:
	cd ${WRKSRC} && env AUTOCONF_VERSION=${AUTOCONF_VERSION} \
		AUTOMAKE_VERSION=${AUTOMAKE_VERSION} /bin/sh ./autogen.sh

post-install:
	${INSTALL_DATA_DIR} ${PREFIX}/share/doc/stlink
	${INSTALL_DATA} ${WRKDIST}/doc/tutorial/tutorial.pdf \
		${WRKDIST}/README ${PREFIX}/share/doc/stlink

.include <bsd.port.mk>
