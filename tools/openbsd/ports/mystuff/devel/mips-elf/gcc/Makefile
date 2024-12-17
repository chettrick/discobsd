COMMENT=	gcc for ${CONFIG} cross-development

VERSION=	4.8.1
DISTNAME=	gcc-${VERSION}
PKGNAME=	${CONFIG}-gcc-${VERSION}
REVISION=	0
SHARED_LIBS +=  lto_plugin                0.0 # 0.0

SUBST_VARS+=	VERSION

WANTLIB=	${COMPILER_LIBCXX} c gmp m mpc mpfr
DIST_SUBDIR=	gcc

COMPILER=	base-clang ports-gcc base-gcc

MAKE_GXX=	No
MAKE_FORTRAN=	No
MAKE_OBJC=	No
MAKE_JAVA=	No
MAKE_ADA=	No

SITES=		${SITE_GCC:=releases/gcc-$(VERSION)/}

BUILD_DEPENDS+=	devel/autoconf/2.13 \
		devel/bison \
		devel/metaauto \
		devel/${CONFIG}/binutils

LIB_DEPENDS=	devel/mpfr \
		devel/libmpc

RUN_DEPENDS=	devel/${CONFIG}/binutils

#LANGS=		c,c++
LANGS=		c
CFLAGS=		-O2 -g

CXXFLAGS_base-clang=	-std=c++14

CONFIGURE_ARGS+=--enable-languages=${LANGS} \
		--with-arch-32=mips32r2 \
		--with-gmp="${LOCALBASE}" \
		--with-mpc="${LOCALBASE}" \
		--with-mpfr="${LOCALBASE}" \
		--disable-libssp \
# XXX		--with-newlib \
# XXX		--disable-libstdcxx-pch \
		--disable-libstdc___v3 \
# XXX		--disable-libssp \
# XXX		--disable-libstdcxx \
# XXX		--disable-libcc1 \
# XXX		--disable-lto \
		--disable-nls \
		--disable-shared \
		--disable-decimal-float \
		--disable-threads \
		--disable-libmudflap \
		--disable-libgomp \
		--disable-libquadmath \
		--enable-lto
CONFIGURE_ENV=	CPPFLAGS="-I${LOCALBASE}/include" \
		LDFLAGS="-L${LOCALBASE}/lib"

SEPARATE_BUILD=	Yes
USE_GMAKE=	Yes
YACC=		bison

TEST_DEPENDS=	devel/dejagnu

# XXX - Needs testing before using. If works, should replace all the patches.
# Fix build with clang in c++11 mode
#post-patch:
#        find ${WRKSRC} -type f \( -name '*.[ch]' -o -name '*.cc' \) | \
#                xargs egrep -l '"(HOST_WIDE|PLUGIN_COND|PRId64)' | \
#                xargs sed -i -E -e 's/"(HOST_WIDE|PLUGIN_COND|PRId64)/" \1/g'

# XXX
post-install:
	mv -f ${PREFIX}/libexec/gcc/${CONFIG}/${VERSION}/liblto_plugin.so.* \
		${PREFIX}/libexec/gcc/${CONFIG}/${VERSION}/liblto_plugin.so
	chown -R ${SHAREOWN}:${SHAREGRP} \
		${PREFIX}/lib/gcc/${CONFIG}/${VERSION}/

.include <bsd.port.mk>
