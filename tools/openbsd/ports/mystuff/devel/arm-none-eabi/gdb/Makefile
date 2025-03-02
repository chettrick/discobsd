COMMENT=	gdb for arm-none-eabi cross-development

DISTNAME=	gdb-12.1

HOMEPAGE=	https://www.gnu.org/software/gdb/

WANTLIB+=	${COMPILER_LIBCXX}
WANTLIB+=	c curses expat m pthread

SITES=		${SITE_GNU:=gdb/}
DIST_SUBDIR=	gdb

USE_GMAKE=	Yes

MODGNU_CONFIG_GUESS_DIRS=	${WRKSRC} ${WRKSRC}/gdb

# C++11
COMPILER=	base-clang ports-gcc

SEPARATE_BUILD=	Yes

CONFIGURE_ARGS+=				\
		--disable-install-libbfd	\
		--disable-sim			\
		--enable-interwork		\
		--enable-multilib		\
		--enable-thumb			\
		--with-system-zlib		\
		--without-guile			\
		--without-isl			\
		--without-lzma			\
		--without-mpc			\
		--without-mpfr			\
		--without-python

MAKE_FLAGS=	V=1 LDFLAGS="-L${LOCALBASE}/lib ${LDFLAGS}"
# Avoid using malloc replacement for no good reason
MAKE_ENV=	gl_cv_malloc_ptrdiff=yes

MODULES+=	lang/python

TEST_DEPENDS=	devel/dejagnu

.include <bsd.port.mk>
