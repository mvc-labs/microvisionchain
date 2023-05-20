ACLOCAL_AMFLAGS = -I build-aux/m4

lib_LTLIBRARIES = libsecp256k1.la
if USE_JNI
JNI_LIB = libsecp256k1_jni.la
noinst_LTLIBRARIES = $(JNI_LIB)
else
JNI_LIB =
endif
include_HEADERS = include/secp256k1.h
noinst_HEADERS =
noinst_HEADERS += src/scalar.h
noinst_HEADERS += src/scalar_4x64.h
noinst_HEADERS += src/scalar_8x32.h
noinst_HEADERS += src/scalar_low.h
noinst_HEADERS += src/scalar_impl.h
noinst_HEADERS += src/scalar_4x64_impl.h
noinst_HEADERS += src/scalar_8x32_impl.h
noinst_HEADERS += src/scalar_low_impl.h
noinst_HEADERS += src/group.h
noinst_HEADERS += src/group_impl.h
noinst_HEADERS += src/num_gmp.h
noinst_HEADERS += src/num_gmp_impl.h
noinst_HEADERS += src/ecdsa.h
noinst_HEADERS += src/ecdsa_impl.h
noinst_HEADERS += src/eckey.h
noinst_HEADERS += src/eckey_impl.h
noinst_HEADERS += src/ecmult.h
noinst_HEADERS += src/ecmult_impl.h
noinst_HEADERS += src/ecmult_const.h
noinst_HEADERS += src/ecmult_const_impl.h
noinst_HEADERS += src/ecmult_gen.h
noinst_HEADERS += src/ecmult_gen_impl.h
noinst_HEADERS += src/num.h
noinst_HEADERS += src/num_impl.h
noinst_HEADERS += src/field_10x26.h
noinst_HEADERS += src/field_10x26_impl.h
noinst_HEADERS += src/field_5x52.h
noinst_HEADERS += src/field_5x52_impl.h
noinst_HEADERS += src/field_5x52_int128_impl.h
noinst_HEADERS += src/field_5x52_asm_impl.h
noinst_HEADERS += src/java/org_mvc_NativeSecp256k1.h
noinst_HEADERS += src/java/org_mvc_Secp256k1Context.h
noinst_HEADERS += src/util.h
noinst_HEADERS += src/testrand.h
noinst_HEADERS += src/testrand_impl.h
noinst_HEADERS += src/hash.h
noinst_HEADERS += src/hash_impl.h
noinst_HEADERS += src/field.h
noinst_HEADERS += src/field_impl.h
noinst_HEADERS += contrib/lax_der_parsing.h
noinst_HEADERS += contrib/lax_der_parsing.c
noinst_HEADERS += contrib/lax_der_privatekey_parsing.h
noinst_HEADERS += contrib/lax_der_privatekey_parsing.c

if USE_EXTERNAL_ASM
COMMON_LIB = libsecp256k1_common.la
noinst_LTLIBRARIES = $(COMMON_LIB)
else
COMMON_LIB =
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsecp256k1.pc

if USE_EXTERNAL_ASM
if USE_ASM_ARM
libsecp256k1_common_la_SOURCES = src/asm/field_10x26_arm.s
endif
endif

libsecp256k1_la_SOURCES = src/secp256k1.c
libsecp256k1_la_CPPFLAGS = -DSECP256K1_BUILD -I$(top_srcdir)/include -I$(top_srcdir)/src $(SECP_INCLUDES)
libsecp256k1_la_LIBADD = $(JNI_LIB) $(SECP_LIBS) $(COMMON_LIB)

libsecp256k1_jni_la_SOURCES  = src/java/org_mvc_NativeSecp256k1.c src/java/org_mvc_Secp256k1Context.c
libsecp256k1_jni_la_CPPFLAGS = -DSECP256K1_BUILD $(JNI_INCLUDES)

noinst_PROGRAMS =

JAVAROOT=src/java
JAVAORG=org/mvc
JAVA_GUAVA=$(srcdir)/$(JAVAROOT)/guava/guava-18.0.jar
CLASSPATH_ENV=CLASSPATH=$(JAVA_GUAVA)
JAVA_FILES= \
  $(JAVAROOT)/$(JAVAORG)/NativeSecp256k1.java \
  $(JAVAROOT)/$(JAVAORG)/NativeSecp256k1Test.java \
  $(JAVAROOT)/$(JAVAORG)/NativeSecp256k1Util.java \
  $(JAVAROOT)/$(JAVAORG)/Secp256k1Context.java

if USE_JNI

$(JAVA_GUAVA):
	@echo Guava is missing. Fetch it via: \
	wget https://search.maven.org/remotecontent?filepath=com/google/guava/guava/18.0/guava-18.0.jar -O $(@)
	@false

.stamp-java: $(JAVA_FILES)
	@echo   Compiling $^
	$(AM_V_at)$(CLASSPATH_ENV) javac $^
	@touch $@


endif

if USE_ECMULT_STATIC_PRECOMPUTATION
CPPFLAGS_FOR_BUILD +=-I$(top_srcdir)
CFLAGS_FOR_BUILD += -Wall -Wextra -Wno-unused-function

gen_context_OBJECTS = gen_context.o
gen_context_BIN = gen_context$(BUILD_EXEEXT)
gen_%.o: src/gen_%.c
	$(CC_FOR_BUILD) $(CPPFLAGS_FOR_BUILD) $(CFLAGS_FOR_BUILD) -c $< -o $@

$(gen_context_BIN): $(gen_context_OBJECTS)
	$(CC_FOR_BUILD) $^ -o $@

$(libsecp256k1_la_OBJECTS): src/ecmult_static_context.h
$(tests_OBJECTS): src/ecmult_static_context.h
$(bench_internal_OBJECTS): src/ecmult_static_context.h

src/ecmult_static_context.h: $(gen_context_BIN)
	./$(gen_context_BIN)

CLEANFILES = $(gen_context_BIN) src/ecmult_static_context.h $(JAVAROOT)/$(JAVAORG)/*.class .stamp-java
endif

EXTRA_DIST = autogen.sh src/gen_context.c src/basic-config.h $(JAVA_FILES)

if ENABLE_MODULE_ECDH
include src/modules/ecdh/Makefile.am.include
endif

if ENABLE_MODULE_MULTISET
include src/modules/multiset/Makefile.am.include
endif

if ENABLE_MODULE_RECOVERY
include src/modules/recovery/Makefile.am.include
endif
