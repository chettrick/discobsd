COMMENT=		toolset for ST Microelectronics ST-Link V1/V2/V2-1/V3

V=			1.8.0
DISTNAME=		stlink-${V}

SHARED_LIBS+=		stlink			0.0	# 0.0

CATEGORIES=		devel

HOMEPAGE=		https://github.com/stlink-org/stlink

# BSD
PERMIT_PACKAGE=		Yes

WANTLIB=		c usb-1.0

GH_ACCOUNT=		stlink-org
GH_PROJECT=		stlink
GH_TAGNAME=		v${V}

LIB_DEPENDS=		devel/libusb1

MODULES=		devel/cmake

.include <bsd.port.mk>
