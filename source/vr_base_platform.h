#ifndef VR_BASE_PLATFORM_H
#define VR_BASE_PLATFORM_H

#if !defined NULL

    #define NULL ((void*) 0)

#endif

#define vr_static_assert(expr) \
    extern char vr_static_assert_case[(expr) ? 1 : -1]

#define vr_static_array_count(expr) \
    ((VrSint) (sizeof (expr) / (sizeof *(expr))))

#define VR_WORD_SIZE_NONE 0
#define VR_WORD_SIZE_64   1
#define VR_WORD_SIZE_32   2

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

        #define VR_UINT64_TYPE unsigned long long
        #define VR_UINT32_TYPE unsigned int
        #define VR_UINT16_TYPE unsigned short
        #define VR_UINT8_TYPE  unsigned char

        #define VR_SINT64_TYPE signed long long
        #define VR_SINT32_TYPE signed int
        #define VR_SINT16_TYPE signed short
        #define VR_SINT8_TYPE  signed char

    #elif defined __linux__ || defined __gnu_linux__

        #define VR_SYSTEM VR_SYSTEM_LINUX

        #define _DEFAULT_SOURCE

        #define VR_UINT64_TYPE unsigned long
        #define VR_UINT32_TYPE unsigned int
        #define VR_UINT16_TYPE unsigned short
        #define VR_UINT8_TYPE  unsigned char

        #define VR_SINT64_TYPE signed long
        #define VR_SINT32_TYPE signed int
        #define VR_SINT16_TYPE signed short
        #define VR_SINT8_TYPE  signed char

    #else

        #define VR_SYSTEM VR_SYSTEM_NONE

    #endif

#endif

#if defined VR_USE_LIBC_TYPES

    #include <stdint.h>

    typedef uint64_t VrUint64;
    typedef uint32_t VrUint32;
    typedef uint16_t VrUint16;
    typedef uint8_t  VrUint8;

    typedef int64_t VrSint64;
    typedef int32_t VrSint32;
    typedef int16_t VrSint16;
    typedef int8_t  VrSint8;

#elif defined VR_UINT64_TYPE

    typedef VR_UINT64_TYPE VrUint64;
    typedef VR_UINT32_TYPE VrUint32;
    typedef VR_UINT16_TYPE VrUint16;
    typedef VR_UINT8_TYPE  VrUint8;

    typedef VR_SINT64_TYPE VrSint64;
    typedef VR_SINT32_TYPE VrSint32;
    typedef VR_SINT16_TYPE VrSint16;
    typedef VR_SINT8_TYPE  VrSint8;

#else

    #error "Unable to define integer types"

#endif

#define VR_UINT64_MAX ((VrUint64) 18446744073709551615llu)
#define VR_UINT32_MAX ((VrUint32) 4294967295u)
#define VR_UINT16_MAX ((VrUint16) 65535u)
#define VR_UINT8_MAX  ((VrUint8)  255u)

#define VR_UINT64_MIN ((VrUint64) 0)
#define VR_UINT32_MIN ((VrUint32) 0)
#define VR_UINT16_MIN ((VrUint16) 0)
#define VR_UINT8_MIN  ((VrUint8)  0)

#define VR_SINT64_MAX ((VrSint64) 9223372036854775807ll)
#define VR_SINT32_MAX ((VrSint32) 2147483647)
#define VR_SINT16_MAX ((VrSint16) 32767)
#define VR_SINT8_MAX  ((VrSint8)  127)

#define VR_SINT64_MIN ((VrSint64) -VR_SINT64_MAX - 1)
#define VR_SINT32_MIN ((VrSint32) -VR_SINT32_MAX - 1)
#define VR_SINT16_MIN ((VrSint16) -VR_SINT16_MAX - 1)
#define VR_SINT8_MIN  ((VrSint8)  -VR_SINT8_MAX  - 1)

#if VR_WORD_SIZE == VR_WORD_SIZE_64

    typedef VrUint64 VrUint;
    typedef VrSint64 VrSint;

    #define VR_UINT_MAX VR_UINT64_MAX
    #define VR_UINT_MIN VR_UINT64_MIN

    #define VR_SINT_MAX VR_SINT64_MAX
    #define VR_SINT_MIN VR_SINT64_MIN

#else

    typedef VrUint32 VrUint;
    typedef VrSint32 VrSint;

    #define VR_UINT_MAX VR_UINT32_MAX
    #define VR_UINT_MIN VR_UINT32_MIN

    #define VR_SINT_MAX VR_SINT32_MAX
    #define VR_SINT_MIN VR_SINT32_MIN

#endif

#define VR_SINT_KILO_10 ((VrSint) 1000ll)
#define VR_SINT_MEGA_10 ((VrSint) 1000000ll)
#define VR_SINT_GIGA_10 ((VrSint) 1000000000ll)

#define VR_SINT_KILO_2 ((VrSint) 1024ll)
#define VR_SINT_MEGA_2 ((VrSint) 1048576ll)
#define VR_SINT_GIGA_2 ((VrSint) 1073741824ll)

typedef double VrFloat64;
typedef float  VrFloat32;

typedef VrUint32 VrChar32;
typedef VrUint16 VrChar16;
typedef char     VrChar8;

typedef VrUint64 VrBool64;
typedef VrUint32 VrBool32;
typedef VrUint16 VrBool16;
typedef VrUint8  VrBool8;

typedef void (*VrProc) (void);

vr_static_assert(sizeof (VrUint64) == 8);
vr_static_assert(sizeof (VrUint32) == 4);
vr_static_assert(sizeof (VrUint16) == 2);
vr_static_assert(sizeof (VrUint8)  == 1);

vr_static_assert(sizeof (VrSint64) == 8);
vr_static_assert(sizeof (VrSint32) == 4);
vr_static_assert(sizeof (VrSint16) == 2);
vr_static_assert(sizeof (VrSint8)  == 1);

vr_static_assert(sizeof (VrFloat64) == 8);
vr_static_assert(sizeof (VrFloat32) == 4);

vr_static_assert(sizeof (VrChar32) == 4);
vr_static_assert(sizeof (VrChar16) == 2);
vr_static_assert(sizeof (VrChar8)  == 1);

vr_static_assert(sizeof (VrBool64) == 8);
vr_static_assert(sizeof (VrBool32) == 4);
vr_static_assert(sizeof (VrBool16) == 2);
vr_static_assert(sizeof (VrBool8)  == 1);

vr_static_assert(sizeof (void*) == sizeof (VrUint));
vr_static_assert(sizeof (void*) == sizeof (VrSint));
vr_static_assert(sizeof (void*) == sizeof (VrProc));

typedef enum VrEndian
{
    VrEndian_None,
    VrEndian_Little,
    VrEndian_Big,
}
VrEndian;

typedef enum VrWordSize
{
    VrWordSize_None = VR_WORD_SIZE_NONE,
    VrWordSize_64   = VR_WORD_SIZE_64,
    VrWordSize_32   = VR_WORD_SIZE_32,
}
VrWordSize;

typedef enum VrCompiler
{
    VrCompiler_None  = VR_COMPILER_NONE,
    VrCompiler_GCC   = VR_COMPILER_GCC,
    VrCompiler_Clang = VR_COMPILER_CLANG,
    VrCompiler_MSVC  = VR_COMPILER_MSVC,
}
VrCompiler;

typedef enum VrSystem
{
    VrSystem_None    = VR_SYSTEM_NONE,
    VrSystem_Windows = VR_SYSTEM_WINDOWS,
    VrSystem_Linux   = VR_SYSTEM_LINUX,
}
VrSystem;

#define vr_is_platform_little_endian() (vr_platform_endian() == VrEndian_Little)
#define vr_is_platform_big_endian()    (vr_platform_endian() == VrEndian_Big)

VrEndian vr_platform_endian(void);

VrWordSize vr_platform_word_size(void);

VrCompiler vr_platform_compiler(void);

VrSystem vr_platform_system(void);

#endif
