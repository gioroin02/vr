#ifndef VR_BASE_DEFINE
#define VR_BASE_DEFINE

#if !defined NULL

    #define NULL ((void*) 0)

#endif

#define vr_static_assert(expr, str) \
    extern char ro_static_assert_casse[(expr) ? 1 : -1]

#define vr_min(x, y) ((x) < (y) ? (x) : (y))
#define vr_max(x, y) ((x) < (y) ? (y) : (x))

#define vr_clamp(x, min, max) vr_max(min, vr_min(x, max))

#define vr_clamp_top(x, max) vr_min(x, max)
#define vr_clamp_bot(x, min) vr_max(x, min)

#endif
