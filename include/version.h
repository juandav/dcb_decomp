#ifndef VERSION_H
#define VERSION_H

/*
 * The version of the game being built. The Makefile passes exactly one
 * -DVERSION_<VERSION> (make VERSION=jp gives -DVERSION_JP); here every
 * version's macro becomes 0 or 1, so that code tests them with #if:
 *
 *     #if VERSION_JP
 *     #if VERSION_US || VERSION_EU
 *
 * never with #ifdef or defined(), which can't tell a version that is off
 * from a misspelt one (-Wundef warns about the misspelling in #if). A
 * condition names the versions it is for: the versions have no order.
 * A new version gets its line here and in the Makefile's VERSIONS.
 */
#if defined(VERSION_US) + defined(VERSION_JP) + defined(VERSION_EU) != 1
#error "build with exactly one of -DVERSION_US, -DVERSION_JP and -DVERSION_EU, as make VERSION=<version> does"
#endif

#ifndef VERSION_US
#define VERSION_US 0
#endif
#ifndef VERSION_JP
#define VERSION_JP 0
#endif
#ifndef VERSION_EU
#define VERSION_EU 0
#endif

/*
 * Not a version: a Japanese debug build that the European disc carries
 * (eu's INTSEG and NISSEG), which mk/version/eu.mk compiles as jp's with
 * -DJP_DEBUG_BUILD=1. Its debug code is under #if JP_DEBUG_BUILD.
 */
#ifndef JP_DEBUG_BUILD
#define JP_DEBUG_BUILD 0
#endif

#endif /* VERSION_H */
