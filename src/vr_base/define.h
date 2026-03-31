#ifndef VR_BASE_DEFINE
#define VR_BASE_DEFINE

#if !defined NULL

    #define NULL ((void*) 0)

#endif

#define vr_static_assert(expr, str) \
    extern char vr_static_assert_case[(expr) ? 1 : -1]

#define vr_min(x, y) ((x) < (y) ? (x) : (y))
#define vr_max(x, y) ((x) < (y) ? (y) : (x))

#define vr_limit(x, btm, top) vr_max(btm, vr_min(x, top))

#define vr_limit_top(x, top) vr_min(x, top)
#define vr_limit_btm(x, btm) vr_max(x, btm)

#endif
