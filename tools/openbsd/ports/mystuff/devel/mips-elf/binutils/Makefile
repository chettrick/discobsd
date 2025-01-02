COMMENT=	binutils for ${CONFIG} cross-development

VERSION=	2.40
DISTNAME=	binutils-${VERSION}

HOMEPAGE=	https://www.gnu.org/software/binutils/

WANTLIB=	c

SITES=		${SITE_GNU:=binutils/}
DIST_SUBDIR=	binutils

USE_GMAKE=	Yes
USE_LIBTOOL=	No

CONFIGURE_ARGS+=--enable-commonbfdlib=no	\
		--enable-interwork		\
		--enable-multilib		\
		--without-zstd

SUBST_VARS+=	CONFIG

.include <bsd.port.mk>
