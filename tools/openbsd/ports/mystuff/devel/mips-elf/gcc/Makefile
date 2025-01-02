COMMENT=	gcc for ${CONFIG} cross-development

VERSION=	12.2.0
DISTNAME=	gcc-${VERSION}
PKGNAME=	${CONFIG}-gcc-${VERSION}

SHARED_LIBS+=	lto_plugin                0.0 # 0.0

SUBST_VARS+=	VERSION

WANTLIB=	${COMPILER_LIBCXX} c gmp m mpc mpfr pthread
DIST_SUBDIR=	gcc

COMPILER=	base-clang ports-gcc base-gcc

MAKE_GXX=	No
MAKE_FORTRAN=	No
MAKE_OBJC=	No
MAKE_JAVA=	No
MAKE_ADA=	No

SITES=		${SITE_GCC:=releases/gcc-${VERSION}/}

BUILD_DEPENDS+=	${RUN_DEPENDS} \
		devel/bison

LIB_DEPENDS=	devel/mpfr \
		devel/libmpc

RUN_DEPENDS=	devel/${CONFIG}/binutils

#LANGS=		c,c++
LANGS=		c
CFLAGS=		-O2 -g

CONFIGURE_ARGS+=--enable-languages=${LANGS} \
		--with-arch-32=mips32r2 \
		--with-gmp="${LOCALBASE}" \
		--with-mpc="${LOCALBASE}" \
		--with-mpfr="${LOCALBASE}" \
		--without-isl \
		--without-zstd \
		--disable-libssp \
		--with-newlib \
		--disable-libstdc___v3 \
		--disable-libcc1 \
		--disable-nls \
		--disable-shared \
		--disable-decimal-float \
		--disable-threads \
		--disable-libmudflap \
		--disable-libgomp \
		--disable-libquadmath \
		--enable-multilib \
		--enable-cpp \
		--enable-lto
CONFIGURE_ENV=	CPPFLAGS="${CPPFLAGS} -I${LOCALBASE}/include" \
		LDFLAGS="-L${LOCALBASE}/lib"

# Avoid build failure when lang/gcc/8 is installed whose ansidecl.h misses these
CPPFLAGS+=	-DATTRIBUTE_NONSTRING= -DATTRIBUTE_RESULT_SIZE_1=
CPPFLAGS+=	-DATTRIBUTE_RESULT_SIZE_2= -DATTRIBUTE_RESULT_SIZE_1_2=
CPPFLAGS+=	-DATTRIBUTE_WARN_UNUSED_RESULT=

SEPARATE_BUILD=	Yes
USE_GMAKE=	Yes
YACC=		bison

post-install:
	chown -R ${SHAREOWN}:${SHAREGRP} \
		${PREFIX}/lib/gcc/${CONFIG}/${VERSION}/

.include <bsd.port.mk>
