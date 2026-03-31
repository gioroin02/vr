#ifndef VR_BASE_COMPILER_H
#define VR_BASE_COMPILER_H

#define VR_COMPILER_NONE  0
#define VR_COMPILER_GCC   1
#define VR_COMPILER_CLANG 2
#define VR_COMPILER_MSVC  3

/*
    Found at: https://sourceforge.net/p/predef/wiki/Compilers/
*/

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

#if !defined vr_debug_break

    #if VR_COMPILER == VR_COMPILER_GCC || VR_COMPILER == VR_COMPILER_CLANG

        #define vr_debug_break() __builtin_trap()

    #elif VR_COMPILER == VR_COMPILER_MSVC

        #define vr_debug_break() __debugbreak()

    #else

        #define vr_debug_break() (*(volatile int*) NULL = 0)

    #endif

#endif

/*
    Enum: VR_Compiler

    Elenca i compilatori che la libreria tenta di rilevare.

    Cases:
        VR_Compiler_None  - Indica generalmente un compilatore non rilevato o invalido
        VR_Compiler_GCC   - Compilatore GCC o compatibile
        VR_Compiler_Clang - Compilatore Clang o compatibile
        VR_Compiler_MSVC  - Compilatore MSVC o compatibile

    See Also:
        <vr_get_compiler>
*/
typedef enum
{
    VR_Compiler_None  = VR_COMPILER_NONE,
    VR_Compiler_GCC   = VR_COMPILER_GCC,
    VR_Compiler_Clang = VR_COMPILER_CLANG,
    VR_Compiler_MSVC  = VR_COMPILER_MSVC,
}
VR_Compiler;

/*
    Function: vr_get_compiler

    Returns:
        Il compilatore rilevato dalla libreria.

        Se restituisce VR_Compiler_None significa che la libreria non è stata
        in grado di rilevare un compilatore valido.

    See Also:
        <VR_Compiler>
*/
VR_Compiler vr_get_compiler();

#endif
