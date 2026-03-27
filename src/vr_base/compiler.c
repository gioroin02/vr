#ifndef VR_BASE_COMPILER_C
#define VR_BASE_COMPILER_C

#include "compiler.h"

VR_Compiler vr_get_compiler()
{
    #if VR_COMPILER == VR_COMPILER_GCC

        return VR_Compiler_GCC;

    #elif VR_COMPILER == VR_COMPILER_CLANG

        return VR_Compiler_Clang;

    #elif VR_COMPILER == VR_COMPILER_MSVC

        return VR_Compiler_MSVC;

    #else

        return VR_Compiler_None;

    #endif
}

#endif
