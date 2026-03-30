#ifndef VR_BASE_MACHINE_H
#define VR_BASE_MACHINE_H

#define VR_WORD_NONE 0
#define VR_WORD_32   1
#define VR_WORD_64   2

#define vr_is_machine_little_endian() (vr_get_machine_endian() == VR_Endian_Little)
#define vr_is_machine_big_endian()    (vr_get_machine_endian() == VR_Endian_Big)

/*
    Found at: https://sourceforge.net/p/predef/wiki/Architectures/
    Found at: https://sourceforge.net/p/predef/wiki/OperatingSystems/
*/

#if !defined VR_WORD

    #if defined _WIN64      || defined __LP64__    || defined _LP64       || \
        (defined __x86_64__ && !defined __ILP32__) || defined __ia64__    || \
        defined __ia64      || defined __IA64__    || defined _IA64       || \
        defined _M_IA64     || defined __amd64__   || defined __amd64     || \
        defined _M_AMD64    || defined __aarch64__

        #define VR_WORD VR_WORD_64

    #elif defined _WIN32   || defined __ILP32__ || defined __i386   || \
          defined __i386   || defined i386      || defined __i486__ || \
          defined __i586__ || defined __i686__  || defined __IA32__ || \
          defined _M_I86   || defined _M_IX86   || defined __X86__  || \
          defined _X86_    || defined __I86__   || defined __386    || \
          defined __arm__  || defined __arm     || defined _ARM     || \
          defined _M_ARM   || defined _M_ARMT

        #define VR_WORD VR_WORD_32

    #else

        #define VR_WORD VR_WORD_NONE

    #endif

#endif

/*
    Enum: VR_Word

    Elenca le dimensioni della parola del processore che la libreria
    tenta di rilevare.

    Cases:
        VR_Word_None - Indica generalmente una parola non rilevata o invalida
        VR_Word_32   - Parola a 32 bit
        VR_Word_64   - Parola a 64 bit

    See Also:
        <vr_get_machine_word>
*/
typedef enum
{
    VR_Word_None = VR_WORD_NONE,
    VR_Word_32   = VR_WORD_32,
    VR_Word_64   = VR_WORD_64,
}
VR_Word;

/*
    Enum: VR_Endian

    Elenca le endianness del processore che la libreria tenta di rilevare.

    Cases:
        VR_Endian_None   - Indica generalmente una endianness non rilevata o invalida
        VR_Endian_Little - Little endian
        VR_Endian_Big    - Big endian

    See Also:
        <vr_get_machine_endian>
*/
typedef enum
{
    VR_Endian_None,
    VR_Endian_Little,
    VR_Endian_Big,
}
VR_Endian;

/*
    Function: vr_get_machine_word

    Returns:
        La parola del processore rilevata dalla libreria.

        Se restituisce VR_Word_None significa che la libreria non è stata
        in grado di rilevare una parola valida.

    See Also:
        <VR_Word>
*/
VR_Word vr_get_machine_word();

/*
    Function: vr_get_machine_endian

    Returns:
        La endianness del processore rilevata dalla libreria.

        Se restituisce VR_Endian_None significa che la libreria non è stata
        in grado di rilevare una endianness valida.

    See Also:
        <VR_Endian>
*/
VR_Endian vr_get_machine_endian();

#endif
