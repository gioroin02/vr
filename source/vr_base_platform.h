#ifndef VR_BASE_PLATFORM_H
#define VR_BASE_PLATFORM_H

#if !defined NULL

    #define NULL ((void*) 0)

#endif

#define VR_WORD_SIZE_NONE 0
#define VR_WORD_SIZE_32   1
#define VR_WORD_SIZE_64   2

#define VR_COMPILER_NONE  0
#define VR_COMPILER_GCC   1
#define VR_COMPILER_CLANG 2
#define VR_COMPILER_MSVC  3

#define VR_SYSTEM_NONE    0
#define VR_SYSTEM_WINDOWS 1
#define VR_SYSTEM_LINUX   2

// Found at: https://sourceforge.net/p/predef/wiki/Architectures/
// Found at: https://sourceforge.net/p/predef/wiki/OperatingSystems/

#if !defined VR_WORD_SIZE

    #if defined _WIN64      || defined __LP64__    || defined _LP64       || \
        (defined __x86_64__ && !defined __ILP32__) || defined __ia64__    || \
        defined __ia64      || defined __IA64__    || defined _IA64       || \
        defined _M_IA64     || defined __amd64__   || defined __amd64     || \
        defined _M_AMD64    || defined __aarch64__

        #define VR_WORD_SIZE VR_WORD_SIZE_64

    #elif defined _WIN32   || defined __ILP32__ || defined __i386   || \
          defined __i386   || defined i386      || defined __i486__ || \
          defined __i586__ || defined __i686__  || defined __IA32__ || \
          defined _M_I86   || defined _M_IX86   || defined __X86__  || \
          defined _X86_    || defined __I86__   || defined __386    || \
          defined __arm__  || defined __arm     || defined _ARM     || \
          defined _M_ARM   || defined _M_ARMT

        #define VR_WORD_SIZE VR_WORD_SIZE_32

    #else

        #define VR_WORD_SIZE VR_WORD_SIZE_NONE

    #endif

#endif

// Found at: https://sourceforge.net/p/predef/wiki/Compilers/

#if !defined VR_COMPILER

    #if defined __clang__

        #define VR_COMPILER VR_COMPILER_CLANG

    #elif defined __GNUC__

        #define VR_COMPILER VR_COMPILER_GCC

    #elif defined _MSC_VER

        #define VR_COMPILER VR_COMPILER_MSVC

    #else

        #define VR_COMPILER VR_COMPILER_NONE

    #endif

#endif

// Found at: https://sourceforge.net/p/predef/wiki/OperatingSystems/

#if !defined VR_SYSTEM

    #if defined __WIN32__   || defined _WIN32 || defined _WIN64 || \
        defined __WINDOWS__ || defined __TOS_WIN__

        #define VR_SYSTEM VR_SYSTEM_WINDOWS

        #define VR_WORD_SIZE_64_IS_LONG_LONG

    #elif defined __linux__ || defined __gnu_linux__

        #define VR_SYSTEM VR_SYSTEM_LINUX

        #define VR_WORD_SIZE_64_IS_LONG
        #define _DEFAULT_SOURCE

    #else

        #define VR_SYSTEM VR_SYSTEM_NONE

    #endif

#endif

#if !defined vr_debug_break

    #if VR_COMPILER == VR_COMPILER_GCC || VR_COMPILER == VR_COMPILER_CLANG

        #define vr_debug_break() __builtin_trap()

    #elif VR_COMPILER == VR_COMPILER_MSVC

        #define vr_debug_break() __debugbreak()

    #else

        #define vr_debug_break() (*(volatile int*) NULL = 0)

    #endif

#endif

#define vr_static_assert(expr) \
    extern char vr_static_assert_case[(expr) ? 1 : -1]

#define vr_assert(expr) if (!(expr)) vr_debug_break()

#define vr_min(a, b) ((a) < (b) ? (a) : (b))
#define vr_max(a, b) ((a) < (b) ? (b) : (a))

#if defined VR_WORD_SIZE_64_IS_LONG_LONG

    typedef unsigned long long uint64;
    typedef unsigned       int uint32;
    typedef unsigned     short uint16;
    typedef unsigned      char uint8;

    typedef signed long long int64;
    typedef signed       int int32;
    typedef signed     short int16;
    typedef signed      char int8;

    #define VR_UINT64_MAX ((uint64) 18446744073709551615llu)
    #define VR_UINT32_MAX ((uint32) 4294967295u)
    #define VR_UINT16_MAX ((uint16) 65535u)
    #define VR_UINT8_MAX  ((uint8)  255u)

    #define VR_UINT64_MIN ((uint64) 0)
    #define VR_UINT32_MIN ((uint32) 0)
    #define VR_UINT16_MIN ((uint16) 0)
    #define VR_UINT8_MIN  ((uint8)  0)

    #define VR_INT64_MAX ((int64) 9223372036854775807ll)
    #define VR_INT32_MAX ((int32) 2147483647)
    #define VR_INT16_MAX ((int16) 32767)
    #define VR_INT8_MAX  ((int8)  127)

    #define VR_INT64_MIN ((int64) -VR_INT64_MAX - 1)
    #define VR_INT32_MIN ((int32) -VR_INT32_MAX - 1)
    #define VR_INT16_MIN ((int16) -VR_INT16_MAX - 1)
    #define VR_INT8_MIN  ((int8)  -VR_INT8_MAX  - 1)

#elif defined VR_WORD_SIZE_64_IS_LONG

    typedef unsigned  long uint64;
    typedef unsigned   int uint32;
    typedef unsigned short uint16;
    typedef unsigned  char uint8;

    typedef signed  long int64;
    typedef signed   int int32;
    typedef signed short int16;
    typedef signed  char int8;

    #define VR_UINT64_MAX ((uint64) 18446744073709551615lu)
    #define VR_UINT32_MAX ((uint32) 4294967295u)
    #define VR_UINT16_MAX ((uint16) 65535u)
    #define VR_UINT8_MAX  ((uint8)  255u)

    #define VR_UINT64_MIN ((uint64) 0)
    #define VR_UINT32_MIN ((uint32) 0)
    #define VR_UINT16_MIN ((uint16) 0)
    #define VR_UINT8_MIN  ((uint8)  0)

    #define VR_INT64_MAX ((int64) 9223372036854775807l)
    #define VR_INT32_MAX ((int32) 2147483647)
    #define VR_INT16_MAX ((int16) 32767)
    #define VR_INT8_MAX  ((int8)  127)

    #define VR_INT64_MIN ((int64) -VR_INT64_MAX - 1)
    #define VR_INT32_MIN ((int32) -VR_INT32_MAX - 1)
    #define VR_INT16_MIN ((int16) -VR_INT16_MAX - 1)
    #define VR_INT8_MIN  ((int8)  -VR_INT8_MAX  - 1)

#else

    #error "Unknown word size"

#endif

typedef double float64;
typedef float  float32;

typedef uint32 char32;
typedef uint16 char16;
typedef char   char8;

typedef uint64 bool64;
typedef uint32 bool32;
typedef uint16 bool16;
typedef uint8  bool8;

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

#define VR_INTPTR_KILO ((intptr) 1000ll)
#define VR_INTPTR_MEGA ((intptr) 1000000ll)
#define VR_INTPTR_GIGA ((intptr) 1000000000ll)

#define VR_INTPTR_KIBI ((intptr) 1024ll)
#define VR_INTPTR_MEBI ((intptr) 1048576ll)
#define VR_INTPTR_GIBI ((intptr) 1073741824ll)

vr_static_assert(sizeof (uint64) == 8);
vr_static_assert(sizeof (uint32) == 4);
vr_static_assert(sizeof (uint16) == 2);
vr_static_assert(sizeof (uint8)  == 1);

vr_static_assert(sizeof (int64) == 8);
vr_static_assert(sizeof (int32) == 4);
vr_static_assert(sizeof (int16) == 2);
vr_static_assert(sizeof (int8) == 1);

vr_static_assert(sizeof (float64) == 8);
vr_static_assert(sizeof (float32) == 4);

vr_static_assert(sizeof (char32) == 4);
vr_static_assert(sizeof (char16) == 2);
vr_static_assert(sizeof (char8)  == 1);

vr_static_assert(sizeof (bool64) == 8);
vr_static_assert(sizeof (bool32) == 4);
vr_static_assert(sizeof (bool16) == 2);
vr_static_assert(sizeof (bool8)  == 1);

vr_static_assert(sizeof (void*) == sizeof (uintptr));
vr_static_assert(sizeof (void*) == sizeof (intptr));
vr_static_assert(sizeof (void*) == sizeof (void (*) (void)));

typedef enum
{
    VR_Endian_None,
    VR_Endian_Little,
    VR_Endian_Big,
}
VR_Endian;

typedef enum
{
    VR_Word_Size_None = VR_WORD_SIZE_NONE,
    VR_Word_Size_32   = VR_WORD_SIZE_32,
    VR_Word_Size_64   = VR_WORD_SIZE_64,
}
VR_Word_Size;

typedef enum
{
    VR_Compiler_None  = VR_COMPILER_NONE,
    VR_Compiler_GCC   = VR_COMPILER_GCC,
    VR_Compiler_Clang = VR_COMPILER_CLANG,
    VR_Compiler_MSVC  = VR_COMPILER_MSVC,
}
VR_Compiler;

typedef enum
{
    VR_System_None    = VR_SYSTEM_NONE,
    VR_System_Windows = VR_SYSTEM_WINDOWS,
    VR_System_Linux   = VR_SYSTEM_LINUX,
}
VR_System;

#define vr_is_machine_little_endian() (vr_machine_endian() == VR_Endian_Little)
#define vr_is_machine_big_endian()    (vr_machine_endian() == VR_Endian_Big)

VR_Endian vr_machine_endian();

VR_Word_Size vr_machine_word_size();

VR_Compiler vr_current_compiler();

VR_System vr_current_system();

#endif
