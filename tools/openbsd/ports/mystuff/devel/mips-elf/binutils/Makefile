# $OpenBSD: Makefile,v 1.1.1.1 2017/11/02 18:30:57 bentley Exp $

COMMENT=	binutils for ${CONFIG} cross-development

VERSION=	2.27
DISTNAME=	binutils-${VERSION}

HOMEPAGE=	https://www.gnu.org/software/binutils/

# XXX WANTLIB=	c
WANTLIB=	c z

MASTER_SITES=	${MASTER_SITE_GNU:=binutils/}
DIST_SUBDIR=	binutils

USE_LIBTOOL=	No

CONFIGURE_ARGS+=--enable-commonbfdlib=no \
		--enable-interwork	 \
		--enable-multilib

SUBST_VARS+=	CONFIG

.include <bsd.port.mk>
