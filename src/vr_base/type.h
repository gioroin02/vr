#ifndef VR_BASE_TYPE_H
#define VR_BASE_TYPE_H

#include "machine.h"
#include "system.h"
#include "define.h"

#if !defined VR_ALLOW_LIBC

    #if VR_SYSTEM == VR_SYSTEM_WINDOWS || defined LLP64

        typedef unsigned long long uint64_t;
        typedef signed   long long int64_t;

    #else

        typedef unsigned long uint64_t;
        typedef signed long   int64_t;

    #endif

    typedef unsigned int   uint32_t;
    typedef unsigned short uint16_t;
    typedef unsigned char  uint8_t;

    typedef signed int   int32_t;
    typedef signed short int16_t;
    typedef signed char  int8_t;

#else

    #define _DEFAULT_SOURCE
    #include <inttypes.h>

#endif

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t  uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t  int8;

typedef double float64;
typedef float  float32;

typedef uint32 char32;
typedef uint16 char16;
typedef char   char8;

typedef uint64 bool64;
typedef uint32 bool32;
typedef uint16 bool16;
typedef uint8  bool8;

#define VR_UINT64_MAX ((uint64) 18446744073709551615ull)
#define VR_UINT32_MAX ((uint32) 4294967295ul)
#define VR_UINT16_MAX ((uint16) 65535u)
#define VR_UINT8_MAX  ((uint8)  255u)

#define VR_UINT64_MIN ((uint64) 0)
#define VR_UINT32_MIN ((uint32) 0)
#define VR_UINT16_MIN ((uint16) 0)
#define VR_UINT8_MIN  ((uint8)  0)

#define VR_INT64_MAX ((int64) 9223372036854775807ll)
#define VR_INT32_MAX ((int32) 2147483647l)
#define VR_INT16_MAX ((int16) 32767)
#define VR_INT8_MAX  ((int8)  127)

#define VR_INT64_MIN (-((int64) 9223372036854775807ll) - 1)
#define VR_INT32_MIN (-((int32) 2147483647l) - 1)
#define VR_INT16_MIN (-((int16) 32767) - 1)
#define VR_INT8_MIN  (-((int8)  127) - 1)

#if VR_WORD == VR_WORD_64

    typedef uint64 uintptr;
    typedef int64  intptr;

    #define VR_UINTPTR_MAX VR_UINT64_MAX
    #define VR_UINTPTR_MIN VR_UINT64_MIN

    #define VR_INTPTR_MAX VR_INT64_MAX
    #define VR_INTPTR_MIN VR_INT64_MIN

#else

    typedef uint32 uintptr;
    typedef int32  intptr;

    #define VR_UINTPTR_MAX VR_UINT32_MAX
    #define VR_UINTPTR_MIN VR_UINT32_MIN

    #define VR_INTPTR_MAX VR_INT32_MAX
    #define VR_INTPTR_MIN VR_INT32_MIN

#endif

vr_static_assert(sizeof (uint64) == 8, "");
vr_static_assert(sizeof (uint32) == 4, "");
vr_static_assert(sizeof (uint16) == 2, "");
vr_static_assert(sizeof (uint8)  == 1, "");

vr_static_assert(sizeof (int64) == 8, "");
vr_static_assert(sizeof (int32) == 4, "");
vr_static_assert(sizeof (int16) == 2, "");
vr_static_assert(sizeof (int8) == 1, "");

vr_static_assert(sizeof (float64) == 8, "");
vr_static_assert(sizeof (float32) == 4, "");

vr_static_assert(sizeof (char32) == 4, "");
vr_static_assert(sizeof (char16) == 2, "");
vr_static_assert(sizeof (char8)  == 1, "");

vr_static_assert(sizeof (bool64) == 8, "");
vr_static_assert(sizeof (bool32) == 4, "");
vr_static_assert(sizeof (bool16) == 2, "");
vr_static_assert(sizeof (bool8)  == 1, "");

vr_static_assert(sizeof (uintptr) == sizeof (void*), "");
vr_static_assert(sizeof (intptr)  == sizeof (void*), "");

vr_static_assert(sizeof (void*) == sizeof (void (*) (void)),
    "Pointer to function must be same size of regular pointer");

#endif
