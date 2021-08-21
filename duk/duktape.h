/*
 *  Duktape public API for Duktape 1.2.1.
 *  See the API reference for documentation on call semantics.
 *  The exposed API is inside the DUK_API_PUBLIC_H_INCLUDED
 *  include guard.  Other parts of the header are Duktape
 *  internal and related to platform/compiler/feature detection.
 *
 *  Git commit 74bd1c845e5198b5e2d6cb7c98e54c3af1d6c0e4 (v1.2.1).
 *
 *  See Duktape AUTHORS.rst and LICENSE.txt for copyright and
 *  licensing information.
 */

/* LICENSE.txt */
/*
 *  ===============
 *  Duktape license
 *  ===============
 *  
 *  (http://opensource.org/licenses/MIT)
 *  
 *  Copyright (c) 2013-2015 by Duktape authors (see AUTHORS.rst)
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *  
 */

/* AUTHORS.rst */
/*
 *  ===============
 *  Duktape authors
 *  ===============
 *  
 *  Copyright
 *  =========
 *  
 *  Duktape copyrights are held by its authors.  Each author has a copyright
 *  to their contribution, and agrees to irrevocably license the contribution
 *  under the Duktape ``LICENSE.txt``.
 *  
 *  Authors
 *  =======
 *  
 *  Please include an e-mail address, a link to your GitHub profile, or something
 *  similar to allow your contribution to be identified accurately.
 *  
 *  The following people have contributed code and agreed to irrevocably license
 *  their contributions under the Duktape ``LICENSE.txt`` (in order of appearance):
 *  
 *  * Sami Vaarala <sami.vaarala@iki.fi>
 *  * Niki Dobrev
 *  * Andreas \u00d6man <andreas@lonelycoder.com>
 *  * L\u00e1szl\u00f3 Lang\u00f3 <llango.u-szeged@partner.samsung.com>
 *  * Legimet <legimet.calc@gmail.com>
 *  
 *  Other contributions
 *  ===================
 *  
 *  The following people have contributed something other than code (e.g. reported
 *  bugs, provided ideas, etc; roughly in order of appearance):
 *  
 *  * Greg Burns
 *  * Anthony Rabine
 *  * Carlos Costa
 *  * Aur\u00e9lien Bouilland
 *  * Preet Desai (Pris Matic)
 *  * judofyr (http://www.reddit.com/user/judofyr)
 *  * Jason Woofenden
 *  * Micha\u0142 Przyby\u015b
 *  * Anthony Howe
 *  * Conrad Pankoff
 *  * Jim Schimpf
 *  * Rajaran Gaunker (https://github.com/zimbabao)
 *  * Andreas \u00d6man
 *  * Doug Sanden
 *  * Josh Engebretson (https://github.com/JoshEngebretson)
 *  * Remo Eichenberger (https://github.com/remoe)
 *  * Mamod Mehyar (https://github.com/mamod)
 *  * David Demelier (https://github.com/hftmarkand)
 *  * Tim Caswell (https://github.com/creationix)
 *  * Mitchell Blank Jr (https://github.com/mitchblank)
 *  * https://github.com/yushli
 *  * Seo Sanghyeon (https://github.com/sanxiyn)
 *  * Han ChoongWoo (https://github.com/tunz)
 *  * Joshua Peek (https://github.com/josh)
 *  * Bruce E. Pascoe (https://github.com/fatcerberus)
 *  * https://github.com/Kelledin
 *  
 *  If you are accidentally missing from this list, send me an e-mail
 *  (``sami.vaarala@iki.fi``) and I'll fix the omission.
 */

#include "./../vb.h"

#ifndef DUKTAPE_H_INCLUDED
#define DUKTAPE_H_INCLUDED



/*
 *  Determine platform features, select feature selection defines
 *  (e.g. _XOPEN_SOURCE), include system headers, and define DUK_USE_XXX
 *  defines which are (only) checked in Duktape internal code for
 *  activated features.  Duktape feature selection is based on automatic
 *  feature detection, user supplied DUK_OPT_xxx defines, and optionally
 *  a "duk_custom.h" user header (if DUK_OPT_HAVE_CUSTOM_H is defined).
 *
 *  When compiling Duktape, DUK_COMPILING_DUKTAPE is set, and this file
 *  is included before any system headers are included.  Feature selection
 *  defines (e.g. _XOPEN_SOURCE) are defined here before any system headers
 *  are included (which is a requirement for system headers to work correctly).
 *  This file is responsible for including all system headers and contains
 *  all platform dependent cruft in general.  When compiling user code,
 *  DUK_COMPILING_DUKTAPE is not defined, and we must avoid e.g. defining
 *  unnecessary feature selection defines.
 *
 *  The general order of handling:
 *    - Compiler feature detection (require no includes)
 *    - Intermediate platform detection (-> easier platform defines)
 *    - Platform detection, system includes, byte order detection, etc
 *    - ANSI C wrappers (e.g. DUK_MEMCMP), wrappers for constants, etc
 *    - DUK_USE_xxx defines are resolved based on input defines
 *    - Duktape Date provider settings
 *    - Final sanity checks
 *
 *  DUK_F_XXX are internal feature detection macros which should not be
 *  used outside this header.
 *
 *  Useful resources:
 *
 *    http://sourceforge.net/p/predef/wiki/Home/
 *    http://sourceforge.net/p/predef/wiki/Architectures/
 *    http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
 *    http://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types
 *
 *  Preprocessor defines available in a particular GCC:
 *
 *    gcc -dM -E - </dev/null   # http://www.brain-dump.org/blog/entry/107
 */

#ifndef DUK_FEATURES_H_INCLUDED
#define DUK_FEATURES_H_INCLUDED

//-------------------------------- config block added for duk4vb ------------------------
#ifdef  __cplusplus
extern "C" {
#endif
	int ScriptTimeoutCheck(const void*udata); 
#ifdef  __cplusplus
}
#endif

//-dz defines
#define DUK_OPT_EXEC_TIMEOUT_CHECK  ScriptTimeoutCheck
#define DUK_OPT_INTERRUPT_COUNTER  1
#define DUK_OPT_DEBUGGER_SUPPORT   1
//#define DUK_OPT_DEBUGGER_FWD_LOGGING 1  this only forwards when debugging..might as well as just handle with stdout always since were local..


//-dz i added ifdef DUK_DBG_USE_PACKED_PROTOCOL to simplify unpacking of ints and strings in debugger protocol..(i dont need compact protocol)

//----------------------------------------------------------------------------------

/*
 *  Compiler features
 */

#undef DUK_F_C99
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define DUK_F_C99
#endif

#undef DUK_F_CPP
#if defined(__cplusplus)
#define DUK_F_CPP
#endif

#undef DUK_F_CPP11
#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define DUK_F_CPP11
#endif

/*
 *  Provides the duk_rdtsc() inline function (if available), limited to
 *  GCC C99.
 *
 *  See: http://www.mcs.anl.gov/~kazutomo/rdtsc.html
 */

/* XXX: more accurate detection of what gcc versions work; more inline
 * asm versions for other compilers.
 */
#if defined(__GNUC__) && defined(__i386__) && defined(DUK_F_C99) && \
    !defined(__cplusplus) /* unsigned long long not standard */
static __inline__ unsigned long long duk_rdtsc(void) {
	unsigned long long int x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
}
#define DUK_RDTSC_AVAILABLE 1
#elif defined(__GNUC__) && defined(__x86_64__) && defined(DUK_F_C99) && \
    !defined(__cplusplus) /* unsigned long long not standard */
static __inline__ unsigned long long duk_rdtsc(void) {
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}
#define DUK_RDTSC_AVAILABLE 1
#else
/* not available */
#undef DUK_RDTSC_AVAILABLE
#endif

/*
 *  Intermediate platform, architecture, and compiler detection.  These are
 *  hopelessly intertwined - e.g. architecture defines depend on compiler etc.
 *
 *  Provide easier defines for platforms and compilers which are often tricky
 *  or verbose to detect.  The intent is not to provide intermediate defines for
 *  all features; only if existing feature defines are inconvenient.
 */

/* Intel x86 (32-bit) */
#if defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(__i486__) || defined(__i586__) || defined(__i686__) || \
    defined(__IA32__) || defined(_M_IX86) || defined(__X86__) || \
    defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__)
#define DUK_F_X86
#endif

/* AMD64 (64-bit) */
#if defined(__amd64__) || defined(__amd64) || \
    defined(__x86_64__) || defined(__x86_64) || \
    defined(_M_X64) || defined(_M_AMD64)
#define DUK_F_X64
#endif

/* X32: 64-bit with 32-bit pointers (allows packed tvals).  X32 support is
 * not very mature yet.
 *
 * https://sites.google.com/site/x32abi/
 */
#if defined(DUK_F_X64) && \
    (defined(_ILP32) || defined(__ILP32__))
#define DUK_F_X32
/* define only one of: DUK_F_X86, DUK_F_X32, or DUK_F_X64 */
#undef DUK_F_X64
#undef DUK_F_X86
#endif

/* ARM */
#if defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM)
#define DUK_F_ARM
#endif

/* MIPS */
/* Related defines: __MIPSEB__, __MIPSEL__, __mips_isa_rev, __LP64__ */
#if defined(__mips__) || defined(mips) || defined(_MIPS_ISA) || \
    defined(_R3000) || defined(_R4000) || defined(_R5900) || \
    defined(_MIPS_ISA_MIPS1) || defined(_MIPS_ISA_MIPS2) || \
    defined(_MIPS_ISA_MIPS3) || defined(_MIPS_ISA_MIPS4) || \
    defined(__mips) || defined(__MIPS__)
#define DUK_F_MIPS
#if defined(__LP64__) || defined(__mips64) || defined(__mips64__) || \
    defined(__mips_n64)
#define DUK_F_MIPS64
#else
#define DUK_F_MIPS32
#endif
#endif

/* SuperH */
#if defined(__sh__) || \
    defined(__sh1__) || defined(__SH1__) || \
    defined(__sh2__) || defined(__SH2__) || \
    defined(__sh3__) || defined(__SH3__) || \
    defined(__sh4__) || defined(__SH4__) || \
    defined(__sh5__) || defined(__SH5__)
#define DUK_F_SUPERH
#endif

/* Motorola 68K.  Not defined by VBCC, so user must define one of these
 * manually when using VBCC.
 */
#if defined(__m68k__) || defined(M68000) || defined(__MC68K__)
#define DUK_F_M68K
#endif

/* Linux */
#if defined(__linux) || defined(__linux__) || defined(linux)
#define DUK_F_LINUX
#endif

/* FreeBSD */
#if defined(__FreeBSD__) || defined(__FreeBSD)
#define DUK_F_FREEBSD
#endif

/* NetBSD */
#if defined(__NetBSD__) || defined(__NetBSD)
#define DUK_F_NETBSD
#endif

/* OpenBSD */
#if defined(__OpenBSD__) || defined(__OpenBSD)
#define DUK_F_OPENBSD
#endif

/* BSD variant */
#if defined(DUK_F_FREEBSD) || defined(DUK_F_NETBSD) || defined(DUK_F_OPENBSD) || \
    defined(__bsdi__) || defined(__DragonFly__)
#define DUK_F_BSD
#endif

/* Generic Unix (includes Cygwin) */
#if defined(__unix) || defined(__unix__) || defined(unix) || \
    defined(DUK_F_LINUX) || defined(DUK_F_BSD)
#define DUK_F_UNIX
#endif

/* Cygwin */
#if defined(__CYGWIN__)
#define DUK_F_CYGWIN
#endif

/* Windows (32-bit or above) */
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define DUK_F_WINDOWS
#endif

#if defined(__APPLE__)
#define DUK_F_APPLE
#endif

/* Atari ST TOS. __TOS__ defined by PureC (which doesn't work as a target now
 * because int is 16-bit, to be fixed).  No platform define in VBCC apparently,
 * so to use with VBCC, user must define '__TOS__' manually.
  */
#if defined(__TOS__)
#define DUK_F_TOS
#endif

/* AmigaOS.  Neither AMIGA nor __amigaos__ is defined on VBCC, so user must
 * define 'AMIGA' manually.
 */
#if defined(AMIGA) || defined(__amigaos__)
#define DUK_F_AMIGAOS
#endif

/* Flash player (e.g. Crossbridge) */
#if defined(__FLASHPLAYER__)
#define DUK_F_FLASHPLAYER
#endif

/* Emscripten (provided explicitly by user), improve if possible */
#if defined(EMSCRIPTEN)
#define DUK_F_EMSCRIPTEN
#endif

/* QNX */
#if defined(__QNX__)
#define DUK_F_QNX
#endif

/* TI-Nspire (using Ndless) */
#if defined(_TINSPIRE)
#define DUK_F_TINSPIRE
#endif

/* GCC and GCC version convenience define. */
#if defined(__GNUC__)
#define DUK_F_GCC
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
/* Convenience, e.g. gcc 4.5.1 == 40501; http://stackoverflow.com/questions/6031819/emulating-gccs-builtin-unreachable */
#define DUK_F_GCC_VERSION  (__GNUC__ * 10000L + __GNUC_MINOR__ * 100L + __GNUC_PATCHLEVEL__)
#else
#error cannot figure out gcc version
#endif
#endif

/* Clang */
#if defined(__clang__)
#define DUK_F_CLANG
/* It seems clang also defines __GNUC__, so undo the GCC detection. */
#if defined(DUK_F_GCC)
#undef DUK_F_GCC
#endif
#if defined(DUK_F_GCC_VERSION)
#undef DUK_F_GCC_VERSION
#endif
#endif

/* MSVC */
#if defined(_MSC_VER)
/* MSVC preprocessor defines: http://msdn.microsoft.com/en-us/library/b0084kay.aspx
 * _MSC_FULL_VER includes the build number, but it has at least two formats, see e.g.
 * BOOST_MSVC_FULL_VER in http://www.boost.org/doc/libs/1_52_0/boost/config/compiler/visualc.hpp
 */
#define DUK_F_MSVC
#if defined(_MSC_FULL_VER)
#if (_MSC_FULL_VER > 100000000)
#define DUK_F_MSVC_FULL_VER _MSC_FULL_VER
#else
#define DUK_F_MSCV_FULL_VER (_MSC_FULL_VER * 10)
#endif
#endif
#endif  /* _MSC_VER */

/* MinGW */
#if defined(__MINGW32__) || defined(__MINGW64__)
/* NOTE: Also GCC flags are detected (DUK_F_GCC etc). */
#define DUK_F_MINGW
#endif

/* BCC (Bruce's C compiler): this is a "torture target" for compilation */
#if defined(__BCC__) || defined(__BCC_VERSION__)
#define DUK_F_BCC
#endif

#if defined(__VBCC__)
#define DUK_F_VBCC
#endif

#if (defined(DUK_F_C99) || defined(DUK_F_CPP11)) && \
    !defined(DUK_F_BCC)
/* ULL / LL preprocessor constants should be avoided because they're not
 * always available.  With suitable options, some compilers will support
 * 64-bit integer types but won't support ULL / LL preprocessor constants.
 * Assume C99/C++11 environments have these.  However, BCC is nominally
 * C99 but doesn't support these constants.
 */
#define DUK_F_ULL_CONSTS
#endif

/*
 *  Platform detection, system includes, Date provider selection.
 *
 *  Feature selection (e.g. _XOPEN_SOURCE) must happen before any system
 *  headers are included.  This header should avoid providing any feature
 *  selection defines when compiling user code (only when compiling Duktape
 *  itself).  If a feature selection option is required for user code to
 *  compile correctly (e.g. it is needed for type detection), it should
 *  probably be -checked- here, not defined here.
 *
 *  Date provider selection seems a bit out-of-place here, but since
 *  the date headers and provider functions are heavily platform
 *  specific, there's little point in duplicating the platform if-else
 *  ladder.  All platform specific Date provider functions are in
 *  duk_bi_date.c; here we provide appropriate #defines to enable them,
 *  and include all the necessary system headers so that duk_bi_date.c
 *  compiles.  Date "providers" are:
 *
 *    NOW = getting current time (required)
 *    TZO = getting local time offset (required)
 *    PRS = parse datetime (optional)
 *    FMT = format datetime (optional)
 *
 *  There's a lot of duplication here, unfortunately, because many
 *  platforms have similar (but not identical) headers, Date providers,
 *  etc.  The duplication could be removed by more complicated nested
 *  #ifdefs, but it would then be more difficult to make fixes which
 *  affect only a specific platform.
 *
 *  XXX: add a way to provide custom functions to provide the critical
 *  primitives; this would be convenient when porting to unknown platforms
 *  (rather than muck with Duktape internals).
 */

#if defined(DUK_COMPILING_DUKTAPE) && \
 (defined(DUK_F_LINUX) || defined(DUK_F_EMSCRIPTEN))
/* A more recent Emscripten (2014-05) seems to lack "linux" environment
 * defines, so check for Emscripten explicitly.
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE  200809L
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE      /* e.g. getdate_r */
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE    /* e.g. strptime */
#endif
#endif

#if defined(DUK_F_QNX) && defined(DUK_COMPILING_DUKTAPE)
/* See: /opt/qnx650/target/qnx6/usr/include/sys/platform.h */
#define _XOPEN_SOURCE    600
#define _POSIX_C_SOURCE  200112L
#endif

#undef DUK_F_MSVC_CRT_SECURE
#if defined(DUK_F_WINDOWS) && defined(_MSC_VER)
/* http://msdn.microsoft.com/en-us/library/8ef0s5kh.aspx
 * http://msdn.microsoft.com/en-us/library/wd3wzwts.aspx
 * Seem to be available since VS2005.
 */
#if (_MSC_VER >= 1400)
/* VS2005+, secure CRT functions are preferred.  Windows Store applications
 * (and probably others) should use these.
 */
#define DUK_F_MSVC_CRT_SECURE
#endif
#if (_MSC_VER < 1700)
/* VS2012+ has stdint.h, < VS2012 does not (but it's available for download). */
#define DUK_F_NO_STDINT_H
#endif
/* Initial fix: disable secure CRT related warnings when compiling Duktape
 * itself (must be defined before including Windows headers).  Don't define
 * for user code including duktape.h.
 */
#if defined(DUK_COMPILING_DUKTAPE) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif  /* DUK_F_WINDOWS && _MSC_VER */

#if defined(DUK_F_TOS) || defined(DUK_F_BCC)
#define DUK_F_NO_STDINT_H
#endif

/* Workaround for older C++ compilers before including <inttypes.h>,
 * see e.g.: https://sourceware.org/bugzilla/show_bug.cgi?id=15366
 */
#if defined(__cplusplus) && !defined(__STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS
#endif
#if defined(__cplusplus) && !defined(__STDC_CONSTANT_MACROS)
#define __STDC_CONSTANT_MACROS
#endif

#if defined(__APPLE__)
/* Mac OSX, iPhone, Darwin */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <TargetConditionals.h>
#include <architecture/byte_order.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_OPENBSD)
/* http://www.monkey.org/openbsd/archive/ports/0401/msg00089.html */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <sys/endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_BSD)
/* other BSD */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <sys/endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_TOS)
/* Atari ST TOS */
#define DUK_USE_DATE_NOW_TIME
#define DUK_USE_DATE_TZO_GMTIME
/* no parsing (not an error) */
#define DUK_USE_DATE_FMT_STRFTIME
#include <limits.h>
#include <time.h>
#elif defined(DUK_F_AMIGAOS)
#if defined(DUK_F_M68K)
/* AmigaOS on M68k */
#define DUK_USE_DATE_NOW_TIME
#define DUK_USE_DATE_TZO_GMTIME
/* no parsing (not an error) */
#define DUK_USE_DATE_FMT_STRFTIME
#include <limits.h>
#include <time.h>
#else
#error AmigaOS but not M68K, not supported now
#endif
#elif defined(DUK_F_WINDOWS)
/* Windows 32-bit and 64-bit are currently the same. */
/* MSVC does not have sys/param.h */
#define DUK_USE_DATE_NOW_WINDOWS
#define DUK_USE_DATE_TZO_WINDOWS
/* Note: PRS and FMT are intentionally left undefined for now.  This means
 * there is no platform specific date parsing/formatting but there is still
 * the ISO 8601 standard format.
 */
#include <windows.h>
#include <limits.h>
#elif defined(DUK_F_FLASHPLAYER)
/* Crossbridge */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_QNX)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_TINSPIRE)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_LINUX)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#if defined(DUK_F_BCC)
/* no endian.h */
#else
#include <endian.h>
#endif  /* DUK_F_BCC */
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(__posix)
/* POSIX */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_CYGWIN)
/* Cygwin -- don't use strptime() for now */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#else
/* Other UNIX, hopefully others */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#if defined(DUK_F_BCC)
/* no endian.h */
#else
#include <endian.h>
#endif  /* DUK_F_BCC */
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#endif

/* Shared includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  /* varargs */
#include <setjmp.h>
#include <stddef.h>  /* e.g. ptrdiff_t */
#if defined(DUK_F_NO_STDINT_H)
/* stdint.h not available */
#else
/* Technically C99 (C++11) but found in many systems.  Note the workaround
 * above for some C++ compilers (__STDC_LIMIT_MACROS etc).
 */
#include <stdint.h>
#endif
#include <math.h>

/*
 *  Detection for specific libc variants (like uclibc) and other libc specific
 *  features.  Potentially depends on the #includes above.
 */

#if defined(__UCLIBC__)
#define DUK_F_UCLIBC
#endif

/*
 *  Wrapper typedefs and constants for integer types, also sanity check types.
 *
 *  C99 typedefs are quite good but not always available, and we want to avoid
 *  forcibly redefining the C99 typedefs.  So, there are Duktape wrappers for
 *  all C99 typedefs and Duktape code should only use these typedefs.  Type
 *  detection when C99 is not supported is best effort and may end up detecting
 *  some types incorrectly.
 *
 *  Pointer sizes are a portability problem: pointers to different types may
 *  have a different size and function pointers are very difficult to manage
 *  portably.
 *
 *  http://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types
 *
 *  Note: there's an interesting corner case when trying to define minimum
 *  signed integer value constants which leads to the current workaround of
 *  defining e.g. -0x80000000 as (-0x7fffffffL - 1L).  See doc/code-issues.txt
 *  for a longer discussion.
 *
 *  Note: avoid typecasts and computations in macro integer constants as they
 *  can then no longer be used in macro relational expressions (such as
 *  #if DUK_SIZE_MAX < 0xffffffffUL).  There is internal code which relies on
 *  being able to compare DUK_SIZE_MAX against a limit.
 */

/* XXX: add feature options to force basic types from outside? */

#if !defined(INT_MAX)
#error INT_MAX not defined
#endif

/* Check that architecture is two's complement, standard C allows e.g.
 * INT_MIN to be -2**31+1 (instead of -2**31).
 */
#if defined(INT_MAX) && defined(INT_MIN)
#if INT_MAX != -(INT_MIN + 1)
#error platform does not seem complement of two
#endif
#else
#error cannot check complement of two
#endif

/* Pointer size determination based on architecture.
 * XXX: unsure about BCC correctness.
 */
#if defined(DUK_F_X86) || defined(DUK_F_X32) || \
    defined(DUK_F_BCC) || \
    (defined(__WORDSIZE) && (__WORDSIZE == 32))
#define DUK_F_32BIT_PTRS
#elif defined(DUK_F_X64) || \
      (defined(__WORDSIZE) && (__WORDSIZE == 64))
#define DUK_F_64BIT_PTRS
#else
/* not sure, not needed with C99 anyway */
#endif

/* Intermediate define for 'have inttypes.h' */
#undef DUK_F_HAVE_INTTYPES
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && \
    !(defined(DUK_F_AMIGAOS) && defined(DUK_F_VBCC))
/* vbcc + AmigaOS has C99 but no inttypes.h */
#define DUK_F_HAVE_INTTYPES
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 apparently ratified stdint.h */
#define DUK_F_HAVE_INTTYPES
#endif

/* Basic integer typedefs and limits, preferably from inttypes.h, otherwise
 * through automatic detection.
 */
#if defined(DUK_F_HAVE_INTTYPES)
/* C99 or compatible */

#define DUK_F_HAVE_64BIT
#include <inttypes.h>

typedef uint8_t duk_uint8_t;
typedef int8_t duk_int8_t;
typedef uint16_t duk_uint16_t;
typedef int16_t duk_int16_t;
typedef uint32_t duk_uint32_t;
typedef int32_t duk_int32_t;
typedef uint64_t duk_uint64_t;
typedef int64_t duk_int64_t;
typedef uint_least8_t duk_uint_least8_t;
typedef int_least8_t duk_int_least8_t;
typedef uint_least16_t duk_uint_least16_t;
typedef int_least16_t duk_int_least16_t;
typedef uint_least32_t duk_uint_least32_t;
typedef int_least32_t duk_int_least32_t;
typedef uint_least64_t duk_uint_least64_t;
typedef int_least64_t duk_int_least64_t;
typedef uint_fast8_t duk_uint_fast8_t;
typedef int_fast8_t duk_int_fast8_t;
typedef uint_fast16_t duk_uint_fast16_t;
typedef int_fast16_t duk_int_fast16_t;
typedef uint_fast32_t duk_uint_fast32_t;
typedef int_fast32_t duk_int_fast32_t;
typedef uint_fast64_t duk_uint_fast64_t;
typedef int_fast64_t duk_int_fast64_t;
typedef uintptr_t duk_uintptr_t;
typedef intptr_t duk_intptr_t;
typedef uintmax_t duk_uintmax_t;
typedef intmax_t duk_intmax_t;

#define DUK_UINT8_MIN         0
#define DUK_UINT8_MAX         UINT8_MAX
#define DUK_INT8_MIN          INT8_MIN
#define DUK_INT8_MAX          INT8_MAX
#define DUK_UINT_LEAST8_MIN   0
#define DUK_UINT_LEAST8_MAX   UINT_LEAST8_MAX
#define DUK_INT_LEAST8_MIN    INT_LEAST8_MIN
#define DUK_INT_LEAST8_MAX    INT_LEAST8_MAX
#define DUK_UINT_FAST8_MIN    0
#define DUK_UINT_FAST8_MAX    UINT_FAST8_MAX
#define DUK_INT_FAST8_MIN     INT_FAST8_MIN
#define DUK_INT_FAST8_MAX     INT_FAST8_MAX
#define DUK_UINT16_MIN        0
#define DUK_UINT16_MAX        UINT16_MAX
#define DUK_INT16_MIN         INT16_MIN
#define DUK_INT16_MAX         INT16_MAX
#define DUK_UINT_LEAST16_MIN  0
#define DUK_UINT_LEAST16_MAX  UINT_LEAST16_MAX
#define DUK_INT_LEAST16_MIN   INT_LEAST16_MIN
#define DUK_INT_LEAST16_MAX   INT_LEAST16_MAX
#define DUK_UINT_FAST16_MIN   0
#define DUK_UINT_FAST16_MAX   UINT_FAST16_MAX
#define DUK_INT_FAST16_MIN    INT_FAST16_MIN
#define DUK_INT_FAST16_MAX    INT_FAST16_MAX
#define DUK_UINT32_MIN        0
#define DUK_UINT32_MAX        UINT32_MAX
#define DUK_INT32_MIN         INT32_MIN
#define DUK_INT32_MAX         INT32_MAX
#define DUK_UINT_LEAST32_MIN  0
#define DUK_UINT_LEAST32_MAX  UINT_LEAST32_MAX
#define DUK_INT_LEAST32_MIN   INT_LEAST32_MIN
#define DUK_INT_LEAST32_MAX   INT_LEAST32_MAX
#define DUK_UINT_FAST32_MIN   0
#define DUK_UINT_FAST32_MAX   UINT_FAST32_MAX
#define DUK_INT_FAST32_MIN    INT_FAST32_MIN
#define DUK_INT_FAST32_MAX    INT_FAST32_MAX
#define DUK_UINT64_MIN        0
#define DUK_UINT64_MAX        UINT64_MAX
#define DUK_INT64_MIN         INT64_MIN
#define DUK_INT64_MAX         INT64_MAX
#define DUK_UINT_LEAST64_MIN  0
#define DUK_UINT_LEAST64_MAX  UINT_LEAST64_MAX
#define DUK_INT_LEAST64_MIN   INT_LEAST64_MIN
#define DUK_INT_LEAST64_MAX   INT_LEAST64_MAX
#define DUK_UINT_FAST64_MIN   0
#define DUK_UINT_FAST64_MAX   UINT_FAST64_MAX
#define DUK_INT_FAST64_MIN    INT_FAST64_MIN
#define DUK_INT_FAST64_MAX    INT_FAST64_MAX

#define DUK_UINTPTR_MIN       0
#define DUK_UINTPTR_MAX       UINTPTR_MAX
#define DUK_INTPTR_MIN        INTPTR_MIN
#define DUK_INTPTR_MAX        INTPTR_MAX

#define DUK_UINTMAX_MIN       0
#define DUK_UINTMAX_MAX       UINTMAX_MAX
#define DUK_INTMAX_MIN        INTMAX_MIN
#define DUK_INTMAX_MAX        INTMAX_MAX

#define DUK_SIZE_MIN          0
#define DUK_SIZE_MAX          SIZE_MAX

#else  /* C99 types */

/* When C99 types are not available, we use heuristic detection to get
 * the basic 8, 16, 32, and (possibly) 64 bit types.  The fast/least
 * types are then assumed to be exactly the same for now: these could
 * be improved per platform but C99 types are very often now available.
 * 64-bit types are not available on all platforms; this is OK at least
 * on 32-bit platforms.
 *
 * This detection code is necessarily a bit hacky and can provide typedefs
 * and defines that won't work correctly on some exotic platform.
 */

#if (defined(CHAR_BIT) && (CHAR_BIT == 8)) || \
    (defined(UCHAR_MAX) && (UCHAR_MAX == 255))
typedef unsigned char duk_uint8_t;
typedef signed char duk_int8_t;
#else
#error cannot detect 8-bit type
#endif

#if defined(USHRT_MAX) && (USHRT_MAX == 65535UL)
typedef unsigned short duk_uint16_t;
typedef signed short duk_int16_t;
#elif defined(UINT_MAX) && (UINT_MAX == 65535UL)
/* On some platforms int is 16-bit but long is 32-bit (e.g. PureC) */
typedef unsigned int duk_uint16_t;
typedef signed int duk_int16_t;
#else
#error cannot detect 16-bit type
#endif

#if defined(UINT_MAX) && (UINT_MAX == 4294967295UL)
typedef unsigned int duk_uint32_t;
typedef signed int duk_int32_t;
#elif defined(ULONG_MAX) && (ULONG_MAX == 4294967295UL)
/* On some platforms int is 16-bit but long is 32-bit (e.g. PureC) */
typedef unsigned long duk_uint32_t;
typedef signed long duk_int32_t;
#else
#error cannot detect 32-bit type
#endif

/* 64-bit type detection is a bit tricky.
 *
 * ULLONG_MAX is a standard define.  __LONG_LONG_MAX__ and __ULONG_LONG_MAX__
 * are used by at least GCC (even if system headers don't provide ULLONG_MAX).
 * Some GCC variants may provide __LONG_LONG_MAX__ but not __ULONG_LONG_MAX__.
 *
 * ULL / LL constants are rejected / warned about by some compilers, even if
 * the compiler has a 64-bit type and the compiler/system headers provide an
 * unsupported constant (ULL/LL)!  Try to avoid using ULL / LL constants.
 * As a side effect we can only check that e.g. ULONG_MAX is larger than 32
 * bits but can't be sure it is exactly 64 bits.  Self tests will catch such
 * cases.
 */
#undef DUK_F_HAVE_64BIT
#if !defined(DUK_F_HAVE_64BIT) && defined(ULONG_MAX)
#if (ULONG_MAX > 4294967295UL)
#define DUK_F_HAVE_64BIT
typedef unsigned long duk_uint64_t;
typedef signed long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && defined(ULLONG_MAX)
#if (ULLONG_MAX > 4294967295UL)
#define DUK_F_HAVE_64BIT
typedef unsigned long long duk_uint64_t;
typedef signed long long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && defined(__ULONG_LONG_MAX__)
#if (__ULONG_LONG_MAX__ > 4294967295UL)
#define DUK_F_HAVE_64BIT
typedef unsigned long long duk_uint64_t;
typedef signed long long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && defined(__LONG_LONG_MAX__)
#if (__LONG_LONG_MAX__ > 2147483647L)
#define DUK_F_HAVE_64BIT
typedef unsigned long long duk_uint64_t;
typedef signed long long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && \
    (defined(DUK_F_MINGW) || defined(DUK_F_MSVC))
/* Both MinGW and MSVC have a 64-bit type. */
#define DUK_F_HAVE_64BIT
typedef unsigned long duk_uint64_t;
typedef signed long duk_int64_t;
#endif
#if !defined(DUK_F_HAVE_64BIT)
/* cannot detect 64-bit type, not always needed so don't error */
#endif

typedef duk_uint8_t duk_uint_least8_t;
typedef duk_int8_t duk_int_least8_t;
typedef duk_uint16_t duk_uint_least16_t;
typedef duk_int16_t duk_int_least16_t;
typedef duk_uint32_t duk_uint_least32_t;
typedef duk_int32_t duk_int_least32_t;
typedef duk_uint8_t duk_uint_fast8_t;
typedef duk_int8_t duk_int_fast8_t;
typedef duk_uint16_t duk_uint_fast16_t;
typedef duk_int16_t duk_int_fast16_t;
typedef duk_uint32_t duk_uint_fast32_t;
typedef duk_int32_t duk_int_fast32_t;
#if defined(DUK_F_HAVE_64BIT)
typedef duk_uint64_t duk_uint_least64_t;
typedef duk_int64_t duk_int_least64_t;
typedef duk_uint64_t duk_uint_fast64_t;
typedef duk_int64_t duk_int_fast64_t;
#endif
#if defined(DUK_F_HAVE_64BIT)
typedef duk_uint64_t duk_uintmax_t;
typedef duk_int64_t duk_intmax_t;
#else
typedef duk_uint32_t duk_uintmax_t;
typedef duk_int32_t duk_intmax_t;
#endif

/* Note: the funny looking computations for signed minimum 16-bit, 32-bit, and
 * 64-bit values are intentional as the obvious forms (e.g. -0x80000000L) are
 * -not- portable.  See code-issues.txt for a detailed discussion.
 */
#define DUK_UINT8_MIN         0UL
#define DUK_UINT8_MAX         0xffUL
#define DUK_INT8_MIN          (-0x80L)
#define DUK_INT8_MAX          0x7fL
#define DUK_UINT_LEAST8_MIN   0UL
#define DUK_UINT_LEAST8_MAX   0xffUL
#define DUK_INT_LEAST8_MIN    (-0x80L)
#define DUK_INT_LEAST8_MAX    0x7fL
#define DUK_UINT_FAST8_MIN    0UL
#define DUK_UINT_FAST8_MAX    0xffUL
#define DUK_INT_FAST8_MIN     (-0x80L)
#define DUK_INT_FAST8_MAX     0x7fL
#define DUK_UINT16_MIN        0UL
#define DUK_UINT16_MAX        0xffffUL
#define DUK_INT16_MIN         (-0x7fffL - 1L)
#define DUK_INT16_MAX         0x7fffL
#define DUK_UINT_LEAST16_MIN  0UL
#define DUK_UINT_LEAST16_MAX  0xffffUL
#define DUK_INT_LEAST16_MIN   (-0x7fffL - 1L)
#define DUK_INT_LEAST16_MAX   0x7fffL
#define DUK_UINT_FAST16_MIN   0UL
#define DUK_UINT_FAST16_MAX   0xffffUL
#define DUK_INT_FAST16_MIN    (-0x7fffL - 1L)
#define DUK_INT_FAST16_MAX    0x7fffL
#define DUK_UINT32_MIN        0UL
#define DUK_UINT32_MAX        0xffffffffUL
#define DUK_INT32_MIN         (-0x7fffffffL - 1L)
#define DUK_INT32_MAX         0x7fffffffL
#define DUK_UINT_LEAST32_MIN  0UL
#define DUK_UINT_LEAST32_MAX  0xffffffffUL
#define DUK_INT_LEAST32_MIN   (-0x7fffffffL - 1L)
#define DUK_INT_LEAST32_MAX   0x7fffffffL
#define DUK_UINT_FAST32_MIN   0UL
#define DUK_UINT_FAST32_MAX   0xffffffffUL
#define DUK_INT_FAST32_MIN    (-0x7fffffffL - 1L)
#define DUK_INT_FAST32_MAX    0x7fffffffL

/* 64-bit constants.  Since LL / ULL constants are not always available,
 * use computed values.  These values can't be used in preprocessor
 * comparisons; flag them as such.
 */
#if defined(DUK_F_HAVE_64BIT)
#define DUK_UINT64_MIN        ((duk_uint64_t) 0)
#define DUK_UINT64_MAX        ((duk_uint64_t) -1)
#define DUK_INT64_MIN         ((duk_int64_t) (~(DUK_UINT64_MAX >> 1)))
#define DUK_INT64_MAX         ((duk_int64_t) (DUK_UINT64_MAX >> 1))
#define DUK_UINT_LEAST64_MIN  DUK_UINT64_MIN
#define DUK_UINT_LEAST64_MAX  DUK_UINT64_MAX
#define DUK_INT_LEAST64_MIN   DUK_INT64_MIN
#define DUK_INT_LEAST64_MAX   DUK_INT64_MAX
#define DUK_UINT_FAST64_MIN   DUK_UINT64_MIN
#define DUK_UINT_FAST64_MAX   DUK_UINT64_MAX
#define DUK_INT_FAST64_MIN    DUK_INT64_MIN
#define DUK_INT_FAST64_MAX    DUK_INT64_MAX
#define DUK_UINT64_MIN_COMPUTED
#define DUK_UINT64_MAX_COMPUTED
#define DUK_INT64_MIN_COMPUTED
#define DUK_INT64_MAX_COMPUTED
#define DUK_UINT_LEAST64_MIN_COMPUTED
#define DUK_UINT_LEAST64_MAX_COMPUTED
#define DUK_INT_LEAST64_MIN_COMPUTED
#define DUK_INT_LEAST64_MAX_COMPUTED
#define DUK_UINT_FAST64_MIN_COMPUTED
#define DUK_UINT_FAST64_MAX_COMPUTED
#define DUK_INT_FAST64_MIN_COMPUTED
#define DUK_INT_FAST64_MAX_COMPUTED
#endif

#if defined(DUK_F_HAVE_64BIT)
#define DUK_UINTMAX_MIN       DUK_UINT64_MIN
#define DUK_UINTMAX_MAX       DUK_UINT64_MAX
#define DUK_INTMAX_MIN        DUK_INT64_MIN
#define DUK_INTMAX_MAX        DUK_INT64_MAX
#define DUK_UINTMAX_MIN_COMPUTED
#define DUK_UINTMAX_MAX_COMPUTED
#define DUK_INTMAX_MIN_COMPUTED
#define DUK_INTMAX_MAX_COMPUTED
#else
#define DUK_UINTMAX_MIN       0UL
#define DUK_UINTMAX_MAX       0xffffffffUL
#define DUK_INTMAX_MIN        (-0x7fffffffL - 1L)
#define DUK_INTMAX_MAX        0x7fffffffL
#endif

/* This detection is not very reliable. */
#if defined(DUK_F_32BIT_PTRS)
typedef duk_int32_t duk_intptr_t;
typedef duk_uint32_t duk_uintptr_t;
#define DUK_UINTPTR_MIN       DUK_UINT32_MIN
#define DUK_UINTPTR_MAX       DUK_UINT32_MAX
#define DUK_INTPTR_MIN        DUK_INT32_MIN
#define DUK_INTPTR_MAX        DUK_INT32_MAX
#elif defined(DUK_F_64BIT_PTRS) && defined(DUK_F_HAVE_64BIT)
typedef duk_int64_t duk_intptr_t;
typedef duk_uint64_t duk_uintptr_t;
#define DUK_UINTPTR_MIN       DUK_UINT64_MIN
#define DUK_UINTPTR_MAX       DUK_UINT64_MAX
#define DUK_INTPTR_MIN        DUK_INT64_MIN
#define DUK_INTPTR_MAX        DUK_INT64_MAX
#define DUK_UINTPTR_MIN_COMPUTED
#define DUK_UINTPTR_MAX_COMPUTED
#define DUK_INTPTR_MIN_COMPUTED
#define DUK_INTPTR_MAX_COMPUTED
#else
#error cannot determine intptr type
#endif

/* SIZE_MAX may be missing so use an approximate value for it. */
#undef DUK_SIZE_MAX_COMPUTED
#if !defined(SIZE_MAX)
#define DUK_SIZE_MAX_COMPUTED
#define SIZE_MAX              ((size_t) (-1))
#endif
#define DUK_SIZE_MIN          0
#define DUK_SIZE_MAX          SIZE_MAX

#endif  /* C99 types */

/* A few types are assumed to always exist. */
typedef size_t duk_size_t;
typedef ptrdiff_t duk_ptrdiff_t;

/* The best type for an "all around int" in Duktape internals is "at least
 * 32 bit signed integer" which is most convenient.  Same for unsigned type.
 * Prefer 'int' when large enough, as it is almost always a convenient type.
 */
#if defined(UINT_MAX) && (UINT_MAX >= 0xffffffffUL)
typedef int duk_int_t;
typedef unsigned int duk_uint_t;
#define DUK_INT_MIN           INT_MIN
#define DUK_INT_MAX           INT_MAX
#define DUK_UINT_MIN          0
#define DUK_UINT_MAX          UINT_MAX
#else
typedef duk_int_fast32_t duk_int_t;
typedef duk_uint_fast32_t duk_uint_t;
#define DUK_INT_MIN           DUK_INT_FAST32_MIN
#define DUK_INT_MAX           DUK_INT_FAST32_MAX
#define DUK_UINT_MIN          DUK_UINT_FAST32_MIN
#define DUK_UINT_MAX          DUK_UINT_FAST32_MAX
#endif

/* Same as 'duk_int_t' but guaranteed to be a 'fast' variant if this
 * distinction matters for the CPU.  These types are used mainly in the
 * executor where it might really matter.
 */
typedef duk_int_fast32_t duk_int_fast_t;
typedef duk_uint_fast32_t duk_uint_fast_t;
#define DUK_INT_FAST_MIN      DUK_INT_FAST32_MIN
#define DUK_INT_FAST_MAX      DUK_INT_FAST32_MAX
#define DUK_UINT_FAST_MIN     DUK_UINT_FAST32_MIN
#define DUK_UINT_FAST_MAX     DUK_UINT_FAST32_MAX

/* Small integers (16 bits or more) can fall back to the 'int' type, but
 * have a typedef so they are marked "small" explicitly.
 */
typedef int duk_small_int_t;
typedef unsigned int duk_small_uint_t;
#define DUK_SMALL_INT_MIN     INT_MIN
#define DUK_SMALL_INT_MAX     INT_MAX
#define DUK_SMALL_UINT_MIN    0
#define DUK_SMALL_UINT_MAX    UINT_MAX

/* Fast variants of small integers, again for really fast paths like the
 * executor.
 */
typedef duk_int_fast16_t duk_small_int_fast_t;
typedef duk_uint_fast16_t duk_small_uint_fast_t;
#define DUK_SMALL_INT_FAST_MIN    DUK_INT_FAST16_MIN
#define DUK_SMALL_INT_FAST_MAX    DUK_INT_FAST16_MAX
#define DUK_SMALL_UINT_FAST_MIN   DUK_UINT_FAST16_MIN
#define DUK_SMALL_UINT_FAST_MAX   DUK_UINT_FAST16_MAX

/* Boolean values are represented with the platform 'int'. */
typedef duk_small_int_t duk_bool_t;
#define DUK_BOOL_MIN              DUK_SMALL_INT_MIN
#define DUK_BOOL_MAX              DUK_SMALL_INT_MAX

/* Index values must have at least 32-bit signed range. */
typedef duk_int_t duk_idx_t;
#define DUK_IDX_MIN               DUK_INT_MIN
#define DUK_IDX_MAX               DUK_INT_MAX

/* Array index values, could be exact 32 bits.
 * Currently no need for signed duk_arridx_t.
 */
typedef duk_uint_t duk_uarridx_t;
#define DUK_UARRIDX_MIN           DUK_UINT_MIN
#define DUK_UARRIDX_MAX           DUK_UINT_MAX

/* Duktape/C function return value, platform int is enough for now to
 * represent 0, 1, or negative error code.  Must be compatible with
 * assigning truth values (e.g. duk_ret_t rc = (foo == bar);).
 */
typedef duk_small_int_t duk_ret_t;
#define DUK_RET_MIN               DUK_SMALL_INT_MIN
#define DUK_RET_MAX               DUK_SMALL_INT_MAX

/* Error codes are represented with platform int.  High bits are used
 * for flags and such, so 32 bits are needed.
 */
typedef duk_int_t duk_errcode_t;
#define DUK_ERRCODE_MIN           DUK_INT_MIN
#define DUK_ERRCODE_MAX           DUK_INT_MAX

/* Codepoint type.  Must be 32 bits or more because it is used also for
 * internal codepoints.  The type is signed because negative codepoints
 * are used as internal markers (e.g. to mark EOF or missing argument).
 * (X)UTF-8/CESU-8 encode/decode take and return an unsigned variant to
 * ensure duk_uint32_t casts back and forth nicely.  Almost everything
 * else uses the signed one.
 */
typedef duk_int_t duk_codepoint_t;
typedef duk_uint_t duk_ucodepoint_t;
#define DUK_CODEPOINT_MIN         DUK_INT_MIN
#define DUK_CODEPOINT_MAX         DUK_INT_MAX
#define DUK_UCODEPOINT_MIN        DUK_UINT_MIN
#define DUK_UCODEPOINT_MAX        DUK_UINT_MAX

/* IEEE double typedef. */
typedef double duk_double_t;

/* We're generally assuming that we're working on a platform with a 32-bit
 * address space.  If DUK_SIZE_MAX is a typecast value (which is necessary
 * if SIZE_MAX is missing), the check must be avoided because the
 * preprocessor can't do a comparison.
 */
#if !defined(DUK_SIZE_MAX)
#error DUK_SIZE_MAX is undefined, probably missing SIZE_MAX
#elif !defined(DUK_SIZE_MAX_COMPUTED)
#if DUK_SIZE_MAX < 0xffffffffUL
/* On some systems SIZE_MAX can be smaller than max unsigned 32-bit value
 * which seems incorrect if size_t is (at least) an unsigned 32-bit type.
 * However, it doesn't seem useful to error out compilation if this is the
 * case.
 */
#endif
#endif

/* Convenience define: 32-bit pointers.  32-bit platforms are an important
 * footprint optimization target, and this define allows e.g. struct sizes
 * to be organized for compactness.
 */

#undef DUK_USE_32BIT_PTRS
#if defined(DUK_UINTPTR_MAX) && !defined(DUK_UINTPTR_MAX_COMPUTED)
#if DUK_UINTPTR_MAX <= 0xffffffffUL
#define DUK_USE_32BIT_PTRS
#endif
#endif

/*
 *  Check whether we should use 64-bit integers
 */

/* Quite incomplete now.  Use 64-bit types if detected (C99 or other detection)
 * unless they are known to be unreliable.  For instance, 64-bit types are
 * available on VBCC but seem to misbehave.
 */
#if defined(DUK_F_HAVE_64BIT) && !defined(DUK_F_VBCC)
#define DUK_USE_64BIT_OPS
#else
#undef DUK_USE_64BIT_OPS
#endif

/*
 *  Alignment requirement and support for unaligned accesses
 *
 *  Assume unaligned accesses are not supported unless specifically allowed
 *  in the target platform.  Some platforms may support unaligned accesses
 *  but alignment to 4 or 8 may still be desirable.
 */

#undef DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#undef DUK_USE_ALIGN_4
#undef DUK_USE_ALIGN_8

#if defined(DUK_F_EMSCRIPTEN)
/* Required on at least some targets, so use whenever Emscripten used,
 * regardless of compilation target.
 */
#define DUK_USE_ALIGN_8
#elif defined(DUK_F_ARM)
#define DUK_USE_ALIGN_4
#elif defined(DUK_F_MIPS32)
/* Based on 'make checkalign' there are no alignment requirements on
 * Linux MIPS except for doubles, which need align by 4.  Alignment
 * requirements vary based on target though.
 */
#define DUK_USE_ALIGN_4
#elif defined(DUK_F_MIPS64)
/* Good default is a bit arbitrary because alignment requirements
 * depend on target.  See https://github.com/svaarala/duktape/issues/102.
 */
#define DUK_USE_ALIGN_8
#elif defined(DUK_F_SUPERH)
/* Based on 'make checkalign' there are no alignment requirements on
 * Linux SH4, but align by 4 is probably a good basic default.
 */
#define DUK_USE_ALIGN_4
#elif defined(DUK_F_X86) || defined(DUK_F_X32) || defined(DUK_F_X64) || \
      defined(DUK_F_BCC)
/* XXX: This is technically not guaranteed because it's possible to configure
 * an x86 to require aligned accesses with Alignment Check (AC) flag.
 */
#define DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#else
/* Unknown, use safe default */
#define DUK_USE_ALIGN_8
#endif

/* User forced alignment to 4 or 8. */
#if defined(DUK_OPT_FORCE_ALIGN)
#undef DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#undef DUK_USE_ALIGN_4
#undef DUK_USE_ALIGN_8
#if (DUK_OPT_FORCE_ALIGN == 4)
#define DUK_USE_ALIGN_4
#elif (DUK_OPT_FORCE_ALIGN == 8)
#define DUK_USE_ALIGN_8
#else
#error invalid DUK_OPT_FORCE_ALIGN value
#endif
#endif

/* Compiler specific hackery needed to force struct size to match aligment,
 * see e.g. duk_hbuffer.h.
 *
 * http://stackoverflow.com/questions/11130109/c-struct-size-alignment
 * http://stackoverflow.com/questions/10951039/specifying-64-bit-alignment
 */
#if defined(DUK_F_MSVC)
#define DUK_USE_PACK_MSVC_PRAGMA
#elif defined(DUK_F_GCC)
#define DUK_USE_PACK_GCC_ATTR
#elif defined(DUK_F_CLANG)
#define DUK_USE_PACK_CLANG_ATTR
#else
#define DUK_USE_PACK_DUMMY_MEMBER
#endif

#ifdef DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#define DUK_USE_HASHBYTES_UNALIGNED_U32_ACCESS
#else
#undef DUK_USE_HASHBYTES_UNALIGNED_U32_ACCESS
#endif

/* Object property allocation layout has implications for memory and code
 * footprint and generated code size/speed.  The best layout also depends
 * on whether the platform has alignment requirements or benefits from
 * having mostly aligned accesses.
 */
#undef DUK_USE_HOBJECT_LAYOUT_1
#undef DUK_USE_HOBJECT_LAYOUT_2
#undef DUK_USE_HOBJECT_LAYOUT_3
#if defined(DUK_USE_UNALIGNED_ACCESSES_POSSIBLE) && \
    !defined(DUK_USE_ALIGN_4) && !defined(DUK_USE_ALIGN_8)
/* On platforms without any alignment issues, layout 1 is preferable
 * because it compiles to slightly less code and provides direct access
 * to property keys.
 */
#define DUK_USE_HOBJECT_LAYOUT_1
#else
/* On other platforms use layout 2, which requires some padding but
 * is a bit more natural than layout 3 in ordering the entries.  Layout
 * 3 is currently not used.
 */
#define DUK_USE_HOBJECT_LAYOUT_2
#endif

/*
 *  Byte order and double memory layout detection
 *
 *  Endianness detection is a major portability hassle because the macros
 *  and headers are not standardized.  There's even variance across UNIX
 *  platforms.  Even with "standard" headers, details like underscore count
 *  varies between platforms, e.g. both __BYTE_ORDER and _BYTE_ORDER are used
 *  (Crossbridge has a single underscore, for instance).
 *
 *  The checks below are structured with this in mind: several approaches are
 *  used, and at the end we check if any of them worked.  This allows generic
 *  approaches to be tried first, and platform/compiler specific hacks tried
 *  last.  As a last resort, the user can force a specific endianness, as it's
 *  not likely that automatic detection will work on the most exotic platforms.
 *
 *  Duktape supports little and big endian machines.  There's also support
 *  for a hybrid used by some ARM machines where integers are little endian
 *  but IEEE double values use a mixed order (12345678 -> 43218765).  This
 *  byte order for doubles is referred to as "mixed endian".
 */

#undef DUK_F_BYTEORDER
#undef DUK_USE_BYTEORDER_FORCED

/* DUK_F_BYTEORDER is set as an intermediate value when detection
 * succeeds, to one of:
 *   1 = little endian
 *   2 = mixed (arm hybrid) endian
 *   3 = big endian
 */

/* For custom platforms allow user to define byteorder explicitly.
 * Since endianness headers are not standardized, this is a useful
 * workaround for custom platforms for which endianness detection
 * is not directly supported.  Perhaps custom hardware is used and
 * user cannot submit upstream patches.
 */
#if defined(DUK_OPT_FORCE_BYTEORDER)
#if (DUK_OPT_FORCE_BYTEORDER == 1)
#define DUK_F_BYTEORDER 1
#elif (DUK_OPT_FORCE_BYTEORDER == 2)
#define DUK_F_BYTEORDER 2
#elif (DUK_OPT_FORCE_BYTEORDER == 3)
#define DUK_F_BYTEORDER 3
#else
#error invalid DUK_OPT_FORCE_BYTEORDER value
#endif
#define DUK_USE_BYTEORDER_FORCED
#endif  /* DUK_OPT_FORCE_BYTEORDER */

/* More or less standard endianness predefines provided by header files.
 * The ARM hybrid case is detected by assuming that __FLOAT_WORD_ORDER
 * will be big endian, see: http://lists.mysql.com/internals/443.
 */
#if !defined(DUK_F_BYTEORDER)
#if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && (__BYTE_ORDER == __LITTLE_ENDIAN) || \
    defined(_BYTE_ORDER) && defined(_LITTLE_ENDIAN) && (_BYTE_ORDER == _LITTLE_ENDIAN) || \
    defined(__LITTLE_ENDIAN__)
/* Integer little endian */
#if defined(__FLOAT_WORD_ORDER) && defined(__LITTLE_ENDIAN) && (__FLOAT_WORD_ORDER == __LITTLE_ENDIAN) || \
    defined(_FLOAT_WORD_ORDER) && defined(_LITTLE_ENDIAN) && (_FLOAT_WORD_ORDER == _LITTLE_ENDIAN)
#define DUK_F_BYTEORDER 1
#elif defined(__FLOAT_WORD_ORDER) && defined(__BIG_ENDIAN) && (__FLOAT_WORD_ORDER == __BIG_ENDIAN) || \
      defined(_FLOAT_WORD_ORDER) && defined(_BIG_ENDIAN) && (_FLOAT_WORD_ORDER == _BIG_ENDIAN)
#define DUK_F_BYTEORDER 2
#elif !defined(__FLOAT_WORD_ORDER) && !defined(_FLOAT_WORD_ORDER)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 1
#else
/* byte order is little endian but cannot determine IEEE double word order */
#endif  /* float word order */
#elif defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && (__BYTE_ORDER == __BIG_ENDIAN) || \
      defined(_BYTE_ORDER) && defined(_BIG_ENDIAN) && (_BYTE_ORDER == _BIG_ENDIAN) || \
      defined(__BIG_ENDIAN__)
/* Integer big endian */
#if defined(__FLOAT_WORD_ORDER) && defined(__BIG_ENDIAN) && (__FLOAT_WORD_ORDER == __BIG_ENDIAN) || \
    defined(_FLOAT_WORD_ORDER) && defined(_BIG_ENDIAN) && (_FLOAT_WORD_ORDER == _BIG_ENDIAN)
#define DUK_F_BYTEORDER 3
#elif !defined(__FLOAT_WORD_ORDER) && !defined(_FLOAT_WORD_ORDER)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 3
#else
/* byte order is big endian but cannot determine IEEE double word order */
#endif  /* float word order */
#else
/* cannot determine byte order */
#endif  /* integer byte order */
#endif  /* !defined(DUK_F_BYTEORDER) */

/* GCC and Clang provide endianness defines as built-in predefines, with
 * leading and trailing double underscores (e.g. __BYTE_ORDER__).  See
 * output of "make gccpredefs" and "make clangpredefs".  Clang doesn't
 * seem to provide __FLOAT_WORD_ORDER__.
 * http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
 */
#if !defined(DUK_F_BYTEORDER) && defined(__BYTE_ORDER__)
#if defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
/* Integer little endian */
#if defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
    (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define DUK_F_BYTEORDER 1
#elif defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
      (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
#define DUK_F_BYTEORDER 2
#elif !defined(__FLOAT_WORD_ORDER__)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 1
#else
/* byte order is little endian but cannot determine IEEE double word order */
#endif  /* float word order */
#elif defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
/* Integer big endian */
#if defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
    (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
#define DUK_F_BYTEORDER 3
#elif !defined(__FLOAT_WORD_ORDER__)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 3
#else
/* byte order is big endian but cannot determine IEEE double word order */
#endif  /* float word order */
#else
/* cannot determine byte order; __ORDER_PDP_ENDIAN__ is related to 32-bit
 * integer ordering and is not relevant
 */
#endif  /* integer byte order */
#endif  /* !defined(DUK_F_BYTEORDER) && defined(__BYTE_ORDER__) */

/* Atari ST TOS */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_TOS)
#define DUK_F_BYTEORDER 3
#endif

/* AmigaOS on M68k */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_AMIGAOS)
#if defined(DUK_F_M68K)
#define DUK_F_BYTEORDER 3
#endif
#endif

/* On Windows, assume we're little endian.  Even Itanium which has a
 * configurable endianness runs little endian in Windows.
 */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_WINDOWS)
/* XXX: verify that Windows on ARM is little endian for floating point
 * values too.
 */
#define DUK_F_BYTEORDER 1
#endif  /* Windows */

/* Crossbridge should work with the standard byteorder #ifdefs.  It doesn't
 * provide _FLOAT_WORD_ORDER but the standard approach now covers that case
 * too.  This has been left here just in case.
 */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_FLASHPLAYER)
#define DUK_F_BYTEORDER 1
#endif

/* QNX gcc cross compiler seems to define e.g. __LITTLEENDIAN__ or __BIGENDIAN__:
 *  $ /opt/qnx650/host/linux/x86/usr/bin/i486-pc-nto-qnx6.5.0-gcc -dM -E - </dev/null | grep -ni endian
 *  67:#define __LITTLEENDIAN__ 1
 *  $ /opt/qnx650/host/linux/x86/usr/bin/mips-unknown-nto-qnx6.5.0-gcc -dM -E - </dev/null | grep -ni endian
 *  81:#define __BIGENDIAN__ 1
 *  $ /opt/qnx650/host/linux/x86/usr/bin/arm-unknown-nto-qnx6.5.0-gcc -dM -E - </dev/null | grep -ni endian
 *  70:#define __LITTLEENDIAN__ 1
 */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_QNX)
/* XXX: ARM hybrid? */
#if defined(__LITTLEENDIAN__)
#define DUK_F_BYTEORDER 1
#elif defined(__BIGENDIAN__)
#define DUK_F_BYTEORDER 3
#endif
#endif

/* Bruce's C Compiler (BCC), assume we're on x86. */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_BCC)
#define DUK_F_BYTEORDER 1
#endif

/* Check whether or not byte order detection worked based on the intermediate
 * define, and define final values.  If detection failed, #error out.
 */
#if defined(DUK_F_BYTEORDER)
#if (DUK_F_BYTEORDER == 1)
#define DUK_USE_INTEGER_LE
#define DUK_USE_DOUBLE_LE
#elif (DUK_F_BYTEORDER == 2)
#define DUK_USE_INTEGER_LE  /* integer endianness is little on purpose */
#define DUK_USE_DOUBLE_ME
#elif (DUK_F_BYTEORDER == 3)
#define DUK_USE_INTEGER_BE
#define DUK_USE_DOUBLE_BE
#else
#error unsupported: byte order detection failed (internal error, should not happen)
#endif  /* byte order */
#else
#error unsupported: byte order detection failed
#endif  /* defined(DUK_F_BYTEORDER) */

/*
 *  Check whether or not a packed duk_tval representation is possible.
 *  What's basically required is that pointers are 32-bit values
 *  (sizeof(void *) == 4).  Best effort check, not always accurate.
 *  If guess goes wrong, crashes may result; self tests also verify
 *  the guess.
 */

#undef DUK_USE_PACKED_TVAL_POSSIBLE

/* Strict C99 case: DUK_UINTPTR_MAX (= UINTPTR_MAX) should be very reliable */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_HAVE_INTTYPES) && defined(DUK_UINTPTR_MAX)
#if (DUK_UINTPTR_MAX <= 0xffffffffUL)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif
#endif

/* Non-C99 case, still relying on DUK_UINTPTR_MAX, as long as it is not a computed value */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_UINTPTR_MAX) && !defined(DUK_UINTPTR_MAX_COMPUTED)
#if (DUK_UINTPTR_MAX <= 0xffffffffUL)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif
#endif

/* DUK_SIZE_MAX (= SIZE_MAX) is often reliable */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_SIZE_MAX) && !defined(DUK_SIZE_MAX_COMPUTED)
#if DUK_SIZE_MAX <= 0xffffffffUL
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif
#endif

/* M68K: packed always possible */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_M68K)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* With Emscripten, force unpacked duk_tval just to be safe, as it seems to
 * break at least on Firefox (probably IEEE double arithmetic is not 100%
 * supported, especially for NaNs).
 */
#if defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_EMSCRIPTEN)
#undef DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* Microsoft Visual Studio 2010 on x64 fails the above rules and tries to
 * use a packed type.  Force unpacked on x64 in general.
 */
#if defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_X64)
#undef DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* GCC/clang inaccurate math would break compliance and probably duk_tval,
 * so refuse to compile.  Relax this if -ffast-math is tested to work.
 */
#if defined(__FAST_MATH__)
#error __FAST_MATH__ defined, refusing to compile
#endif

/*
 *  Detection of double constants and math related functions.  Availability
 *  of constants and math functions is a significant porting concern.
 *
 *  INFINITY/HUGE_VAL is problematic on GCC-3.3: it causes an overflow warning
 *  and there is no pragma in GCC-3.3 to disable it.  Using __builtin_inf()
 *  avoids this problem for some reason.
 */

#define DUK_DOUBLE_2TO32     4294967296.0
#define DUK_DOUBLE_2TO31     2147483648.0

#undef DUK_USE_COMPUTED_INFINITY
#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION < 40600)
/* GCC older than 4.6: avoid overflow warnings related to using INFINITY */
#define DUK_DOUBLE_INFINITY  (__builtin_inf())
#elif defined(INFINITY)
#define DUK_DOUBLE_INFINITY  ((double) INFINITY)
#elif !defined(DUK_F_VBCC) && !defined(DUK_F_MSVC) && !defined(DUK_F_BCC)
#define DUK_DOUBLE_INFINITY  (1.0 / 0.0)
#else
/* In VBCC (1.0 / 0.0) results in a warning and 0.0 instead of infinity.
 * Use a computed infinity (initialized when a heap is created at the
 * latest).
 */
extern double duk_computed_infinity;
#define DUK_USE_COMPUTED_INFINITY
#define DUK_DOUBLE_INFINITY  duk_computed_infinity
#endif

#undef DUK_USE_COMPUTED_NAN
#if defined(NAN)
#define DUK_DOUBLE_NAN       NAN
#elif !defined(DUK_F_VBCC) && !defined(DUK_F_MSVC) && !defined(DUK_F_BCC)
#define DUK_DOUBLE_NAN       (0.0 / 0.0)
#else
/* In VBCC (0.0 / 0.0) results in a warning and 0.0 instead of NaN.
 * In MSVC (VS2010 Express) (0.0 / 0.0) results in a compile error.
 * Use a computed NaN (initialized when a heap is created at the
 * latest).
 */
extern double duk_computed_nan;
#define DUK_USE_COMPUTED_NAN
#define DUK_DOUBLE_NAN       duk_computed_nan
#endif

/* Many platforms are missing fpclassify() and friends, so use replacements
 * if necessary.  The replacement constants (FP_NAN etc) can be anything but
 * match Linux constants now.
 */
#undef DUK_USE_REPL_FPCLASSIFY
#undef DUK_USE_REPL_SIGNBIT
#undef DUK_USE_REPL_ISFINITE
#undef DUK_USE_REPL_ISNAN
#undef DUK_USE_REPL_ISINF

/* Complex condition broken into separate parts. */
#undef DUK_F_USE_REPL_ALL
#if !(defined(FP_NAN) && defined(FP_INFINITE) && defined(FP_ZERO) && \
      defined(FP_SUBNORMAL) && defined(FP_NORMAL))
/* Missing some obvious constants. */
#define DUK_F_USE_REPL_ALL
#elif defined(DUK_F_AMIGAOS) && defined(DUK_F_VBCC)
/* VBCC is missing the built-ins even in C99 mode (perhaps a header issue) */
#define DUK_F_USE_REPL_ALL
#elif defined(DUK_F_FREEBSD) && defined(DUK_F_CLANG)
/* Placeholder fix for (detection is wider than necessary):
 * http://llvm.org/bugs/show_bug.cgi?id=17788
 */
#define DUK_F_USE_REPL_ALL
#elif defined(DUK_F_UCLIBC)
/* At least some uclibc versions have broken floating point math.  For
 * example, fpclassify() can incorrectly classify certain NaN formats.
 * To be safe, use replacements.
 */
#define DUK_F_USE_REPL_ALL
#endif

#if defined(DUK_F_USE_REPL_ALL)
#define DUK_USE_REPL_FPCLASSIFY
#define DUK_USE_REPL_SIGNBIT
#define DUK_USE_REPL_ISFINITE
#define DUK_USE_REPL_ISNAN
#define DUK_USE_REPL_ISINF
#define DUK_FPCLASSIFY       duk_repl_fpclassify
#define DUK_SIGNBIT          duk_repl_signbit
#define DUK_ISFINITE         duk_repl_isfinite
#define DUK_ISNAN            duk_repl_isnan
#define DUK_ISINF            duk_repl_isinf
#define DUK_FP_NAN           0
#define DUK_FP_INFINITE      1
#define DUK_FP_ZERO          2
#define DUK_FP_SUBNORMAL     3
#define DUK_FP_NORMAL        4
#else
#define DUK_FPCLASSIFY       fpclassify
#define DUK_SIGNBIT          signbit
#define DUK_ISFINITE         isfinite
#define DUK_ISNAN            isnan
#define DUK_ISINF            isinf
#define DUK_FP_NAN           FP_NAN
#define DUK_FP_INFINITE      FP_INFINITE
#define DUK_FP_ZERO          FP_ZERO
#define DUK_FP_SUBNORMAL     FP_SUBNORMAL
#define DUK_FP_NORMAL        FP_NORMAL
#endif

#if defined(DUK_F_USE_REPL_ALL)
#undef DUK_F_USE_REPL_ALL
#endif

/* Some math functions are C99 only.  This is also an issue with some
 * embedded environments using uclibc where uclibc has been configured
 * not to provide some functions.  For now, use replacements whenever
 * using uclibc.
 */
#undef DUK_USE_MATH_FMIN
#undef DUK_USE_MATH_FMAX
#undef DUK_USE_MATH_ROUND
#if defined(DUK_F_UCLIBC)
/* uclibc may be missing these */
#elif defined(DUK_F_AMIGAOS) && defined(DUK_F_VBCC)
/* vbcc + AmigaOS may be missing these */
#elif !defined(DUK_F_C99) && !defined(DUK_F_CPP11)
/* build is not C99 or C++11, play it safe */
#else
/* C99 or C++11, no known issues */
#define DUK_USE_MATH_FMIN
#define DUK_USE_MATH_FMAX
#define DUK_USE_MATH_ROUND
#endif

/* These functions don't currently need replacement but are wrapped for
 * completeness.  Because these are used as function pointers, they need
 * to be defined as concrete C functions (not macros).
 */
#define DUK_FABS             fabs
#define DUK_FMIN             fmin
#define DUK_FMAX             fmax
#define DUK_FLOOR            floor
#define DUK_CEIL             ceil
#define DUK_FMOD             fmod
#define DUK_POW              pow
#define DUK_ACOS             acos
#define DUK_ASIN             asin
#define DUK_ATAN             atan
#define DUK_ATAN2            atan2
#define DUK_SIN              sin
#define DUK_COS              cos
#define DUK_TAN              tan
#define DUK_EXP              exp
#define DUK_LOG              log
#define DUK_SQRT             sqrt

/* NetBSD 6.0 x86 (at least) has a few problems with pow() semantics,
 * see test-bug-netbsd-math-pow.js.  Use NetBSD specific workaround.
 * (This might be a wider problem; if so, generalize the define name.)
 */
#undef DUK_USE_POW_NETBSD_WORKAROUND
#if defined(DUK_F_NETBSD)
#define DUK_USE_POW_NETBSD_WORKAROUND
#endif

/* Rely as little as possible on compiler behavior for NaN comparison,
 * signed zero handling, etc.  Currently never activated but may be needed
 * for broken compilers.
 */
#undef DUK_USE_PARANOID_MATH

/* There was a curious bug where test-bi-date-canceling.js would fail e.g.
 * on 64-bit Ubuntu, gcc-4.8.1, -m32, and no -std=c99.  Some date computations
 * using doubles would be optimized which then broke some corner case tests.
 * The problem goes away by adding 'volatile' to the datetime computations.
 * Not sure what the actual triggering conditions are, but using this on
 * non-C99 systems solves the known issues and has relatively little cost
 * on other platforms.  See bugs/issue-2e9d9c2d761dabaf8136c0897b91a270d1a47147.yaml.
 */
#undef DUK_USE_PARANOID_DATE_COMPUTATION
#if !defined(DUK_F_C99)
#define DUK_USE_PARANOID_DATE_COMPUTATION
#endif

/*
 *  ANSI C string/memory function wrapper defines to allow easier workarounds.
 *  Also convenience macros like DUK_MEMZERO which may be mapped to existing
 *  platform function to zero memory (like the deprecated bzero).
 *
 *  For instance, some platforms don't support zero-size memcpy correctly,
 *  some arcane uclibc versions have a buggy memcpy (but working memmove)
 *  and so on.  Such broken platforms can be dealt with here.
 *
 *  NOTE: ANSI C (various versions) and some implementations require that the
 *  pointer arguments to memset(), memcpy(), and memmove() be valid values
 *  even when byte size is 0 (even a NULL pointer is considered invalid in
 *  this context).  Zero-size operations as such are allowed, as long as their
 *  pointer arguments point to a valid memory area.  The DUK_MEMSET(),
 *  DUK_MEMCPY(), and DUK_MEMMOVE() macros require this same behavior, i.e.:
 *  (1) pointers must be valid and non-NULL, (2) zero size must otherwise be
 *  allowed.  If these are not fulfilled, a macro wrapper is needed.
 *
 *    http://stackoverflow.com/questions/5243012/is-it-guaranteed-to-be-safe-to-perform-memcpy0-0-0
 *    http://lists.cs.uiuc.edu/pipermail/llvmdev/2007-October/011065.html
 *
 *  Not sure what's the required behavior when a pointer points just past the
 *  end of a buffer, which often happens in practice (e.g. zero size memmoves).
 *  For example, if allocation size is 3, the following pointer would not
 *  technically point to a valid memory byte:
 *
 *    <-- alloc -->
 *    | 0 | 1 | 2 | .....
 *                  ^-- p=3, points after last valid byte (2)
 *
 *  If this is a practical issue, wrappers are again needed.
 */

typedef FILE duk_file;
#define DUK_STDIN       stdin
#define DUK_STDOUT      stdout
#define DUK_STDERR      stderr

/* Special naming to avoid conflict with e.g. DUK_FREE() in duk_heap.h
 * (which is unfortunately named).
 */
#define DUK_ANSI_MALLOC      malloc
#define DUK_ANSI_REALLOC     realloc
#define DUK_ANSI_CALLOC      calloc
#define DUK_ANSI_FREE        free

/* Old uclibcs have a broken memcpy so use memmove instead (this is overly
 * wide now on purpose):
 * http://lists.uclibc.org/pipermail/uclibc-cvs/2008-October/025511.html
 */
#if defined(DUK_F_UCLIBC)
#define DUK_MEMCPY       memmove
#else
#define DUK_MEMCPY       memcpy
#endif

#define DUK_MEMMOVE      memmove
#define DUK_MEMCMP       memcmp
#define DUK_MEMSET       memset
#define DUK_STRLEN       strlen
#define DUK_STRCMP       strcmp
#define DUK_STRNCMP      strncmp
#define DUK_PRINTF       printf
#define DUK_FPRINTF      fprintf
#define DUK_SPRINTF      sprintf

#if defined(DUK_F_MSVC)
/* _snprintf() does NOT NUL terminate on truncation, but Duktape code never
 * assumes that.
 * http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
 */
#define DUK_SNPRINTF     _snprintf
#else
#define DUK_SNPRINTF     snprintf
#endif

#define DUK_VSPRINTF     vsprintf

#if defined(DUK_F_MSVC)
#if (_MSC_VER < 1600)
/* Older MSVC version are missing vsnprintf() but have _vsnprintf(). */
#define DUK_VSNPRINTF    _vsnprintf
#else
#define DUK_VSNPRINTF    vsnprintf
#endif
#else
#define DUK_VSNPRINTF    vsnprintf
#endif  /* DUK_F_MSVC */

#define DUK_SSCANF       sscanf
#define DUK_VSSCANF      vsscanf
#define DUK_FOPEN        fopen
#define DUK_FCLOSE       fclose
#define DUK_FREAD        fread
#define DUK_FWRITE       fwrite
#define DUK_FSEEK        fseek
#define DUK_FTELL        ftell
#define DUK_FFLUSH       fflush
#define DUK_FPUTC        fputc

#define DUK_MEMZERO(p,n) \
	DUK_MEMSET((p), 0, (n))

/*
 *  Avoiding platform function pointers.
 *
 *  On some platforms built-in functions may be implemented as macros or
 *  inline functions, so they can't be necessarily addressed by function
 *  pointers.  This is certainly the case with some platform "polyfills"
 *  which provide missing C99/C++11 functions through macros, and may be
 *  the case with VS2013 (see GH-17).
 */

/* This is now the default: the cost in footprint is negligible. */
#define DUK_USE_AVOID_PLATFORM_FUNCPTRS

/*
 *  Vararg macro wrappers.  We need va_copy() which is defined in C99 / C++11,
 *  so an awkward replacement is needed for pre-C99 / pre-C++11 environments.
 *  This will quite likely need portability hacks for some non-C99 environments.
 */

#if defined(DUK_F_C99) || defined(DUK_F_CPP11)
/* C99 / C++11 and above: rely on va_copy() which is required.
 * Omit parenthesis on macro right side on purpose to minimize differences
 * to direct use.
 */
#define DUK_VA_COPY(dest,src) va_copy(dest,src)
#elif defined(DUK_F_GCC) || defined(DUK_F_CLANG)
/* GCC: assume we have __va_copy() in non-C99 mode, which should be correct
 * for even quite old GCC versions.  Clang matches GCC behavior.
 */
#define DUK_VA_COPY(dest,src) __va_copy(dest,src)
#else
/* Pre-C99: va_list type is implementation dependent.  This replacement
 * assumes it is a plain value so that a simple assignment will work.
 * This is not the case on all platforms (it may be a single-array element,
 * for instance).
 */
#define DUK_VA_COPY(dest,src) do { (dest) = (src); } while (0)
#endif

/*
 *  Miscellaneous ANSI C or other platform wrappers.
 */

#define DUK_ABORT        abort
#define DUK_EXIT         exit

/*
 *  Macro hackery to convert e.g. __LINE__ to a string without formatting,
 *  see: http://stackoverflow.com/questions/240353/convert-a-preprocessor-token-to-a-string
 */

#define DUK_F_STRINGIFY_HELPER(x)  #x
#define DUK_MACRO_STRINGIFY(x)  DUK_F_STRINGIFY_HELPER(x)

/*
 *  Cause segfault macro.
 *
 *  This is optionally used by panic handling to cause the program to segfault
 *  (instead of e.g. abort()) on panic.  Valgrind will then indicate the C
 *  call stack leading to the panic.
 */

#define DUK_CAUSE_SEGFAULT()  do { \
		*((volatile duk_uint32_t *) NULL) = (duk_uint32_t) 0xdeadbeefUL; \
	} while (0)

/*
 *  Macro for suppressing warnings for potentially unreferenced variables.
 *  The variables can be actually unreferenced or unreferenced in some
 *  specific cases only; for instance, if a variable is only debug printed,
 *  it is unreferenced when debug printing is disabled.
 *
 *  (Introduced here because it's potentially compiler specific.)
 */

#define DUK_UNREF(x)  do { \
		(void) (x); \
	} while (0)

/*
 *  DUK_NORETURN: macro for declaring a 'noreturn' function.
 *  Unfortunately the noreturn declaration may appear in various
 *  places of a function declaration, so the solution is to wrap
 *  the entire declaration inside the macro.  Compiler support
 *  for using a noreturn declaration on function pointers varies;
 *  this macro must only be used for actual function declarations.
 *
 *  http://gcc.gnu.org/onlinedocs/gcc-4.3.2//gcc/Function-Attributes.html
 *  http://clang.llvm.org/docs/LanguageExtensions.html
 */

#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION >= 20500L)
/* since gcc-2.5 */
#define DUK_NORETURN(decl)  decl __attribute__((noreturn))
#elif defined(__clang__)
/* syntax same as gcc */
#define DUK_NORETURN(decl)  decl __attribute__((noreturn))
#elif defined(DUK_F_MSVC)
/* http://msdn.microsoft.com/en-us/library/aa235362(VS.60).aspx */
#define DUK_NORETURN(decl)  __declspec(noreturn) decl
#else
/* Don't know how to declare a noreturn function, so don't do it; this
 * may cause some spurious compilation warnings (e.g. "variable used
 * uninitialized").
 */
#define DUK_NORETURN(decl)  decl
#endif

/*
 *  Macro for stating that a certain line cannot be reached.
 *
 *  http://gcc.gnu.org/onlinedocs/gcc-4.5.0/gcc/Other-Builtins.html#Other-Builtins
 *  http://clang.llvm.org/docs/LanguageExtensions.html
 */

#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION >= 40500L)
/* since gcc-4.5 */
#define DUK_UNREACHABLE()  do { __builtin_unreachable(); } while(0)
#elif defined(__clang__) && defined(__has_builtin)
#if __has_builtin(__builtin_unreachable)
/* same as gcc */
#define DUK_UNREACHABLE()  do { __builtin_unreachable(); } while(0)
#endif
#else
/* unknown */
#endif

#if !defined(DUK_UNREACHABLE)
/* Don't know how to declare unreachable point, so don't do it; this
 * may cause some spurious compilation warnings (e.g. "variable used
 * uninitialized").
 */
#define DUK_UNREACHABLE()  /* unreachable */
#endif

/*
 *  Likely and unlikely branches.  Using these is not at all a clear cut case,
 *  so the selection is a two-step process: (1) DUK_USE_BRANCH_HINTS is set
 *  if the architecture, compiler etc make it useful to use the hints, and (2)
 *  a separate check determines how to do them.
 *
 *  These macros expect the argument to be a relational expression with an
 *  integer value.  If used with pointers, you should use an explicit check
 *  like:
 *
 *    if (DUK_LIKELY(ptr != NULL)) { ... }
 *
 *  instead of:
 *
 *    if (DUK_LIKELY(ptr)) { ... }
 *
 *  http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html  (__builtin_expect)
 */

/* pretty much a placeholder now */
#if defined(DUK_F_GCC)
#define DUK_USE_BRANCH_HINTS
#elif defined(DUK_F_CLANG)
#define DUK_USE_BRANCH_HINTS
#else
#undef DUK_USE_BRANCH_HINTS
#endif

#if defined(DUK_USE_BRANCH_HINTS)
#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION >= 40500L)
/* GCC: test not very accurate; enable only in relatively recent builds
 * because of bugs in gcc-4.4 (http://lists.debian.org/debian-gcc/2010/04/msg00000.html)
 */
#define DUK_LIKELY(x)    __builtin_expect((x), 1)
#define DUK_UNLIKELY(x)  __builtin_expect((x), 0)
#elif defined(DUK_F_CLANG)
#define DUK_LIKELY(x)    __builtin_expect((x), 1)
#define DUK_UNLIKELY(x)  __builtin_expect((x), 0)
#endif
#endif  /* DUK_USE_BRANCH_HINTS */

#if !defined(DUK_LIKELY)
#define DUK_LIKELY(x)    (x)
#endif
#if !defined(DUK_UNLIKELY)
#define DUK_UNLIKELY(x)  (x)
#endif

/*
 *  Function inlining control
 *
 *  DUK_NOINLINE: avoid inlining a function.
 *  DUK_INLINE: suggest inlining a function.
 *  DUK_ALWAYS_INLINE: force inlining for critical functions.
 */

#if defined(DUK_F_CLANG)
#define DUK_NOINLINE        __attribute__((noinline))
#define DUK_INLINE          inline
#define DUK_ALWAYS_INLINE   inline __attribute__((always_inline))
#elif defined(DUK_F_GCC) && defined(DUK_F_GCC_VERSION)
#if (DUK_F_GCC_VERSION >= 30101)
#define DUK_NOINLINE        __attribute__((noinline))
#define DUK_INLINE          inline
#define DUK_ALWAYS_INLINE   inline __attribute__((always_inline))
#endif
#endif

#if !defined(DUK_NOINLINE)
#define DUK_NOINLINE       /*nop*/
#define DUK_INLINE         /*nop*/
#define DUK_ALWAYS_INLINE  /*nop*/
#endif

/*
 *  Symbol visibility macros
 *
 *  To avoid C++ declaration issues (see GH-63):
 *
 *    - Don't use DUK_LOCAL_DECL for local -data symbols- so that you don't
 *      end up with both a "static" declaration and a definition.
 *
 *    - Wrap any DUK_INTERNAL_DECL with a '#if !defined(DUK_SINGLE_FILE)'
 *      so that the internal declarations (which would map to "static" in
 *      a single file distribution) get dropped.
 */

/* XXX: user override for these? user override for just using the default visibility macros? */
/* XXX: separate macros for function and data may be necessary at some point. */

#if defined(DUK_F_GCC_VERSION)
#if (DUK_F_GCC_VERSION >= 40000) && !(defined(DUK_F_MINGW) || defined(DUK_F_CYGWIN))
/* Might work on earlier versions too but limit to GCC 4+.
 * MinGW should use Windows specific __declspec or no visibility attributes at all,
 * otherwise: "warning: visibility attribute not supported in this configuration; ignored".
 * Same applies to Cygwin GCC.
 */
#define DUK_F_GCC_SYMBOL_VISIBILITY
#endif
#endif
#if defined(DUK_F_CLANG) && !defined(DUK_F_GCC_SYMBOL_VISIBILITY)
#define DUK_F_GCC_SYMBOL_VISIBILITY
#endif
#if defined(DUK_OPT_DLL_BUILD) && defined(_WIN32) && (defined(_MSC_VER) || defined(__GNUC__))
/* __declspec(dllexport) and __declspec(dllimport) only for Windows DLL build.
 * MSVC: any minimum version?
 * MinGW: no minimum version, even gcc-2.95.3 supported dllimport/dllexport.
*/
#define DUK_F_MSVC_DLL_SYMBOL_VISIBILITY
#endif

#if defined(DUK_F_GCC_SYMBOL_VISIBILITY)
/* GCC 4+ visibility attributes. */
#define DUK_EXTERNAL_DECL  __attribute__ ((visibility("default"))) extern
#define DUK_EXTERNAL       __attribute__ ((visibility("default")))
#if defined(DUK_SINGLE_FILE)
#define DUK_INTERNAL_DECL  static
#define DUK_INTERNAL       static
#else
#define DUK_INTERNAL_DECL  __attribute__ ((visibility("hidden"))) extern
#define DUK_INTERNAL       __attribute__ ((visibility("hidden")))
#endif
#elif defined(DUK_F_MSVC_DLL_SYMBOL_VISIBILITY)
/* MSVC dllexport/dllimport: appropriate __declspec depends on whether we're
 * compiling Duktape or the application.
 */
#if defined(DUK_COMPILING_DUKTAPE)
#define DUK_EXTERNAL_DECL  extern __declspec(dllexport)
#define DUK_EXTERNAL       __declspec(dllexport)
#else
#define DUK_EXTERNAL_DECL  extern __declspec(dllimport)
#define DUK_EXTERNAL       should_not_happen
#endif
#if defined(DUK_SINGLE_FILE)
#define DUK_INTERNAL_DECL  static
#define DUK_INTERNAL       static
#else
#define DUK_INTERNAL_DECL  extern
#define DUK_INTERNAL       /*empty*/
#endif
#else
/* Default visibility. */
#define DUK_EXTERNAL_DECL  extern
#define DUK_EXTERNAL       /*empty*/
#if defined(DUK_SINGLE_FILE)
#define DUK_INTERNAL_DECL  static
#define DUK_INTERNAL       static
#else  /* DUK_SINGLE_FILE */
#define DUK_INTERNAL_DECL  extern
#define DUK_INTERNAL       /*empty*/
#endif
#endif

/* For now, these are shared. */
#define DUK_LOCAL_DECL     static
#define DUK_LOCAL          static

/*
 *  __FILE__, __LINE__, __func__ are wrapped.  Especially __func__ is a
 *  problem because it is not available even in some compilers which try
 *  to be C99 compatible (e.g. VBCC with -c99 option).
 */

#define DUK_FILE_MACRO  __FILE__

#define DUK_LINE_MACRO  __LINE__

#if !defined(DUK_F_VBCC) && !defined(DUK_F_MSVC)
#define DUK_FUNC_MACRO  __func__
#else
#define DUK_FUNC_MACRO  "unknown"
#endif

/*
 *  Byteswap macros
 *
 *  These are here so that inline assembly or other platform functions can be
 *  used if available.
 */

#define DUK_BSWAP32(x) \
	((((duk_uint32_t) (x)) >> 24) | \
	 ((((duk_uint32_t) (x)) >> 8) & 0xff00UL) | \
	 ((((duk_uint32_t) (x)) << 8) & 0xff0000UL) | \
	 (((duk_uint32_t) (x)) << 24))

#define DUK_BSWAP16(x) \
	((duk_uint16_t) (x) >> 8) | \
	((duk_uint16_t) (x) << 8)

/*
 *  Architecture string, human readable value exposed in Duktape.env
 */

#if defined(DUK_F_X86)
#define DUK_USE_ARCH_STRING "x86"
#elif defined(DUK_F_X32)
#define DUK_USE_ARCH_STRING "x32"
#elif defined(DUK_F_X64)
#define DUK_USE_ARCH_STRING "x64"
#elif defined(DUK_F_ARM)
#define DUK_USE_ARCH_STRING "arm"
#elif defined(DUK_F_MIPS32)
#define DUK_USE_ARCH_STRING "mips32"
#elif defined(DUK_F_MIPS64)
#define DUK_USE_ARCH_STRING "mips64"
#elif defined(DUK_F_SUPERH)
#define DUK_USE_ARCH_STRING "sh"
#elif defined(DUK_F_M68K)
#define DUK_USE_ARCH_STRING "m68k"
#elif defined(DUK_F_FLASHPLAYER)
#define DUK_USE_ARCH_STRING "flashplayer"
#elif defined(DUK_F_EMSCRIPTEN)
#define DUK_USE_ARCH_STRING "emscripten"
#else
#define DUK_USE_ARCH_S*   @0@@Ø              ö *   @0@@Ø              Hö 3   @0@@Ø              Äö *   @0@@Ø              ∞ö 3   @0@@Ø              Ëö 3   @0@@Ø               õ *   @0@@Ø              Põ .   @0@@Ø              Äõ 3   @0@@Ø              ∏õ *   @0@@Ø              Ëõ 3   @0@@Ø               ú *   @0@@Ø              Pú 3   @0@@Ø              àú 3   @0@@Ø              ¿ú *   @0@@Ø              ú 3   @0@@Ø              (ù *   @0@@Ø              Xù 3   @0@@Ø              êù *   @0@@Ø              ¿ù 3   @0@@Ø              ¯ù 3   @0@@Ø              0û 3   @0@@Ø              hû 3   @0@@Ø              †û 3   @0@@Ø              ÿû *   @0@@Ø              ü *   @0@@Ø              8ü *   @0@@Ø              hü 3   @0@@Ø              †ü 3   @0@@Ø              ÿü *   @0@@Ø              † 1   @0@@Ø              @† 3   @0@@Ø              x† 5   @0@@Ø              ∞† *   @0@@Ø              ‡† 3   @0@@Ø              ° 9   @0@@Ø              X° *   @0@@Ø              à° <   @0@@Ø              »° *   @0@@Ø              ¯° 6   @0@@Ø              0¢ 3   @0@@Ø              h¢ 3   @0@@Ø              †¢ 6   @0@@Ø              ÿ¢ 3   @0@@Ø              £ 3   @0@@Ø              H£ 3   @0@@Ø              Ä£ 3   @0@@Ø              ∏£ 3   @0@@Ø              £ 3   @0@@Ø              (§ *   @0@@Ø              X§ 3   @0@@Ø              ê§ *   @0@@Ø              ¿§ *   @0@@Ø              § *   @0@@Ø               • 3   @0@@Ø              X• 3   @0@@Ø              ê• *   @0@@Ø              ¿• *   @0@@Ø              • *   @0@@Ø               ¶ *   @0@@Ø              P¶ *   @0@@Ø              Ä¶ *   @0@@Ø              ∞¶ 3   @0@@Ø              Ë¶ 3   @0@@Ø               ß *   @0@@Ø              Pß *   @0@@Ø              Äß 3   @0@@Ø              ∏ß *   @0@@Ø              Ëß 3   @0@@Ø               ® *   @0@@Ø              P® *   @0@@Ø              Ä® ;   @0@@Ø              ¿® 3   @0@@Ø              ¯® )   @0@@Ø              (© 1   @0@@Ø              `© 3   @0@@Ø              ò© *   @0@@Ø              »© 3   @0@@Ø               ™ <   @0@@Ø              @™ 3   @0@@Ø              x™ *   @0@@Ø              ®™ 3   @0@@Ø              ‡™ *   @0@@Ø              ´ *   @0@@Ø              @´ *   @0@@Ø              p´ 3   @0@@Ø              ®´ 3   @0@@Ø              ‡´ $   @0@@Ø              ¨ 4   @0@@Ø              @¨ 3   @0@@Ø              x¨ *   @0@@Ø              ®¨ *   @0@@Ø              ÿ¨ 3   @0@@Ø              ≠ *   @0@@Ø              @≠ *   @0@@Ø              p≠ 3   @0@@Ø              ®≠ %   @0@@Ø              –≠     @0@@Ø              ≠ *   @0@@Ø               Æ *   @0@@Ø              PÆ *   @0@@Ø              ÄÆ *   @0@@Ø              ∞Æ 1   @0@@Ø              ËÆ *   @0@@Ø              Ø 3   @0@@Ø              PØ *   @0@@Ø              ÄØ /   @0@@Ø              ∞Ø *   @0@@Ø              ‡Ø 3   @0@@Ø              ∞ *   @0@@Ø              H∞ 3   @0@@Ø              Ä∞ 3   @0@@Ø              ∏∞ 3   @0@@Ø              ∞ *   @0@@Ø               ± *   @0@@Ø              P± 3   @0@@Ø              à± *   @0@@Ø              ∏± 3   @0@@Ø              ± 3   @0@@Ø              (≤ *   @0@@Ø              X≤ *   @0@@Ø              à≤ 3   @0@@Ø              ¿≤ ;   @0@@Ø               ≥ *   @0@@Ø              0≥ *   @0@@Ø              `≥ *   @0@@Ø              ê≥ 3   @0@@Ø              »≥ *   @0@@Ø              ¯≥ *   @0@@Ø              (¥ 3   @0@@Ø              `¥ 3   @0@@Ø              ò¥ *   @0@@Ø              »¥ 3   @0@@Ø               µ *   @0@@Ø              0µ 3   @0@@Ø              hµ *   @0@@Ø              òµ 3   @0@@Ø              –µ 9   @0@@Ø              ∂ 3   @0@@Ø              H∂ *   @0@@Ø              x∂ 1   @0@@Ø              ∞∂ *   @0@@Ø              ‡∂ *   @0@@Ø              ∑ 3   @0@@Ø              H∑ 7   @0@@Ø              Ä∑ *   @0@@Ø              ∞∑ 3   @0@@Ø              Ë∑ *   @0@@Ø              ∏ 3   @0@@Ø              P∏ 3   @0@@Ø              à∏ 3   @0@@Ø              ¿∏ 3   @0@@Ø              ¯∏ 3   @0@@Ø              0π 3   @0@@Ø              hπ 2   @0@@Ø              †π *   @0@@Ø              –π 3   @0@@Ø              ∫ 6   @0@@Ø              @∫ *   @0@@Ø              p∫ 3   @0@@Ø              ®∫ *   @0@@Ø              ÿ∫ :   @0@@Ø              ª *   @0@@Ø              Hª 3   @0@@Ø              Äª *   @0@@Ø              ∞ª *   @0@@Ø              ‡ª *   @0@@Ø              º *   @0@@Ø              @º 3   @0@@Ø              xº -   @0@@Ø              ®º 3   @0@@Ø              ‡º *   @0@@Ø              Ω *   @0@@Ø              @Ω *   @0@@Ø              pΩ 3   @0@@Ø              ®Ω *   @0@@Ø              ÿΩ *   @0@@Ø              æ 3   @0@@Ø              @æ *   @0@@Ø              pæ *   @0@@Ø              †æ 3   @0@@Ø              ÿæ ;   @0@@Ø              ø 4   @0@@Ø              Pø %   @0@@Ø              xø %   @0@@Ø              †ø 3   @0@@Ø              ÿø 3   @0@@Ø              ¿ *   @0@@Ø              @¿ 3   @0@@Ø              x¿ *   @0@@Ø              ®¿ 3   @0@@Ø              ‡¿ *   @0@@Ø              ¡ *   @0@@Ø              @¡ *   @0@@Ø              p¡ 3   @0@@Ø              ®¡ *   @0@@Ø              ÿ¡ *   @0@@Ø              ¬ +   @0@@Ø              8¬ :   @0@@Ø              x¬ )   @0@@Ø              ®¬ *   @0@@Ø              ÿ¬ 3   @0@@Ø              √ *   @0@@Ø              @√ #   @0@@Ø              h√ %   @0@@Ø              ê√ *   @0@@Ø              ¿√ 3   @0@@Ø              ¯√ 3   @0@@Ø              0ƒ *   @0@@Ø              `ƒ 3   @0@@Ø              òƒ *   @0@@Ø              »ƒ *   @0@@Ø              ¯ƒ *   @0@@Ø              (≈ #   @0@@Ø              P≈ *   @0@@Ø              Ä≈ 3   @0@@Ø              ∏≈ *   @0@@Ø              Ë≈ 1   @0@@Ø               ∆ 3   @0@@Ø              X∆ *   @0@@Ø              à∆ *   @0@@Ø              ∏∆ +   @0@@Ø              Ë∆ 3   @0@@Ø               « 3   @0@@Ø              X« *   @0@@Ø              à« 3   @0@@Ø              ¿« *   @0@@Ø              « *   @0@@Ø               » *   @0@@Ø              P» 3   @0@@Ø              à» 3   @0@@Ø              ¿» 3   @0@@Ø              ¯» 3   @0@@Ø              0… *   @0@@Ø              `… *   @0@@Ø              ê… 3   @0@@Ø              »… *   @0@@Ø              ¯… 3   @0@@Ø              0  3   @0@@Ø              h  *   @0@@Ø              ò  3   @0@@Ø              –  *   @0@@Ø               À    @0@@Ø               À 3   @0@@Ø              XÀ 3   @0@@Ø              êÀ 3   @0@@Ø              »À 3   @0@@Ø               Ã 3   @0@@Ø              8Ã *   @0@@Ø              hÃ *   @0@@Ø              òÃ +   @0@@Ø              »Ã *   @0@@Ø              ¯Ã *   @0@@Ø              (Õ *   @0@@Ø              XÕ *   @0@@Ø              àÕ 3   @0@@Ø              ¿Õ 6   @0@@Ø               Œ C   @0P@Ø              HŒ 9   @0@@Ø              àŒ *   @0@@Ø              ∏Œ 3   @0@@Ø              Œ *   @0@@Ø               œ *   @0@@Ø              Pœ *   @0@@Ø              Äœ *   @0@@Ø              ∞œ *   @0@@Ø              ‡œ *   @0@@Ø              – 3   @0@@Ø              H– *   @0@@Ø              x– 3   @0@@Ø              ∞– *   @0@@Ø              ‡– *   @0@@Ø              — %   @0@@Ø              8— *   @0@@Ø              h— 3   @0@@Ø              †—    @0@@Ø              ¿— *   @0@@Ø              — /   @0@@Ø               “ *   @0@@Ø              P“ O   @0P@Ø              †“ 3   @0@@Ø              ÿ“ 3   @0@@Ø              ” 3   @0@@Ø              H” 3   @0@@Ø              Ä” 7   @0@@Ø              ∏” 3   @0@@Ø              ” *   @0@@Ø               ‘ 3   @0@@Ø              X‘ 3   @0@@Ø              ê‘ <   @0@@Ø              –‘     @0@@Ø              ‘ 3   @0@@Ø              (’ 3   @0@@Ø              `’ *   @0@@Ø              ê’ 3   @0@@Ø              »’ *   @0@@Ø              ¯’ 4   @0@@Ø              0÷ *   @0@@Ø              `÷ 3   @0@@Ø              ò÷ 3   @0@@Ø              –÷ *   @0@@Ø               ◊ 3   @0@@Ø              8◊ %   @0@@Ø              `◊ *   @0@@Ø              ê◊ 3   @0@@Ø              »◊ *   @0@@Ø              ¯◊ *   @0@@Ø              (ÿ 3   @0@@Ø              `ÿ 3   @0@@Ø              òÿ %   @0@@Ø              ¿ÿ *   @0@@Ø              ÿ 3   @0@@Ø              (Ÿ 3   @0@@Ø              `Ÿ *   @0@@Ø              êŸ *   @0@@Ø              ¿Ÿ 3   @0@@Ø              ¯Ÿ :   @0@@Ø              8⁄ *   @0@@Ø              h⁄ *   @0@@Ø              ò⁄ *   @0@@Ø              »⁄ *   @0@@Ø              ¯⁄ 3   @0@@Ø              0€ *   @0@@Ø              `€ *   @0@@Ø              ê€ *   @0@@Ø              ¿€ *   @0@@Ø              € *   @0@@Ø               ‹ *   @0@@Ø              P‹ *   @0@@Ø              Ä‹ 3   @0@@Ø              ∏‹ *   @0@@Ø              Ë‹ 4   @0@@Ø               › 3   @0@@Ø              X› 3   @0@@Ø              ê› 3   @0@@Ø              –› @   @0P@Ø              ﬁ 3   @0@@Ø              Hﬁ +   @0@@Ø              xﬁ 3   @0@@Ø              ∞ﬁ 3   @0@@Ø              Ëﬁ +   @0@@Ø              ﬂ *   @0@@Ø              Hﬂ :   @0@@Ø              àﬂ *   @0@@Ø              ∏ﬂ 3   @0@@Ø              ﬂ 3   @0@@Ø              (‡ *   @0@@Ø              X‡ 3   @0@@Ø              ê‡ *   @0@@Ø              ¿‡ *   @0@@Ø              ‡ *   @0@@Ø               · *   @0@@Ø              P· *   @0@@Ø              Ä· *   @0@@Ø              ∞· *   @0@@Ø              ‡· *   @0@@Ø              ‚ *   @0@@Ø              @‚ :   @0@@Ø              Ä‚ *   @0@@Ø              ∞‚ 2   @0@@Ø              Ë‚ 2   @0@@Ø               „ 3   @0@@Ø              X„ 3   @0@@Ø              ê„ *   @0@@Ø              ¿„ 3   @0@@Ø              ¯„ 3   @0@@Ø              0‰ 3   @0@@Ø              h‰ *   @0@@Ø              ò‰ 3   @0@@Ø              –‰ *   @0@@Ø               Â *   @0@@Ø              0Â 3   @0@@Ø              hÂ *   @0@@Ø              òÂ *   @0@@Ø              »Â *   @0@@Ø               Ê H   @0P@Ø              HÊ 3   @0@@Ø              ÄÊ 3   @0@@Ø              ∏Ê *   @0@@Ø              ËÊ *   @0@@Ø              Á 3   @0@@Ø              PÁ 3   @0@@Ø              àÁ *   @0@@Ø              ∏Á 3   @0@@Ø              Á 3   @0@@Ø              (Ë *   @0@@Ø              XË #   @0@@Ø              ÄË *   @0@@Ø              ∞Ë 3   @0@@Ø              ËË ,   @0@@Ø              È 3   @0@@Ø              PÈ *   @0@@Ø              ÄÈ 3   @0@@Ø              ∏È *   @0@@Ø              ËÈ 3   @0@@Ø               Í (   @0@@Ø              HÍ *   @0@@Ø              xÍ *   @0@@Ø              ®Í %   @0@@Ø              –Í *   @0@@Ø               Î *   @0@@Ø              0Î +   @0@@Ø              `Î 1   @0@@Ø              òÎ 3   @0@@Ø              –Î -   @0@@Ø               Ï 3   @0@@Ø              8Ï *   @0@@Ø              hÏ )   @0@@Ø              òÏ *   @0@@Ø              »Ï 3   @0@@Ø               Ì 3   @0@@Ø              8Ì %   @0@@Ø              `Ì 3   @0@@Ø              òÌ    @0@@Ø              ∏Ì 3   @0@@Ø              Ì <   @0@@Ø              0Ó 3   @0@@Ø              hÓ 3   @0@@Ø              †Ó *   @0@@Ø              –Ó *   @0@@Ø               Ô &   @0@@Ø              (Ô *   @0@@Ø              XÔ 0   @0@@Ø              àÔ 3   @0@@Ø              ¿Ô 1   @0@@Ø              ¯Ô 3   @0@@Ø              0 *   @0@@Ø              `    @0@@Ø              Ä 1   @0@@Ø              ∏ 3   @0@@Ø               3   @0@@Ø              (Ò 3   @0@@Ø              `Ò '   @0@@Ø              àÒ *   @0@@Ø              ∏Ò *   @0@@Ø              ËÒ 3   @0@@Ø               Ú *   @0@@Ø              PÚ *   @0@@Ø              ÄÚ 3   @0@@Ø              ¿Ú G   @0P@Ø              Û 3   @0@@Ø              @Û 3   @0@@Ø              xÛ *   @0@@Ø              ®Û *   @0@@Ø              ÿÛ 3   @0@@Ø              Ù *   @0@@Ø              @Ù *   @0@@Ø              pÙ 3   @0@@Ø              ®Ù *   @0@@Ø              ÿÙ *   @0@@Ø              ı *   @0@@Ø              8ı *   @0@@Ø              hı 3   @0@@Ø              †ı *   @0@@Ø              –ı *   @0@@Ø               ˆ 3   @0@@Ø              8ˆ 3   @0@@Ø              pˆ *   @0@@Ø              †ˆ *   @0@@Ø              –ˆ 3   @0@@Ø              ˜ .   @0@@Ø              8˜ *   @0@@Ø              h˜ *   @0@@Ø              ò˜ 1   @0@@Ø              –˜ *   @0@@Ø               ¯ *   @0@@Ø              0¯ *   @0@@Ø              `¯ 3   @0@@Ø              †¯ C   @0P@Ø              Ë¯ 3   @0@@Ø               ˘ *   @0@@Ø              P˘ 3   @0@@Ø              à˘ 7   @0@@Ø              ¿˘ 3   @0@@Ø              ¯˘ 3   @0@@Ø              0˙ *   @0@@Ø              `˙ *   @0@@Ø              ê˙ 3   @0@@Ø              »˙ 1   @0@@Ø               ˚ *   @0@@Ø              0˚ "   @0@@Ø              X˚ 3   @0@@Ø              ê˚ 3   @0@@Ø              »˚ 1   @0@@Ø               ¸ 3   @0@@Ø              8¸ 3   @0@@Ø              p¸ 3   @0@@Ø              ®¸ *   @0@@Ø              ÿ¸ 3   @0@@Ø              ˝ 3   @0@@Ø              H˝ *   @0@@Ø              x˝ :   @0@@Ø              ∏˝ *   @0@@Ø              Ë˝ 3   @0@@Ø               ˛ 3   @0@@Ø              X˛ 3   @0@@Ø              ê˛ *   @0@@Ø              ¿˛ *   @0@@Ø              ˛ *   @0@@Ø               ˇ 3   @0@@Ø              Xˇ 3   @0@@Ø              êˇ *   @0@@Ø              ¿ˇ +   @0@@Ø              ˇ 3   @0@@Ø              (  3   @0@@Ø              `  3   @0@@Ø              ò  3   @0@@Ø              –  *   @0@@Ø                *   @0@@Ø              0 3   @0@@Ø              h 7   @0@@Ø              † *   @0@@Ø              – 3   @0@@Ø               *   @0@@Ø              8 *   @0@@Ø              h 3   @0@@Ø              † 3   @0@@Ø              ÿ 3   @0@@Ø               3   @0@@Ø              H *   @0@@Ø              x *   @0@@Ø              ® 3   @0@@Ø              ‡ 3   @0@@Ø               *   @0@@Ø              H 3   @0@@Ø              Ä *   @0@@Ø              ∞ *   @0@@Ø              ‡ 3   @0@@Ø               *   @0@@Ø              H :   @0@@Ø              à *   @0@@Ø              ∏ *   @0@@Ø              Ë 3   @0@@Ø                3   @0@@Ø              X *   @0@@Ø              à 3   @0@@Ø              ¿ *   @0@@Ø               3   @0@@Ø              ( *   @0@@Ø              X 3   @0@@Ø              ê &   @0@@Ø              ∏ 3   @0@@Ø               *   @0@@Ø                3   @0@@Ø              X *   @0@@Ø              à <   @0@@Ø              – D   @0P@Ø              	 *   @0@@Ø              H	 3   @0@@Ø              Ä	 3   @0@@Ø              ∏	 *   @0@@Ø              Ë	 <   @0@@Ø              (
 3   @0@@Ø              `
 *   @0@@Ø              ê
 *   @0@@Ø              ¿
 *   @0@@Ø              
 *   @0@@Ø                3   @0@@Ø              X *   @0@@Ø              à *   @0@@Ø              ∏ 3   @0@@Ø               -   @0@@Ø                *   @0@@Ø              P *   @0@@Ø              Ä C   @0P@Ø              » 3   @0@@Ø                3   @0@@Ø              8 >   @0@@Ø              x *   @0@@Ø              ® *   @0@@Ø              ÿ 3   @0@@Ø               3   @0@@Ø              H 3   @0@@Ø              Ä *   @0@@Ø              ∞ 3   @0@@Ø              Ë 3   @0@@Ø                *   @0@@Ø              P *   @0@@Ø              Ä *   @0@@Ø              ∞ *   @0@@Ø              ‡ 3   @0@@Ø               0   @0@@Ø              H *   @0@@Ø              x *   @0@@Ø              ® 3   @0@@Ø              ‡ 3   @0@@Ø               *   @0@@Ø              H 3   @0@@Ø              Ä *   @0@@Ø              ∞ *   @0@@Ø              ‡ *   @0@@Ø               *   @0@@Ø              @ *   @0@@Ø              p 3   @0@@Ø              ® 3   @0@@Ø              ‡ 3   @0@@Ø               3   @0@@Ø              P 3   @0@@Ø              à *   @0@@Ø              ∏ 3   @0@@Ø               1   @0@@Ø              ( *   @0@@Ø              X 3   @0@@Ø              ê *   @0@@Ø              ¿ 3   @0@@Ø              ¯ 3   @0@@Ø              0 3   @0@@Ø              h *   @0@@Ø              ò :   @0@@Ø              ÿ *   @0@@Ø               *   @0@@Ø              8 ,   @0@@Ø              h *   @0@@Ø              ò ;   @0@@Ø              ÿ 3   @0@@Ø               *   @0@@Ø              @ 3   @0@@Ø              x 3   @0@@Ø              ∞ *   @0@@Ø              ‡ 3   @0@@Ø               4   @0@@Ø              P 7   @0@@Ø              à 3   @0@@Ø              ¿ *   @0@@Ø               *   @0@@Ø                3   @0@@Ø              X 3   @0@@Ø              ê .   @0@@Ø              ¿ 3   @0@@Ø              ¯ *   @0@@Ø              ( 3   @0@@Ø              ` 3   @0@@Ø              ò 7   @0@@Ø              – 3   @0@@Ø               3   @0@@Ø              @ 3   @0@@Ø              x *   @0@@Ø              ® *   @0@@Ø              ÿ *   @0@@Ø               .   @0@@Ø              8 3   @0@@Ø              p *   @0@@Ø              † *   @0@@Ø              – 3   @0@@Ø               *   @0@@Ø              8 3   @0@@Ø              p *   @0@@Ø              † *   @0@@Ø              – 3   @0@@Ø               3   @0@@Ø              @ 3   @0@@Ø              x *   @0@@Ø              ® 3   @0@@Ø              ‡ *   @0@@Ø               3   @0@@Ø              H 3   @0@@Ø              Ä 2   @0@@Ø              ∏ *   @0@@Ø               A   @0P@Ø              8  3   @0@@Ø              p  3   @0@@Ø              ®  2   @0@@Ø              ‡  3   @0@@Ø              ! *   @0@@Ø              H! 3   @0@@Ø              Ä! 3   @0@@Ø              ∏! *   @0@@Ø              Ë! *   @0@@Ø              " .   @0@@Ø              H" *   @0@@Ø              x" 3   @0@@Ø              ∞" 3   @0@@Ø              Ë" 3   @0@@Ø               # 3   @0@@Ø              X# 0   @0@@Ø              à# *   @0@@Ø              ∏# 3   @0@@Ø              # "   @0@@Ø              $ *   @0@@Ø              H$ 6   @0@@Ø              Ä$ *   @0@@Ø              ∞$ 3   @0@@Ø              Ë$ 3   @0@@Ø               % 3   @0@@Ø              X% <   @0@@Ø              ò% *   @0@@Ø              »% *   @0@@Ø              ¯% 6   @0@@Ø              0& 3   @0@@Ø              h& ;   @0@@Ø              ®& 3   @0@@Ø              ‡& 3   @0@@Ø              ' *   @0@@Ø              H' *   @0@@Ø              x' 3   @0@@Ø              ∞' 3   @0@@Ø              Ë' +   @0@@Ø              ( *   @0@@Ø              H( 3   @0@@Ø              Ä( *   @0@@Ø              ∞( *   @0@@Ø              ‡( *   @0@@Ø              ) 3   @0@@Ø              H) *   @0@@Ø              x) *   @0@@Ø              ®) 3   @0@@Ø              ‡) *   @0@@Ø              * 3   @0@@Ø              H* 3   @0@@Ø              Ä* '   @0@@Ø              ®* :   @0@@Ø              Ë* %   @0@@Ø              + 3   @0@@Ø              H+ *   @0@@Ø              x+ 3   @0@@Ø              ∞+ *   @0@@Ø              ‡+ *   @0@@Ø              , 1   @0@@Ø              H, *   @0@@Ø              x, *   @0@@Ø              ®, 3   @0@@Ø              ‡, *   @0@@Ø              - *   @0@@Ø              @- 3   @0@@Ø              x- *   @0@@Ø              ®- *   @0@@Ø              ÿ- 3   @0@@Ø              . 3   @0@@Ø              H. 3   @0@@Ø              Ä. *   @0@@Ø              ∞. *   @0@@Ø              ‡. 3   @0@@Ø              / *   @0@@Ø              H/ %   @0@@Ø              p/ 3   @0@@Ø              ®/ *   @0@@Ø              ÿ/ 3   @0@@Ø              0 3   @0@@Ø              H0 *   @0@@Ø              x0 3   @0@@Ø              ∞0 3   @0@@Ø              Ë0 +   @0@@Ø              1 *   @0@@Ø              H1 3   @0@@Ø              Ä1 3   @0@@Ø              ∏1 3   @0@@Ø              1 *   @0@@Ø               2 *   @0@@Ø              P2 3   @0@@Ø              à2 *   @0@@Ø              ∏2 3   @0@@Ø              2 *   @0@@Ø               3 3   @0@@Ø              X3 *   @0@@Ø              à3 3   @0@@Ø              ¿3 3   @0@@Ø              ¯3    @0@@Ø              4 3   @0@@Ø              P4 3   @0@@Ø              à4 *   @0@@Ø              ∏4 *   @0@@Ø              Ë4 *   @0@@Ø              5 3   @0@@Ø              P5 3   @0@@Ø              à5 3   @0@@Ø              ¿5 3   @0@@Ø              ¯5 *   @0@@Ø              (6 *   @0@@Ø              X6 3   @0@@Ø              ê6 *   @0@@Ø              ¿6 4   @0@@Ø              ¯6 2   @0@@Ø              07 3   @0@@Ø              h7 3   @0@@Ø              †7 *   @0@@Ø              –7 *   @0@@Ø               8 3   @0@@Ø              88 *   @0@@Ø              h8 ?   @0@@Ø              ®8 3   @0@@Ø              ‡8 3   @0@@Ø              9 5   @0@@Ø              P9 3   @0@@Ø              à9 *   @0@@Ø              ∏9 *   @0@@Ø              Ë9 *   @0@@Ø              : *   @0@@Ø              H: *   @0@@Ø              x: 3   @0@@Ø              ∞: 3   @0@@Ø              Ë: *   @0@@Ø              ; 3   @0@@Ø              P; 6   @0@@Ø              à; *   @0@@Ø              ∏; 3   @0@@Ø              ; 3   @0@@Ø              (< 5   @0@@Ø              `< /   @0@@Ø              ê< 3   @0@@Ø              »< %   @0@@Ø              < *   @0@@Ø               = 3   @0@@Ø              X= 3   @0@@Ø              ê= *   @0@@Ø              ¿= 3   @0@@Ø              ¯= 3   @0@@Ø              0> *   @0@@Ø              `> *   @0@@Ø              ê> *   @0@@Ø              ¿> 3   @0@@Ø              ¯> *   @0@@Ø              (? 3   @0@@Ø              `? 3   @0@@Ø              ò? *   @0@@Ø              »? *   @0@@Ø              ¯? *   @0@@Ø              (@ 3   @0@@Ø              `@ *   @0@@Ø              ê@ 3   @0@@Ø              »@ *   @0@@Ø              ¯@ *   @0@@Ø              (A *   @0@@Ø              XA *   @0@@Ø              àA 9   @0@@Ø              »A 3   @0@@Ø               B *   @0@@Ø              0B *   @0@@Ø              `B 3   @0@@Ø              †B U   @0P@Ø              ¯B *   @0@@Ø              (C *   @0@@Ø              XC *   @0@@Ø              àC 3   @0@@Ø              ¿C *   @0@@Ø              C 3   @0@@Ø              (D 6   @0@@Ø              `D 3   @0@@Ø              òD 3   @0@@Ø              –D *   @0@@Ø               E 3   @0@@Ø              8E *   @0@@Ø              hE 3   @0@@Ø              †E 3   @0@@Ø              ÿE 3   @0@@Ø              F 3   @0@@Ø              HF 3   @0@@Ø              ÄF 3   @0@@Ø              ¿F D   @0P@Ø              G *   @0@@Ø              8G *   @0@@Ø              hG *   @0@@Ø              òG *   @0@@Ø              »G 3   @0@@Ø               H 3   @0@@Ø              8H *   @0@@Ø              hH *   @0@@Ø              òH -   @0@@Ø              »H 3   @0@@Ø               I 3   @0@@Ø              8I 3   @0@@Ø              pI 3   @0@@Ø              ®I 3   @0@@Ø              ‡I *   @0@@Ø              J 3   @0@@Ø              HJ 3   @0@@Ø              ÄJ *   @0@@Ø              ∞J 7   @0@@Ø              ËJ *   @0@@Ø              K     @0@@Ø              8K    @0@@Ø              XK *   @0@@Ø              àK 3   @0@@Ø              ¿K 3   @0@@Ø              ¯K :   @0@@Ø              8L 4   @0@@Ø              pL 3   @0@@Ø              ®L *   @0@@Ø              ÿL 3   @0@@Ø              M /   @0@@Ø              @M 3   @0@@Ø              xM -   @0@@Ø              ®M 3   @0@@Ø              ‡M 3   @0@@Ø              N 1   @0@@Ø              PN *   @0@@Ø              ÄN 3   @0@@Ø              ∏N    @0@@Ø              ÿN *   @0@@Ø              O %   @0@@Ø              0O +   @0@@Ø              `O 3   @0@@Ø              òO 3   @0@@Ø              –O *   @0@@Ø               P 3   @0@@Ø              8P 3   @0@@Ø              pP (   @0@@Ø              òP 3   @0@@Ø              –P 3   @0@@Ø              Q *   @0@@Ø              8Q -   @0@@Ø              hQ *   @0@@Ø              òQ 3   @0@@Ø              –Q 3   @0@@Ø              R 3   @0@@Ø              @R 3   @0@@Ø              xR 3   @0@@Ø              ∞R *   @0@@Ø              ‡R 3   @0@@Ø              S 3   @0@@Ø              PS ,   @0@@Ø              ÄS *   @0@@Ø              ∞S S   @0P@Ø              T *   @0@@Ø              8T 3   @0@@Ø              pT 3   @0@@Ø              ®T *   @0@@Ø              ÿT *   @0@@Ø              U *   @0@@Ø              8U *   @0@@Ø              hU *   @0@@Ø              òU 1   @0@@Ø              –U *   @0@@Ø               V 3   @0@@Ø              8V 1   @0@@Ø              pV *   @0@@Ø              †V 3   @0@@Ø              ÿV 3   @0@@Ø              W *   @0@@Ø              @W *   @0@@Ø              pW *   @0@@Ø              †W 3   @0@@Ø              ÿW *   @0@@Ø              X *   @0@@Ø              8X *   @0@@Ø              hX *   @0@@Ø              òX *   @0@@Ø              »X *   @0@@Ø              ¯X -   @0@@Ø              (Y *   @0@@Ø              XY $   @0@@Ø              ÄY !   @0@@Ø              ®Y +   @0@@Ø              ÿY 3   @0@@Ø              Z 3   @0@@Ø              HZ 1   @0@@Ø              ÄZ .   @0@@Ø              ∞Z *   @0@@Ø              ‡Z 3   @0@@Ø              [    @0@@             ([    @0@@             8[    @0@@æ              H[    @0@@è              `[    @0@@è              x[    @0@@è              ê[    @0@@è              ®[    @0@@è              ¿[    @0@@è              ‡[    @0@@è              ¯[ "   @0@@è               \    @0@@è              8\    @0@@è              H\    @0@@è              h\    @0@@è              x\    @0@@è              ò\    @0@@è              ∞\    @0@@è              ¿\    @0@@è              ‡\    @0@@è               ]    @0@@è              ]    @0@@è              8]    @0@@è              X]    @0@@è              x]    @0@@è              ê]    @0@@è              ∞]    @0@@è              –]    @0@@è              ]    @0@@è              ^    @0@@è               ^    @0@@è              0^    @00@Ø              8^    @0@@Ø              H^    @0@@Ø              X^     @0@@Ø              x^    @0@@Ø              ê^    @0@@Ø              ∞^ 	   @0@@Ø              ¿^    @0@@Ø              –^    @0@@Ø              ‡^    @0@@Ø              ¯^    @0@@Ø              _ 	   @0@@Ø               _ 	   @0@@Ø              0_ 
   @0@@Ø              <_    @00@Ø              H_    @0@@Ø              h_    @00@Ø              p_    @00@Ø              x_    @00@Ø              Ä_    @00@Ø              à_    @00@Ø              ê_    @00@Ø              ò_    @00@Ø              ú_    @00@Ø              §_    @00@Ø              ®_    @00@Ø              ∞_    @00@Ø              ∏_    @0@@Ø              ¿_    @00@Ø              »_    @00@[              –_    @00@[              ÿ_    @00@Ø              ‡_    @00@Ø              Ë_    @00@Ø              _    @00@[              ¯_    @00@Ø               `    @00@Ø              `    @00@Ø              `    @00@Ø              `    @00@Ø               `    @00@Ø              (`    @00@Ø              0`    @00@Ø              8`    @00@Ø              @`    @00@Ø              H`    @00@Ø              P`    @00@Ø              X`    @00@[              ``    @00@Ø              h`    @00@Ø              p`    @00@Ø              x`    @00@Ø              Ä`    @00@Ø              à`    @00@Ø              å`    @00@Ø              î`    @00@Ø              ú`    @00@Ø              §`    @00@Ø              ¨`    @00@Ø              ¥`    @00@Ø              ¿`    @0@@Ø              –`    @0@@Ø              ‰`    @00@Ø              `    @0@@Ø               a    @0@@Ø              a 
   @0@@Ø               a    @0@@Ø              8a !   @0@@Ø              `a    @0@@Ø              pa    @0@@Ø              Äa    @0@@Ø              òa    @0@@Ø              ®a    @0@@Ø              ¿a    @0@@Ø              ÿa    @0@@Ø              Ëa    @0@@Ø               b    @0@@Ø              b    @0@@Ø              (b     @0@@Ø              Hb    @0@@Ø              Xb    @0@@Ø              hb    @0@@Ø              xb    @0@@Ø              êb    @0@@Ø              †b    @0@@Ø              ∏b    @0@@Ø              ÿb    @0@@Ø              b    @0@@Ø              c    @0@@Ø               c    @0@@Ø              8c    @0@@Ø              @c 	   @0@@Ø              Pc    @0@@Ø              `c    @0@@Ø              tc    @00@Ø              |c    @00@Ø              Ñc    @00@Ø              àc    @0@@Ø              òc    @0@@Ø              ®c 
   @0@@[              ∏c    @0@@Ø              Ãc    @00@[              ‘c    @00@Ø              ‹c    @00@Ø              ‰c    @00@Ø              Ïc    @00@Ø              ¯c    @0@@Ø              d    @00@Ø              d    @0@@Ø              0d    @0@@Ø              Hd    @0@@Ø              hd    @0@@Ø              xd    @00@Ø              Äd 
   @0@@Ø              êd    @0@@Ø              †d    @0@@Ø              ∞d    @0@@Ø              ¿d    @0@@Ø              ÿd    @0@@Ø              ¯d    @0@@Ø              e 
   @0@@Ø              e    @00@Ø              $e    @00@Ø              0e    @0@@Ø              De    @00@Ø              Pe    @0@@Ø              he    @0@@Ø              Äe    @0@@Ø              òe    @0@@Ø              ∞e    @0@@Ø              »e    @0@@Ø              ‡e 
   @0@@Ø              e 
   @0@@Ø               f 	   @0@@Ø              f    @00@Ø              f    @00@Ø              f    @00@Ø              f    @00@Ø              f    @00@Ø               f    @00@Ø              $f    @00@Ø              (f    @00@Ø              ,f    @00@Ø              0f    @00@Ø              4f    @00@Ø              8f    @00@Ø              @f    @0@@Ø              Pf 	   @0@@Ø              `f    @0@@Ø              Äf    @0@@Ø              òf    @0@@Ø              ∞f    @0@@Ø              –f    @0@@Ø              Ëf    @0@@Ø              g    @0@@Ø               g    @0@@Ø              8g    @0@@Ø              Pg    @0@@Ø              hg    @0@@Ø              xg    @0@@Ø              êg    @0@@Ø              †g    @0@@Ø              ∞g    @0@@Ø              ¿g    @0@@Ø              –g    @0@@Ø              Ëg    @00@Ø              g    @0@@Ø              h    @0@@Ø               h    @0@@Ø              8h    @0@@Ø              Hh    @0@@Ø              Th    @00@Ø              `h    @0@@Ø              xh    @0@@Ø              àh 
   @0@@Ø              òh 
   @0@@Ø              ®h    @0@@Ø              ¿h    @0@@Ø              ‡h    @0@@Ø               i    @0@@Ø              i    @0@@Ø              (i    @0@@Ø              8i    @0@@Ø              Pi    @0@@Ø              `i    @0@@Ø              xi    @0@@Ø              êi    @0@@Ø              †i    @00@Ø              ®i    @00@Ø              ∞i    @0@@Ø              »i    @0@@Ø              ÿi    @0@@Ø              i #   @0@@Ø              j    @0@@Ø              8j "   @0@@Ø              `j '   @0@@Ø              àj 	   @0@@Ø              òj    @0@@Ø              ®j    @0@@Ø              ∏j    @0@@Ø              –j    @0@@Ø              Ëj     @0@@Ø              k    @0@@Ø              k    @0@@Ø              (k    @0@@Ø              8k    @0@@Ø              Hk    @0@@Ø              `k    @0@@Ø              Äk    @0@@Ø              òk    @0@@Ø              ®k    @0@@Ø              ¿k    @0@@Ø              ‡k    @0@@Ø               l    @0@@Ø              l    @0@@Ø              (l    @0@@Ø              8l    @0@@Ø              Hl    @0@@Ø              `l    @0@@Ø              xl    @0@@Ø              êl    @0@@Ø              ∞l    @0@@Ø              »l    @0@@Ø              ÿl    @0@@Ø              l    @0@@Ø              m    @0@@Ø              m    @0@@Ø              (m    @0@@Ø              8m 
   @0@@Ø              Hm    @0@@Ø              hm    @0@@Ø              Äm    @0@@Ø              òm    @0@@Ø              ∞m    @0@@Ø              –m    @0@@Ø              Ëm    @0@@Ø               n    @0@@Ø              n "   @0@@Ø              @n    @0@@Ø              Xn    @0@@Ø              pn    @0@@Ø              Än    @0@@Ø              †n    @0@@Ø              ¿n    @0@@Ø              ÿn    @0@@Ø              Ën    @0@@Ø               o    @0@@Ø              o    @0@@Ø              0o    @0@@Ø              Ho    @0@@Ø              ho    @0@@Ø              Äo    @0@@Ø              êo    @0@@Ø              ®o    @0@@Ø              ¿o    @0@@Ø              ÿo    @0@@Ø              Ëo    @0@@Ø              ¯o    @0@@Ø              p    @0@@Ø               p    @0@@Ø              @p    @0@@Ø              `p    @0@@Ø              xp    @0@@Ø              àp    @0@@Ø              òp    @0@@Ø              ®p    @0@@Ø              ∏p    @0@@Ø              »p    @0@@Ø              ‡p    @0@@Ø              p    @0@@Ø               q    @0@@Ø              q    @0@@Ø              (q    @0@@Ø              @q "   @0@@Ø              hq 	   @0@@Ø              xq    @0@@Ø              êq    @0@@Ø              ®q    @0@@Ø              ∏q    @0@@Ø              »q #   @0@@Ø              q    @0@@Ø               r    @0@@Ø              r #   @0@@Ø              <r    @00@Ø              Hr 	   @0@@Ø              Xr    @0@@Ø              pr    @0@@Ø              àr    @0@@Ø              òr    @0@@Ø              ∞r    @0@@Ø              –r    @0@@Ø              r %   @0@@Ø              s '   @0@@Ø              @s    @0@@Ø              `s     @0@@Ø              Äs    @0@@Ø              †s    @0@@Ø              ∏s    @0@@Ø              –s    @0@@Ø              Ës    @0@@Ø              ¯s    @0@@Ø              t    @0@@Ø               t    @0@@Ø              0t    @0@@Ø              8t    @0@@Ø              Ht    @0@@Ø              `t    @0@@Ø              xt    @0@@Ø              êt    @0@@Ø              ®t    @0@@Ø              ¿t    @0@@Ø              ÿt    @0@@Ø              t    @0@@Ø              u    @0@@Ø              (u    @0@@Ø              Hu    @0@@Ø              `u    @0@@Ø              xu    @0@@Ø              êu    @0@@Ø              †u    @0@@Ø              ∞u    @0@@Ø              »u !   @0@@Ø              u    @0@@Ø              v    @0@@Ø               v    @0@@Ø              8v    @0@@Ø              Xv    @0@@Ø              pv    @0@@Ø              Äv    @0@@Ø              òv    @0@@Ø              ∞v    @0@@Ø              »v    @0@@Ø              Ëv    @0@@Ø              ¯v $   @0@@Ø               w ,   @0@@Ø              Pw    @0@@Ø              hw )   @0@@Ø              òw ,   @0@@Ø              »w 	   @0@@Ø              ÿw    @0@@Ø              ¯w "   @0@@Ø               x    @0@@Ø              8x    @0@@Ø              Hx $   @0@@Ø              px    @0@@Ø              àx    @0@@Ø              ®x    @0@@Ø              ¿x    @0@@Ø              ‡x    @0@@Ø              ¯x    @0@@Ø              y    @0@@Ø              0y    @0@@Ø              Hy $   @0@@Ø              py    @0@@Ø              êy    @0@@Ø              ∞y    @0@@Ø              »y    @0@@Ø              Ëy    @0@@Ø              z !   @0@@Ø              0z    @0@@Ø              Pz     @0@@Ø              pz    @0@@Ø              àz    @0@@Ø              ®z    @0@@Ø              ¿z    @0@@Ø              ‡z    @0@@Ø              z    @0@@Ø              {    @0@@Ø              {    @0@@Ø              ({    @0@@Ø              <{    @00@Ø              H{    @0@@Ø              `{    @0@@Ø              h{    @0@@Ø              Ä{    @0@@Ø              ò{    @0@@Ø              ∞{ 	   @0@@Ø              ¿{    @0@@Ø              –{    @0@@Ø              ‡{    @0@@Ø              {    @0@@Ø               |    @0@@Ø              |    @0@@Ø              (|    @0@@Ø              @|    @0@@Ø              P|    @0@@Ø              `|    @0@@Ø              p|    @0@@Ø              à|    @0@@Ø              ò|    @0@@Ø              ®|    @0@@Ø              ∏|    @0@@Ø              »|    @0@@Ø              ÿ|    @0@@Ø              | '   @0@@Ø              }    @00@Ø               } 
   @0@@Ø              ,}    @00@Ø              4}    @00@Ø              <}    @00@Ø              @}    @00@Ø              H}    @0@@Ø              X}    @00@Ø              `}    @0@@Ø              h} 
   @0@@Ø              x}    @0@@Ø              à}    @0@@Ø              ò}    @0@@Ø              ®}    @0@@Ø              ∏}    @0@@Ø              »}    @0@@Ø              Ë}    @0@@Ø              ~    @0@@Ø               ~    @0@@Ø              8~    @0@@Ø              P~    @0@@Ø              p~    @0@@Ø              ê~    @0@@Ø              †~    @0@@Ø              ¿~    @0@@Ø              ‡~    @0@@Ø                   @0@@Ø                "   @0@@Ø              H #   @0@@Ø              p '   @0@@Ø              ò (   @0@@Ø              ¿    @0@@Ø              –    @0@@Ø              ‡    @0@@Ø               Ä    @0@@Ø               Ä    @0@@Ø              8Ä    @0@@Ø              PÄ    @0@@Ø              hÄ    @0@@Ø              ÄÄ    @0@@Ø              †Ä    @0@@Ø              ¿Ä    @0@@Ø              ÿÄ    @0@@Ø              Ä    @0@@Ø              Å    @0@@Ø              (Å 
   @0@@Ø              8Å    @0@@Ø              PÅ    @0@@Ø              hÅ    @0@@Ø              ÄÅ    @0@@Ø              òÅ    @0@@Ø              ®Å    @0@@Ø              ∏Å    @0@@Ø              »Å    @0@@Ø              ÿÅ    @0@@Ø              Å    @0@@Ø              Ç    @0@@Ø              Ç    @0@@Ø              0Ç    @0@@Ø              HÇ    @0@@Ø              XÇ    @0@@Ø              pÇ    @0@@Ø              àÇ    @0@@Ø              †Ç    @0@@Ø              ¿Ç    @0@@Ø              ÿÇ    @0@@Ø              Ç    @0@@Ø              É    @0@@Ø              (É 
   @0@@Ø              8É 	   @0@@Ø              HÉ    @0@@Ø              hÉ &   @0@@Ø              êÉ 	   @0@@Ø              †É     @0@@Ø              ¿É    @0@@Ø              ÿÉ    @0@@Ø              É    @0@@Ø               Ñ    @0@@Ø              Ñ    @0@@Ø              (Ñ    @0@@Ø              HÑ    @0@@Ø              `Ñ    @0@@Ø              ÄÑ     @0@@Ø              †Ñ    @0@@Ø              ¿Ñ    @0@@Ø              ‡Ñ    @0@@Ø              ¯Ñ    @0@@Ø              Ö    @0@@Ø              (Ö    @0@@Ø              @Ö    @0@@Ø              XÖ    @0@@Ø              hÖ    @0@@Ø              xÖ 	   @0@@Ø              àÖ    @0@@Ø              òÖ    @0@@Ø              ®Ö 	   @0@@Ø              ∏Ö    @0@@Ø              »Ö    @0@@Ø              ÿÖ    @0@@Ø              ËÖ    @0@@Ø               Ü    @0@@Ø              Ü    @0@@Ø              (Ü    @0@@Ø              @Ü 
   @0@@Ø              PÜ    @0@@Ø              `Ü    @0@@Ø              xÜ 	   @0@@Ø              àÜ    @0@@Ø              òÜ    @00@Ø              †Ü 	   @0@@Ø              ∞Ü    @0@@Ø              ∏Ü    @0@@Ø              »Ü    @0@@Ø              ÿÜ    @0@@Ø              ËÜ    @0@@Ø              ¯Ü    @0@@Ø              á    @0@@Ø              (á    @00@Ø              0á    @0@@Ø              8á    @0@@Ø              Há    @0@@Ø              há    @0@@Ø              Äá    @0@@Ø              òá    @0@@Ø              ∞á    @0@@Ø              »á    @0@@Ø              ‡á 
   @0@@Ø              á    @0@@Ø               à    @0@@Ø              à    @0@@Ø               à -   @0@@Ø              Pà 
   @0@@Ø              `à    @0@@Ø              xà    @0@@Ø              òà    @0@@Ø              ∏à    @0@@Ø              »à    @0@@Ø              Ëà    @0@@Ø              â    @0@@Ø               â    @0@@Ø              8â    @0@@Ø              Hâ    @0@@Ø              `â    @0@@Ø              Äâ    @0@@Ø              òâ    @0@@Ø              ∏â "   @0@@Ø              ‡â    @0@@Ø               ä    @0@@Ø              ä    @0@@Ø              0ä    @0@@Ø              Hä    @0@@Ø              Xä    @0@@Ø              pä    @0@@Ø              Ää    @00@Ø              àä    @0@@Ø              êä    @0@@Ø              †ä    @0@@Ø              ¿ä    @0@@Ø              ÿä    @0@@Ø              ä    @0@@Ø               ã    @0@@Ø              ã    @0@@Ø              8ã    @0@@Ø              Pã    @0@@Ø              `ã    @0@@Ø              Äã    @0@@Ø              êã    @0@@Ø              †ã    @0@@Ø              ∞ã    @0@@Ø              »ã    @0@@Ø              ÿã    @0@@Ø              Ëã    @0@@Ø              ¯ã    @0@@Ø              å    @0@@Ø              (å    @0@@Ø              Hå    @0@@Ø              `å    @0@@Ø              xå    @0@@Ø              êå    @0@@Ø              ®å    @0@@Ø              ∏å    @0@@Ø              ÿå    @0@@Ø              ¯å '   @0@@Ø               ç '   @0@@Ø              Hç    @0@@Ø              Tç    @00@Ø              `ç    @0@@Ø              pç 	   @0@@Ø              Äç    @0@@Ø              êç    @0@@Ø              †ç    @0@@Ø              ∞ç    @0@@Ø              ¿ç    @0@@Ø              –ç    @0@@Ø              Ëç    @0@@Ø               é    @0@@Ø              é    @0@@Ø              0é    @0@@Ø              Hé    @0@@Ø              Xé    @0@@Ø              hé    @0@@Ø              Äé    @0@@Ø              òé    @0@@Ø              ®é    @0@@Ø              ¿é    @0@@Ø              ‡é    @0@@Ø              ¯é 
   @0@@Ø              è    @0@@Ø              è    @0@@Ø              8è    @0@@Ø              Pè    @0@@Ø              hè    @0@@Ø              àè    @0@@Ø              †è    @0@@Ø              ∞è    @0@@˚              ¿è    @0@@˚              –è    @0@@˚              ‡è    @0@@˚              è    @0@@˚               ê    @0@@˚              ê    @0@@˚               ê    @0@@˚              0ê    @0@@˚              @ê    @0@@˚              Pê    @0@@˚              `ê    @0@@˚              pê    @0@@˚              Äê    @0@@˚              êê    @0@@˚              †ê    @0@@˚              ∞ê    @0@@˚              ¿ê    @0@@˚              –ê    @0@@˚              ‡ê    @0@@˚              ê    @0@@˚               ë    @0@@˚              ë    @0@@˚               ë    @0@@˚              0ë    @0@@˚              @ë    @0@@˚              Pë    @0@@˚              `ë    @0@@˚              pë    @0@@˚              Äë    @0@@˚              êë    @0@@˚              †ë    @0@@˚              ∞ë    @0@@˚              ¿ë    @0@@˚              –ë    @0@@˚              ‡ë    @0@@˚              ë    @0@@˚               í    @0@@˚              í    @0@@˚               í    @0@@˚              0í    @0@@˚              @í    @0@@˚              Pí    @0@@˚              `í    @0@@˚              pí    @0@@˚              Äí    @0@@˚              êí    @0@@˚              †í    @0@@˚              ∞í    @0@@˚              ¿í    @0@@h              –í    @0@@h              ‡í    @0@@h              í    @0@@h               ì    @0@@h              ì    @0@@h               ì    @0@@h              0ì    @0@@h              @ì    @0@@h              Pì    @0@@h              `ì    @0@@h              pì    @0@@h              Äì    @0@@h              êì    @0@@h              †ì    @0@@h              ∞ì    @0@@h              ¿ì    @0@@h              –ì    @0@@h              ‡ì    @0@@h              ì    @0@@h               î    @0@@h              î    @0@@h               î    @0@@h              0î    @0@@h              @î    @0@@h              Pî    @0@@h              `î    @0@@h              pî    @0@@h              Äî    @0@@h              êî    @0@@h              †î    @0@@h              ∞î    @0@@h              ¿î    @0@@h              –î    @0@@h              ‡î    @0@@h              î    @0@@h               ï    @0@@h              ï    @0@@h               ï    @0@@h              0ï    @0@@h              @ï    @0@@h              Pï    @0@@h              `ï    @0@@h              pï    @0@@h              Äï    @0@@h              êï    @0@@h              †ï    @0@@h              ∞ï    @0@@h              ¿ï    @0@@h              –ï    @0@@h              ‡ï    @0@@h              ï    @0@@h               ñ    @0@@h              ñ    @0@@h               ñ    @0@@h              0ñ    @0@@h              @ñ    @0@@h              Pñ    @0@@h              `ñ    @0@@h              pñ    @0@@h              Äñ    @0@@h              êñ    @0@@h              †ñ    @0@@h              ∞ñ    @0@@h              ¿ñ    @0@@h              –ñ    @0@@h              ‡ñ    @0@@h              ñ    @0@@h               ó    @0@@h              ó    @0@@h               ó    @0@@h              0ó    @0@@h              @ó    @0@@h              Pó    @0@@h              `ó    @0@@h              pó    @0@@h              Äó    @0@@h              êó    @0@@h              †ó    @0@@h              ∞ó    @0@@h              ¿ó    @0@@h              –ó    @0@@h              ‡ó    @0@@h              ó    @0@@h               ò    @0@@h              ò    @0@@h               ò    @0@@h              0ò    @0@@h              @ò    @0@@h              Pò    @0@@h              `ò    @0@@h              pò    @0@@h              Äò    @0@@h              êò    @0@@h              †ò    @0@@h              ∞ò    @0@@h              ¿ò    @0@@h              –ò    @0@@h              ‡ò    @0@@h              ò    @0@@h               ô    @0@@h              ô    @0@@h               ô    @0@@h              0ô    @0@@h              @ô    @0@@h              Pô    @0@@h              `ô    @0@@h              pô    @0@@h              Äô    @0@@h              êô    @0@@h              †ô    @0@@h              ∞ô    @0@@h              ¿ô    @0@@h              –ô    @0@@h              ‡ô    @0@@h              ô    @0@@h               ö    @0@@h              ö    @0@@h               ö    @0@@h              0ö    @0@@h              @ö    @0@@h              Pö    @0@@h              `ö    @0@@h              pö    @0@@h              Äö    @0@@h              êö    @0@@h              †ö    @0@@h              ∞ö    @0@@h              ¿ö    @0@@h              –ö    @0@@h              ‡ö    @0@@h              ö    @0@@h               õ    @0@@h              õ    @0@@h               õ    @0@@h              0õ    @0@@h              @õ    @0@@h              Põ    @0@@h              `õ    @0@@h              põ    @0@@h              Äõ    @0@@h              êõ    @0@@h              †õ    @0@@h              ∞õ    @0@@h              ¿õ    @0@@h              –õ    @0@@h              ‡õ    @0@@h              õ    @0@@h               ú    @0@@h              ú    @0@@h               ú    @0@@h              0ú    @0@@h              @ú    @0@@h              Pú    @0@@h              `ú    @0@@h              pú    @0@@h              Äú    @0@@h              êú    @0@@h              †ú    @0@@h              ∞ú    @0@@h              ¿ú    @0@@h              –ú    @0@@h              ‡ú    @0@@h              ú    @0@@h               ù    @0@@h              ù    @0@@h               ù    @0@@h              0ù    @0@@h              @ù    @0@@h              Pù    @0@@h              `ù    @0@@h              pù    @0@@h              Äù    @0@@h              êù    @0@@\
	                                            DUK_TYPE_MASK_BUFFER | \
	                                            DUK_TYPE_MASK_POINTER | \
	                                            DUK_TYPE_MASK_LIGHTFUNC | \
	                                            DUK_TYPE_MASK_THROW))

/*
 *  Coercion operations: in-place coercion, return coerced value where
 *  applicable.  If index is invalid, throw error.  Some coercions may
 *  throw an expected error (e.g. from a toString() or valueOf() call)
 *  or an internal error (e.g. from out of memory).
 */

DUK_EXTERNAL_DECL void duk_to_undefined(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_to_null(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_bool_t duk_to_boolean(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_double_t duk_to_number(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_int_t duk_to_int(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_uint_t duk_to_uint(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_int32_t duk_to_int32(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_uint32_t duk_to_uint32(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_uint16_t duk_to_uint16(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL const char *duk_to_string(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL const char *duk_to_lstring(duk_context *ctx, duk_idx_t index, duk_size_t *out_len);
DUK_EXTERNAL_DECL void *duk_to_buffer_raw(duk_context *ctx, duk_idx_t index, duk_size_t *out_size, duk_uint_t flags);
DUK_EXTERNAL_DECL void *duk_to_pointer(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_to_object(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_to_defaultvalue(duk_context *ctx, duk_idx_t index, duk_int_t hint);
DUK_EXTERNAL_DECL void duk_to_primitive(duk_context *ctx, duk_idx_t index, duk_int_t hint);

#define DUK_BUF_MODE_FIXED      0   /* internal: request fixed buffer result */
#define DUK_BUF_MODE_DYNAMIC    1   /* internal: request dynamic buffer result */
#define DUK_BUF_MODE_DONTCARE   2   /* internal: don't care about fixed/dynamic nature */

#define duk_to_buffer(ctx,index,out_size) \
	duk_to_buffer_raw((ctx), (index), (out_size), DUK_BUF_MODE_DONTCARE)
#define duk_to_fixed_buffer(ctx,index,out_size) \
	duk_to_buffer_raw((ctx), (index), (out_size), DUK_BUF_MODE_FIXED)
#define duk_to_dynamic_buffer(ctx,index,out_size) \
	duk_to_buffer_raw((ctx), (index), (out_size), DUK_BUF_MODE_DYNAMIC)

/* safe variants of a few coercion operations */
DUK_EXTERNAL_DECL const char *duk_safe_to_lstring(duk_context *ctx, duk_idx_t index, duk_size_t *out_len);
#define duk_safe_to_string(ctx,index) \
	duk_safe_to_lstring((ctx), (index), NULL)

/*
 *  Misc conversion
 */

DUK_EXTERNAL_DECL const char *duk_base64_encode(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_base64_decode(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL const char *duk_hex_encode(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_hex_decode(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL const char *duk_json_encode(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_json_decode(duk_context *ctx, duk_idx_t index);

/*
 *  Buffer
 */

DUK_EXTERNAL_DECL void *duk_resize_buffer(duk_context *ctx, duk_idx_t index, duk_size_t new_size);

/*
 *  Property access
 *
 *  The basic function assumes key is on stack.  The _string variant takes
 *  a C string as a property name, while the _index variant takes an array
 *  index as a property name (e.g. 123 is equivalent to the key "123").
 */

DUK_EXTERNAL_DECL duk_bool_t duk_get_prop(duk_context *ctx, duk_idx_t obj_index);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
DUK_EXTERNAL_DECL duk_bool_t duk_get_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop(duk_context *ctx, duk_idx_t obj_index);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
DUK_EXTERNAL_DECL duk_bool_t duk_put_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop(duk_context *ctx, duk_idx_t obj_index);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
DUK_EXTERNAL_DECL duk_bool_t duk_del_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop(duk_context *ctx, duk_idx_t obj_index);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key);
DUK_EXTERNAL_DECL duk_bool_t duk_has_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index);
DUK_EXTERNAL_DECL void duk_def_prop(duk_context *ctx, duk_idx_t obj_index, duk_uint_t flags);

DUK_EXTERNAL_DECL duk_bool_t duk_get_global_string(duk_context *ctx, const char *key);
DUK_EXTERNAL_DECL duk_bool_t duk_put_global_string(duk_context *ctx, const char *key);

/*
 *  Object prototype
 */

DUK_EXTERNAL_DECL void duk_get_prototype(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_set_prototype(duk_context *ctx, duk_idx_t index);

/*
 *  Object finalizer
 */

DUK_EXTERNAL_DECL void duk_get_finalizer(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_set_finalizer(duk_context *ctx, duk_idx_t index);

/*
 *  Global object
 */

DUK_EXTERNAL_DECL void duk_set_global_object(duk_context *ctx);

/*
 *  Duktape/C function magic value
 */

DUK_EXTERNAL_DECL duk_int_t duk_get_magic(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL void duk_set_magic(duk_context *ctx, duk_idx_t index, duk_int_t magic);
DUK_EXTERNAL_DECL duk_int_t duk_get_current_magic(duk_context *ctx);

/*
 *  Module helpers: put multiple function or constant properties
 */

DUK_EXTERNAL_DECL void duk_put_function_list(duk_context *ctx, duk_idx_t obj_index, const duk_function_list_entry *funcs);
DUK_EXTERNAL_DECL void duk_put_number_list(duk_context *ctx, duk_idx_t obj_index, const duk_number_list_entry *numbers);

/*
 *  Variable access
 */

/* XXX: These calls are incomplete and not usable now.  They are not (yet)
 * part of the public API.
 */
DUK_EXTERNAL_DECL void duk_get_var(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_put_var(duk_context *ctx);
DUK_EXTERNAL_DECL duk_bool_t duk_del_var(duk_context *ctx);
DUK_EXTERNAL_DECL duk_bool_t duk_has_var(duk_context *ctx);

/*
 *  Object operations
 */

DUK_EXTERNAL_DECL void duk_compact(duk_context *ctx, duk_idx_t obj_index);
DUK_EXTERNAL_DECL void duk_enum(duk_context *ctx, duk_idx_t obj_index, duk_uint_t enum_flags);
DUK_EXTERNAL_DECL duk_bool_t duk_next(duk_context *ctx, duk_idx_t enum_index, duk_bool_t get_value);

/*
 *  String manipulation
 */

DUK_EXTERNAL_DECL void duk_concat(duk_context *ctx, duk_idx_t count);
DUK_EXTERNAL_DECL void duk_join(duk_context *ctx, duk_idx_t count);
DUK_EXTERNAL_DECL void duk_decode_string(duk_context *ctx, duk_idx_t index, duk_decode_char_function callback, void *udata);
DUK_EXTERNAL_DECL void duk_map_string(duk_context *ctx, duk_idx_t index, duk_map_char_function callback, void *udata);
DUK_EXTERNAL_DECL void duk_substring(duk_context *ctx, duk_idx_t index, duk_size_t start_char_offset, duk_size_t end_char_offset);
DUK_EXTERNAL_DECL void duk_trim(duk_context *ctx, duk_idx_t index);
DUK_EXTERNAL_DECL duk_codepoint_t duk_char_code_at(duk_context *ctx, duk_idx_t index, duk_size_t char_offset);

/*
 *  Ecmascript operators
 */

DUK_EXTERNAL_DECL duk_bool_t duk_equals(duk_context *ctx, duk_idx_t index1, duk_idx_t index2);
DUK_EXTERNAL_DECL duk_bool_t duk_strict_equals(duk_context *ctx, duk_idx_t index1, duk_idx_t index2);

/*
 *  Function (method) calls
 */

DUK_EXTERNAL_DECL void duk_call(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL void duk_call_method(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL void duk_call_prop(duk_context *ctx, duk_idx_t obj_index, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_pcall(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_pcall_method(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_pcall_prop(duk_context *ctx, duk_idx_t obj_index, duk_idx_t nargs);
DUK_EXTERNAL_DECL void duk_new(duk_context *ctx, duk_idx_t nargs);
DUK_EXTERNAL_DECL duk_int_t duk_safe_call(duk_context *ctx, duk_safe_call_function func, duk_idx_t nargs, duk_idx_t nrets);

/*
 *  Thread management
 */

/* There are currently no native functions to yield/resume, due to the internal
 * limitations on coroutine handling.  These will be added later.
 */

/*
 *  Compilation and evaluation
 */

DUK_EXTERNAL_DECL duk_int_t duk_eval_raw(duk_context *ctx, const char *src_buffer, duk_size_t src_length, duk_uint_t flags);
DUK_EXTERNAL_DECL duk_int_t duk_compile_raw(duk_context *ctx, const char *src_buffer, duk_size_t src_length, duk_uint_t flags);

/* plain */
#define duk_eval(ctx)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL))

#define duk_eval_noresult(ctx)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL | DUK_COMPILE_NORESULT))

#define duk_peval(ctx)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE))

#define duk_peval_noresult(ctx)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE | DUK_COMPILE_NORESULT))

#define duk_compile(ctx,flags)  \
	((void) duk_compile_raw((ctx), NULL, 0, (flags)))

#define duk_pcompile(ctx,flags)  \
	(duk_compile_raw((ctx), NULL, 0, (flags) | DUK_COMPILE_SAFE))

/* string */
#define duk_eval_string(ctx,src)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_eval_raw((ctx), (src), 0, DUK_COMPILE_EVAL | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN))

#define duk_eval_string_noresult(ctx,src)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_eval_raw((ctx), (src), 0, DUK_COMPILE_EVAL | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN | DUK_COMPILE_NORESULT))

#define duk_peval_string(ctx,src)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_eval_raw((ctx), (src), 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN))

#define duk_peval_string_noresult(ctx,src)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_eval_raw((ctx), (src), 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN | DUK_COMPILE_NORESULT))

#define duk_compile_string(ctx,flags,src)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_compile_raw((ctx), (src), 0, (flags) | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN))

#define duk_compile_string_filename(ctx,flags,src)  \
	((void) duk_compile_raw((ctx), (src), 0, (flags) | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN))

#define duk_pcompile_string(ctx,flags,src)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_compile_raw((ctx), (src), 0, (flags) | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN))

#define duk_pcompile_string_filename(ctx,flags,src)  \
	(duk_compile_raw((ctx), (src), 0, (flags) | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN))

/* lstring */
#define duk_eval_lstring(ctx,buf,len)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_eval_raw((ctx), buf, len, DUK_COMPILE_EVAL | DUK_COMPILE_NOSOURCE))

#define duk_eval_lstring_noresult(ctx,buf,len)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_eval_raw((ctx), buf, len, DUK_COMPILE_EVAL | DUK_COMPILE_NOSOURCE | DUK_COMPILE_NORESULT))

#define duk_peval_lstring(ctx,buf,len)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_eval_raw((ctx), buf, len, DUK_COMPILE_EVAL | DUK_COMPILE_NOSOURCE | DUK_COMPILE_SAFE))

#define duk_peval_lstring_noresult(ctx,buf,len)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_eval_raw((ctx), buf, len, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE | DUK_COMPILE_NORESULT))

#define duk_compile_lstring(ctx,flags,buf,len)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 (void) duk_compile_raw((ctx), buf, len, (flags) | DUK_COMPILE_NOSOURCE))

#define duk_compile_lstring_filename(ctx,flags,buf,len)  \
	((void) duk_compile_raw((ctx), buf, len, (flags) | DUK_COMPILE_NOSOURCE))

#define duk_pcompile_lstring(ctx,flags,buf,len)  \
	((void) duk_push_string((ctx), (const char *) (__FILE__)), \
	 duk_compile_raw((ctx), buf, len, (flags) | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE))

#define duk_pcompile_lstring_filename(ctx,flags,buf,len)  \
	(duk_compile_raw((ctx), buf, len, (flags) | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE))

/* file */
#define duk_eval_file(ctx,path)  \
	((void) duk_push_string_file_raw((ctx), (path), 0), \
	 (void) duk_push_string((ctx), (path)), \
	 (void) duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL))

#define duk_eval_file_noresult(ctx,path)  \
	((void) duk_push_string_file_raw((ctx), (path), 0), \
	 (void) duk_push_string((ctx), (path)), \
	 (void) duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL | DUK_COMPILE_NORESULT))

#define duk_peval_file(ctx,path)  \
	((void) duk_push_string_file_raw((ctx), (path), DUK_STRING_PUSH_SAFE), \
	 (void) duk_push_string((ctx), (path)), \
	 duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE))

#define duk_peval_file_noresult(ctx,path)  \
	((void) duk_push_string_file_raw((ctx), (path), DUK_STRING_PUSH_SAFE), \
	 (void) duk_push_string((ctx), (path)), \
	 duk_eval_raw((ctx), NULL, 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE | DUK_COMPILE_NORESULT))

#define duk_compile_file(ctx,flags,path)  \
	((void) duk_push_string_file_raw((ctx), (path), 0), \
	 (void) duk_push_string((ctx), (path)), \
	 (void) duk_compile_raw((ctx), NULL, 0, (flags)))

#define duk_pcompile_file(ctx,flags,path)  \
	((void) duk_push_string_file_raw((ctx), (path), DUK_STRING_PUSH_SAFE), \
	 (void) duk_push_string((ctx), (path)), \
	 duk_compile_raw((ctx), NULL, 0, (flags) | DUK_COMPILE_SAFE))

/*
 *  Logging
 */

DUK_EXTERNAL_DECL void duk_log(duk_context *ctx, duk_int_t level, const char *fmt, ...);
DUK_EXTERNAL_DECL void duk_log_va(duk_context *ctx, duk_int_t level, const char *fmt, va_list ap);

/*
 *  Debugging
 */

DUK_EXTERNAL_DECL void duk_push_context_dump(duk_context *ctx);

#if defined(DUK_USE_FILE_IO)
/* internal use */
#define duk_dump_context_filehandle(ctx,fh) \
	(duk_push_context_dump((ctx)), \
	 DUK_FPRINTF((fh), "%s\n", duk_safe_to_string(ctx, -1)), \
	 duk_pop(ctx))

/* external use */
#define duk_dump_context_stdout(ctx) \
	duk_dump_context_filehandle((ctx), DUK_STDOUT)
#define duk_dump_context_stderr(ctx) \
	duk_dump_context_filehandle((ctx), DUK_STDERR)
#else  /* DUK_USE_FILE_IO */
#define duk_dump_context_stdout(ctx)  ((void) 0)
#define duk_dump_context_stderr(ctx)  ((void) 0)
#endif  /* DUK_USE_FILE_IO */

/*
 *  Debugger (debug protocol)
 */

DUK_EXTERNAL_DECL void duk_debugger_attach(duk_context *ctx,
                                           duk_debug_read_function read_cb,
                                           duk_debug_write_function write_cb,
                                           duk_debug_peek_function peek_cb,
                                           duk_debug_read_flush_function read_flush_cb,
                                           duk_debug_write_flush_function write_flush_cb,
                                           duk_debug_detached_function detached_cb,
                                           void *udata);
DUK_EXTERNAL_DECL void duk_debugger_detach(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_debugger_cooperate(duk_context *ctx);

/*
 *  C++ name mangling
 */

#ifdef __cplusplus
/* end 'extern "C"' wrapper */
}
#endif

#endif  /* DUK_API_PUBLIC_H_INCLUDED */

/*
 *  END PUBLIC API
 */

/*
 *  Sanity check for the final effective internal defines.  This file also
 *  double checks user tweaks made by an optional duk_custom.h header.
 */

#ifndef DUK_FEATURES_SANITY_H_INCLUDED
#define DUK_FEATURES_SANITY_H_INCLUDED

/*
 *  Deprecated feature options.
 *
 *  Catch so that user more easily notices and updates build.
 */

#if defined(DUK_OPT_NO_FUNC_STMT)
#error DUK_OPT_NO_FUNC_STMT is deprecated, use DUK_OPT_NO_NONSTD_FUNC_STMT
#endif

#if defined(DUK_OPT_FUNC_NONSTD_CALLER_PROPERTY)
#error DUK_OPT_FUNC_NONSTD_CALLER_PROPERTY is deprecated, use DUK_OPT_NONSTD_FUNC_CALLER_PROPERTY
#endif

#if defined(DUK_OPT_FUNC_NONSTD_SOURCE_PROPERTY)
#error DUK_OPT_FUNC_NONSTD_SOURCE_PROPERTY is deprecated, use DUK_OPT_NONSTD_FUNC_SOURCE_PROPERTY
#endif

#if defined(DUK_OPT_NO_ARRAY_SPLICE_NONSTD_DELCOUNT)
#error DUK_OPT_NO_ARRAY_SPLICE_NONSTD_DELCOUNT is deprecated, use DUK_OPT_NO_NONSTD_ARRAY_SPLICE_DELCOUNT
#endif

#if defined(DUK_OPT_NO_OBJECT_ES6_PROTO_PROPERTY)
#error DUK_OPT_NO_OBJECT_ES6_PROTO_PROPERTY is deprecated, use DUK_OPT_NO_ES6_OBJECT_PROTO_PROPERTY
#endif

#if defined(DUK_OPT_NO_OBJECT_ES6_SETPROTOTYPEOF)
#error DUK_OPT_NO_OBJECT_ES6_SETPROTOTYPEOF is deprecated, use DUK_OPT_NO_ES6_OBJECT_SETPROTOTYPEOF
#endif

#if defined(DUK_OPT_NO_JSONX)
#error DUK_OPT_NO_JSONX is deprecated, use DUK_OPT_NO_JX
#endif

#if defined(DUK_OPT_NO_JSONC)
#error DUK_OPT_NO_JSONC is deprecated, use DUK_OPT_NO_JC
#endif

/*
 *  Debug print consistency
 */

#if defined(DUK_USE_DPRINT) && !defined(DUK_USE_DEBUG)
#error DUK_USE_DPRINT without DUK_USE_DEBUG
#endif

#if defined(DUK_USE_DDPRINT) && !defined(DUK_USE_DEBUG)
#error DUK_USE_DDPRINT without DUK_USE_DEBUG
#endif

#if defined(DUK_USE_DDDPRINT) && !defined(DUK_USE_DEBUG)
#error DUK_USE_DDDPRINT without DUK_USE_DEBUG
#endif

#if defined(DUK_USE_HEAPPTR16) && defined(DUK_USE_DEBUG)
/* Debug code doesn't have access to 'heap' so it cannot decode pointers. */
#error debug printing cannot currently be used with heap pointer compression
#endif

/*
 *  Debugger consistency
 */

#if defined(DUK_USE_DEBUGGER_SUPPORT)
#if !defined(DUK_USE_INTERRUPT_COUNTER)
#error DUK_USE_INTERRUPT_COUNTER is needed when debugger support is enabled
#endif
#if !defined(DUK_USE_PC2LINE)
#error DUK_USE_PC2LINE is needed when debugger support is enabled
#endif
#endif

/*
 *  Garbage collection consistency
 */

#if defined(DUK_USE_REFERENCE_COUNTING) && !defined(DUK_USE_DOUBLE_LINKED_HEAP)
#error DUK_USE_REFERENCE_COUNTING defined without DUK_USE_DOUBLE_LINKED_HEAP
#endif

#if defined(DUK_USE_GC_TORTURE) && !defined(DUK_USE_MARK_AND_SWEEP)
#error DUK_USE_GC_TORTURE defined without DUK_USE_MARK_AND_SWEEP
#endif

/*
 *  Low memory feature consistency
 */

#if defined(DUK_USE_OBJSIZES16)
#if defined(DUK_USE_HOBJECT_HASH_PART)
#error DUK_USE_OBJSIZES16 assumes DUK_USE_HOBJECT_HASH_PART is not defined
#endif
#endif

#if defined(DUK_USE_STRTAB_CHAIN) && defined(DUK_USE_STRTAB_PROBE)
#error both DUK_USE_STRTAB_CHAIN and DUK_USE_STRTAB_PROBE defined
#endif
#if !defined(DUK_USE_STRTAB_CHAIN) && !defined(DUK_USE_STRTAB_PROBE)
#error neither DUK_USE_STRTAB_CHAIN nor DUK_USE_STRTAB_PROBE is defined
#endif

#endif  /* DUK_FEATURES_SANITY_H_INCLUDED */

/*
 *  Union to access IEEE double memory representation, indexes for double
 *  memory representation, and some macros for double manipulation.
 *
 *  Also used by packed duk_tval.  Use a union for bit manipulation to
 *  minimize aliasing issues in practice.  The C99 standard does not
 *  guarantee that this should work, but it's a very widely supported
 *  practice for low level manipulation.
 *
 *  IEEE double format summary:
 *
 *    seeeeeee eeeeffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff
 *       A        B        C        D        E        F        G        H
 *
 *    s       sign bit
 *    eee...  exponent field
 *    fff...  fraction
 *
 *  See http://en.wikipedia.org/wiki/Double_precision_floating-point_format.
 *
 *  NaNs are represented as exponent 0x7ff and mantissa != 0.  The NaN is a
 *  signaling NaN when the highest bit of the mantissa is zero, and a quiet
 *  NaN when the highest bit is set.
 *
 *  At least three memory layouts are relevant here:
 *
 *    A B C D E F G H    Big endian (e.g. 68k)           DUK_USE_DOUBLE_BE
 *    H G F E D C B A    Little endian (e.g. x86)        DUK_USE_DOUBLE_LE
 *    D C B A H G F E    Mixed/cross endian (e.g. ARM)   DUK_USE_DOUBLE_ME
 *
 *  ARM is a special case: ARM double values are in mixed/cross endian
 *  format while ARM duk_uint64_t values are in standard little endian
 *  format (H G F E D C B A).  When a double is read as a duk_uint64_t
 *  from memory, the register will contain the (logical) value
 *  E F G H A B C D.  This requires some special handling below.
 *
 *  Indexes of various types (8-bit, 16-bit, 32-bit) in memory relative to
 *  the logical (big endian) order:
 *
 *  byte order      duk_uint8_t    duk_uint16_t     duk_uint32_t
 *    BE             01234567         0123               01
 *    LE             76543210         3210               10
 *    ME (ARM)       32107654         1032               01
 *
 *  Some processors may alter NaN values in a floating point load+store.
 *  For instance, on X86 a FLD + FSTP may convert a signaling NaN to a
 *  quiet one.  This is catastrophic when NaN space is used in packed
 *  duk_tval values.  See: misc/clang_aliasing.c.
 */

#ifndef DUK_DBLUNION_H_INCLUDED
#define DUK_DBLUNION_H_INCLUDED

/*
 *  Union for accessing double parts, also serves as packed duk_tval
 */

union duk_double_union {
	double d;
#ifdef DUK_USE_64BIT_OPS
	duk_uint64_t ull[1];
#endif
	duk_uint32_t ui[2];
	duk_uint16_t us[4];
	duk_uint8_t uc[8];
#ifdef DUK_USE_PACKED_TVAL_POSSIBLE
	void *vp[2];  /* used by packed duk_tval, assumes sizeof(void *) == 4 */
#endif
};

typedef union duk_double_union duk_double_union;

/*
 *  Indexes of various types with respect to big endian (logical) layout
 */

#if defined(DUK_USE_DOUBLE_LE)
#ifdef DUK_USE_64BIT_OPS
#define DUK_DBL_IDX_ULL0   0
#endif
#define DUK_DBL_IDX_UI0    1
#define DUK_DBL_IDX_UI1    0
#define DUK_DBL_IDX_US0    3
#define DUK_DBL_IDX_US1    2
#define DUK_DBL_IDX_US2    1
#define DUK_DBL_IDX_US3    0
#define DUK_DBL_IDX_UC0    7
#define DUK_DBL_IDX_UC1    6
#define DUK_DBL_IDX_UC2    5
#define DUK_DBL_IDX_UC3    4
#define DUK_DBL_IDX_UC4    3
#define DUK_DBL_IDX_UC5    2
#define DUK_DBL_IDX_UC6    1
#define DUK_DBL_IDX_UC7    0
#define DUK_DBL_IDX_VP0    DUK_DBL_IDX_UI0  /* packed tval */
#define DUK_DBL_IDX_VP1    DUK_DBL_IDX_UI1  /* packed tval */
#elif defined(DUK_USE_DOUBLE_BE)
#ifdef DUK_USE_64BIT_OPS
#define DUK_DBL_IDX_ULL0   0
#endif
#define DUK_DBL_IDX_UI0    0
#define DUK_DBL_IDX_UI1    1
#define DUK_DBL_IDX_US0    0
#define DUK_DBL_IDX_US1    1
#define DUK_DBL_IDX_US2    2
#define DUK_DBL_IDX_US3    3
#define DUK_DBL_IDX_UC0    0
#define DUK_DBL_IDX_UC1    1
#define DUK_DBL_IDX_UC2    2
#define DUK_DBL_IDX_UC3    3
#define DUK_DBL_IDX_UC4    4
#define DUK_DBL_IDX_UC5    5
#define DUK_DBL_IDX_UC6    6
#define DUK_DBL_IDX_UC7    7
#define DUK_DBL_IDX_VP0    DUK_DBL_IDX_UI0  /* packed tval */
#define DUK_DBL_IDX_VP1    DUK_DBL_IDX_UI1  /* packed tval */
#elif defined(DUK_USE_DOUBLE_ME)
#ifdef DUK_USE_64BIT_OPS
#define DUK_DBL_IDX_ULL0   0  /* not directly applicable, byte order differs from a double */
#endif
#define DUK_DBL_IDX_UI0    0
#define DUK_DBL_IDX_UI1    1
#define DUK_DBL_IDX_US0    1
#define DUK_DBL_IDX_US1    0
#define DUK_DBL_IDX_US2    3
#define DUK_DBL_IDX_US3    2
#define DUK_DBL_IDX_UC0    3
#define DUK_DBL_IDX_UC1    2
#define DUK_DBL_IDX_UC2    1
#define DUK_DBL_IDX_UC3    0
#define DUK_DBL_IDX_UC4    7
#define DUK_DBL_IDX_UC5    6
#define DUK_DBL_IDX_UC6    5
#define DUK_DBL_IDX_UC7    4
#define DUK_DBL_IDX_VP0    DUK_DBL_IDX_UI0  /* packed tval */
#define DUK_DBL_IDX_VP1    DUK_DBL_IDX_UI1  /* packed tval */
#else
#error internal error
#endif

/*
 *  Helper macros for reading/writing memory representation parts, used
 *  by duk_numconv.c and duk_tval.h.
 */

#define DUK_DBLUNION_SET_DOUBLE(u,v)  do {  \
		(u)->d = (v); \
	} while (0)

#define DUK_DBLUNION_SET_HIGH32(u,v)  do {  \
		(u)->ui[DUK_DBL_IDX_UI0] = (duk_uint32_t) (v); \
	} while (0)

#ifdef DUK_USE_64BIT_OPS
#ifdef DUK_USE_DOUBLE_ME
#define DUK_DBLUNION_SET_HIGH32_ZERO_LOW32(u,v)  do { \
		(u)->ull[DUK_DBL_IDX_ULL0] = (duk_uint64_t) (v); \
	} while (0)
#else
#define DUK_DBLUNION_SET_HIGH32_ZERO_LOW32(u,v)  do { \
		(u)->ull[DUK_DBL_IDX_ULL0] = ((duk_uint64_t) (v)) << 32; \
	} while (0)
#endif
#else  /* DUK_USE_64BIT_OPS */
#define DUK_DBLUNION_SET_HIGH32_ZERO_LOW32(u,v)  do { \
		(u)->ui[DUK_DBL_IDX_UI0] = (duk_uint32_t) (v); \
		(u)->ui[DUK_DBL_IDX_UI1] = (duk_uint32_t) 0; \
	} while (0)
#endif  /* DUK_USE_64BIT_OPS */

#define DUK_DBLUNION_SET_LOW32(u,v)  do {  \
		(u)->ui[DUK_DBL_IDX_UI1] = (duk_uint32_t) (v); \
	} while (0)

#define DUK_DBLUNION_GET_DOUBLE(u)  ((u)->d)
#define DUK_DBLUNION_GET_HIGH32(u)  ((u)->ui[DUK_DBL_IDX_UI0])
#define DUK_DBLUNION_GET_LOW32(u)   ((u)->ui[DUK_DBL_IDX_UI1])

#ifdef DUK_USE_64BIT_OPS
#ifdef DUK_USE_DOUBLE_ME
#define DUK_DBLUNION_SET_UINT64(u,v)  do { \
		(u)->ui[DUK_DBL_IDX_UI0] = (duk_uint32_t) ((v) >> 32); \
		(u)->ui[DUK_DBL_IDX_UI1] = (duk_uint32_t) (v); \
	} while (0)
#define DUK_DBLUNION_GET_UINT64(u) \
	((((duk_uint64_t) (u)->ui[DUK_DBL_IDX_UI0]) << 32) | \
	 ((duk_uint64_t) (u)->ui[DUK_DBL_IDX_UI1]))
#else
#define DUK_DBLUNION_SET_UINT64(u,v)  do { \
		(u)->ull[DUK_DBL_IDX_ULL0] = (duk_uint64_t) (v); \
	} while (0)
#define DUK_DBLUNION_GET_UINT64(u)  ((u)->ull[DUK_DBL_IDX_ULL0])
#endif
#define DUK_DBLUNION_SET_INT64(u,v) DUK_DBLUNION_SET_UINT64((u), (duk_uint64_t) (v))
#define DUK_DBLUNION_GET_INT64(u)   ((duk_int64_t) DUK_DBLUNION_GET_UINT64((u)))
#endif  /* DUK_USE_64BIT_OPS */

/*
 *  Double NaN manipulation macros related to NaN normalization needed when
 *  using the packed duk_tval representation.  NaN normalization is necessary
 *  to keep double values compatible with the duk_tval format.
 *
 *  When packed duk_tval is used, the NaN space is used to store pointers
 *  and other tagged values in addition to NaNs.  Actual NaNs are normalized
 *  to a specific format.  The macros below are used by the implementation
 *  to check and normalize NaN values when they might be created.  The macros
 *  are essentially NOPs when the non-packed duk_tval representation is used.
 *
 *  A FULL check is exact and checks all bits.  A NOTFULL check is used by
 *  the packed duk_tval and works correctly for all NaNs except those that
 *  begin with 0x7ff0.  Since the 'normalized NaN' values used with packed
 *  duk_tval begin with 0x7ff8, the partial check is reliable when packed
 *  duk_tval is used.
 *
 *  The ME variant below is specifically for ARM byte order, which has the
 *  feature that while doubles have a mixed byte order (32107654), unsigned
 *  long long values has a little endian byte order (76543210).  When writing
 *  a logical double value through a ULL pointer, the 32-bit words need to be
 *  swapped; hence the #ifdefs below for ULL writes with DUK_USE_DOUBLE_ME.
 *  This is not full ARM support but suffices for some environments.
 */

#ifdef DUK_USE_64BIT_OPS
#ifdef DUK_USE_DOUBLE_ME
#define DUK__DBLUNION_SET_NAN_FULL(u)  do { \
		(u)->ull[DUK_DBL_IDX_ULL0] = 0x000000007ff80000ULL; \
	} while (0)
#else
#define DUK__DBLUNION_SET_NAN_FULL(u)  do { \
		(u)->ull[DUK_DBL_IDX_ULL0] = 0x7ff8000000000000ULL; \
	} while (0)
#endif
#else  /* DUK_USE_64BIT_OPS */
#define DUK__DBLUNION_SET_NAN_FULL(u)  do { \
		(u)->ui[DUK_DBL_IDX_UI0] = (duk_uint32_t) 0x7ff80000UL; \
		(u)->ui[DUK_DBL_IDX_UI1] = (duk_uint32_t) 0x00000000UL; \
	} while (0)
#endif  /* DUK_USE_64BIT_OPS */

#define DUK__DBLUNION_SET_NAN_NOTFULL(u)  do { \
		(u)->us[DUK_DBL_IDX_US0] = 0x7ff8UL; \
	} while (0)

#ifdef DUK_USE_64BIT_OPS
#ifdef DUK_USE_DOUBLE_ME
#define DUK__DBLUNION_IS_NAN_FULL(u) \
	/* E == 0x7ff, F != 0 => NaN */ \
	((((u)->us[DUK_DBL_IDX_US0] & 0x7ff0UL) == 0x7ff0UL) && \
	 ((((u)->ull[DUK_DBL_IDX_ULL0]) & 0xffffffff000fffffULL) != 0))
#else
#define DUK__DBLUNION_IS_NAN_FULL(u) \
	/* E == 0x7ff, F != 0 => NaN */ \
	((((u)->us[DUK_DBL_IDX_US0] & 0x7ff0UL) == 0x7ff0UL) && \
	 ((((u)->ull[DUK_DBL_IDX_ULL0]) & 0x000fffffffffffffULL) != 0))
#endif
#else  /* DUK_USE_64BIT_OPS */
#define DUK__DBLUNION_IS_NAN_FULL(u) \
	/* E == 0x7ff, F != 0 => NaN */ \
	((((u)->ui[DUK_DBL_IDX_UI0] & 0x7ff00000UL) == 0x7ff00000UL) && \
	 (((u)->ui[DUK_DBL_IDX_UI0] & 0x000fffffUL) != 0 || \
          (u)->ui[DUK_DBL_IDX_UI1] != 0))
#endif  /* DUK_USE_64BIT_OPS */

#define DUK__DBLUNION_IS_NAN_NOTFULL(u) \
	/* E == 0x7ff, topmost four bits of F != 0 => assume NaN */ \
	((((u)->us[DUK_DBL_IDX_US0] & 0x7ff0UL) == 0x7ff0UL) && \
	 (((u)->us[DUK_DBL_IDX_US0] & 0x000fUL) != 0x0000UL))

#ifdef DUK_USE_64BIT_OPS
#ifdef DUK_USE_DOUBLE_ME
#define DUK__DBLUNION_IS_NORMALIZED_NAN_FULL(u) \
	((u)->ull[DUK_DBL_IDX_ULL0] == 0x000000007ff80000ULL)
#else
#define DUK__DBLUNION_IS_NORMALIZED_NAN_FULL(u) \
	((u)->ull[DUK_DBL_IDX_ULL0] == 0x7ff8000000000000ULL)
#endif
#else  /* DUK_USE_64BIT_OPS */
#define DUK__DBLUNION_IS_NORMALIZED_NAN_FULL(u) \
	(((u)->ui[DUK_DBL_IDX_UI0] == 0x7ff80000UL) && \
	 ((u)->ui[DUK_DBL_IDX_UI1] == 0x00000000UL))
#endif  /* DUK_USE_64BIT_OPS */

#define DUK__DBLUNION_IS_NORMALIZED_NAN_NOTFULL(u) \
	/* E == 0x7ff, F == 8 => normalized NaN */ \
	((u)->us[DUK_DBL_IDX_US0] == 0x7ff8UL)

#define DUK__DBLUNION_NORMALIZE_NAN_CHECK_FULL(u)  do { \
		if (DUK__DBLUNION_IS_NAN_FULL((u))) { \
			DUK__DBLUNION_SET_NAN_FULL((u)); \
		} \
	} while (0)

#define DUK__DBLUNION_NORMALIZE_NAN_CHECK_NOTFULL(u)  do { \
		if (DUK__DBLUNION_IS_NAN_NOTFULL((u))) { \
			DUK__DBLUNION_SET_NAN_NOTFULL((u)); \
		} \
	} while (0)

/* Concrete macros for NaN handling used by the implementation internals.
 * Chosen so that they match the duk_tval representation: with a packed
 * duk_tval, ensure NaNs are properly normalized; with a non-packed duk_tval
 * these are essentially NOPs.
 */

#if defined(DUK_USE_PACKED_TVAL)
#if defined(DUK_USE_FULL_TVAL)
#define DUK_DBLUNION_NORMALIZE_NAN_CHECK(u)  DUK__DBLUNION_NORMALIZE_NAN_CHECK_FULL((u))
#define DUK_DBLUNION_IS_NAN(u)               DUK__DBLUNION_IS_NAN_FULL((u))
#define DUK_DBLUNION_IS_NORMALIZED_NAN(u)    DUK__DBLUNION_IS_NORMALIZED_NAN_FULL((u))
#define DUK_DBLUNION_SET_NAN(d)              DUK__DBLUNION_SET_NAN_FULL((d))
#else
#define DUK_DBLUNION_NORMALIZE_NAN_CHECK(u)  DUK__DBLUNION_NORMALIZE_NAN_CHECK_NOTFULL((u))
#define DUK_DBLUNION_IS_NAN(u)               DUK__DBLUNION_IS_NAN_NOTFULL((u))
#define DUK_DBLUNION_IS_NORMALIZED_NAN(u)    DUK__DBLUNION_IS_NORMALIZED_NAN_NOTFULL((u))
#define DUK_DBLUNION_SET_NAN(d)              DUK__DBLUNION_SET_NAN_NOTFULL((d))
#endif
#define DUK_DBLUNION_IS_NORMALIZED(u) \
	(!DUK_DBLUNION_IS_NAN((u)) ||  /* either not a NaN */ \
	 DUK_DBLUNION_IS_NORMALIZED_NAN((u)))  /* or is a normalized NaN */
#else  /* DUK_USE_PACKED_TVAL */
#define DUK_DBLUNION_NORMALIZE_NAN_CHECK(u)  /* nop: no need to normalize */
#define DUK_DBLUNION_IS_NAN(u)               (DUK_ISNAN((u)->d))
#define DUK_DBLUNION_IS_NORMALIZED_NAN(u)    (DUK_ISNAN((u)->d))
#define DUK_DBLUNION_IS_NORMALIZED(u)        1  /* all doubles are considered normalized */
#define DUK_DBLUNION_SET_NAN(u)  do { \
		/* in non-packed representation we don't care about which NaN is used */ \
		(u)->d = DUK_DOUBLE_NAN; \
	} while (0)
#endif  /* DUK_USE_PACKED_TVAL */

/* Byteswap an (aligned) duk_double_union. */
#if defined(DUK_USE_DOUBLE_LE)
#define DUK_DBLUNION_BSWAP(u) do { \
		duk_uint32_t duk__bswaptmp1, duk__bswaptmp2; \
		duk__bswaptmp1 = (u)->ui[0]; \
		duk__bswaptmp2 = (u)->ui[1]; \
		duk__bswaptmp1 = DUK_BSWAP32(duk__bswaptmp1); \
		duk__bswaptmp2 = DUK_BSWAP32(duk__bswaptmp2); \
		(u)->ui[0] = duk__bswaptmp2; \
		(u)->ui[1] = duk__bswaptmp1; \
	} while (0)
#elif defined(DUK_USE_DOUBLE_ME)
#define DUK_DBLUNION_BSWAP(u) do { \
		duk_uint32_t duk__bswaptmp1, duk__bswaptmp2; \
		duk__bswaptmp1 = (u)->ui[0]; \
		duk__bswaptmp2 = (u)->ui[1]; \
		duk__bswaptmp1 = DUK_BSWAP32(duk__bswaptmp1); \
		duk__bswaptmp2 = DUK_BSWAP32(duk__bswaptmp2); \
		(u)->ui[0] = duk__bswaptmp1; \
		(u)->ui[1] = duk__bswaptmp2; \
	} while (0)
#elif defined(DUK_USE_DOUBLE_BE)
#define DUK_DBLUNION_BSWAP(u) do { } while (0)
#else
#error internal error, double endianness insane
#endif

#endif  /* DUK_DBLUNION_H_INCLUDED */

#endif  /* DUKTAPE_H_INCLUDED */
