#include <vr_base_platform.h>

#include <stdio.h>

const VrChar8* vr_word_size_name(VrWordSize word_size)
{
    switch (word_size) {
        case VrWordSize_None: { return "None"; } break;
        case VrWordSize_64:   { return "64";   } break;
        case VrWordSize_32:   { return "32";   } break;

        default: break;
    }

    return "<?>";
}

const VrChar8* vr_endian_name(VrEndian endian)
{
    switch (endian) {
        case VrEndian_None:   { return "None";   } break;
        case VrEndian_Little: { return "Little"; } break;
        case VrEndian_Big:    { return "Big";    } break;

        default: break;
    }

    return "<?>";
}

const VrChar8* vr_system_name(VrSystem system)
{
    switch (system) {
        case VrSystem_None:    { return "None";    } break;
        case VrSystem_Windows: { return "Windows"; } break;
        case VrSystem_Linux:   { return "Linux";   } break;

        default: break;
    }

    return "<?>";
}

const VrChar8* vr_compiler_name(VrCompiler compiler)
{
    switch (compiler) {
        case VrCompiler_None:  { return "None";  } break;
        case VrCompiler_GCC:   { return "GCC";   } break;
        case VrCompiler_Clang: { return "Clang"; } break;
        case VrCompiler_MSVC:  { return "MSVC";  } break;

        default: break;
    }

    return "<?>";
}

int main(void)
{
    printf("Platform:\n");
    printf("    word size = VrWordSize.%s\n", vr_word_size_name(vr_platform_word_size()));
    printf("    endian    = VrEndian.%s\n",   vr_endian_name(vr_platform_endian()));
    printf("    system    = VrSystem.%s\n",   vr_system_name(vr_platform_system()));
    printf("    compiler  = VrCompiler.%s\n", vr_compiler_name(vr_platform_compiler()));

    return 0;
}
