// bdc 0x08a17064 GmoMotionEvalTrack
#include "bdc.h"

/* Evaluates one motion track at `frame` into `out` for `GmoMotionApplyTrack`. Track flags are
   `param8`: `& 0xf` interpolation mode, `& 0x80` half-float keys, `& 0xf00` pre-infinity and
   `& 0xf000` post-infinity mode. With any infinity bit set the frame is first wrapped against the
   first/last key time (pre: 0/0x300/0x400 clamp, 0x100 repeat, 0x200 mirror; post: 0/0x3000/0x4000
   clamp, 0x1000 repeat, 0x2000 mirror; other values leave it). The surrounding key pair is searched
   from the byte offset cached in `*cursor` (written back). Keys are `(paramC * {1,3,5} + 1)` values
   (time first; floats, or halves at half the stride); `paramA` is the key count (below 2: key 0
   only, t = 0). Then for each group of 4 components, with `t = (frame - t0) / (t1 - t0)` taken
   from the unwrapped `frame`: mode 0 constant, 1 linear, 2 cubic Hermite (`vtfm4`), 4 slerp, any
   other (3) Bezier via `GmoBezierSolveParamB` and the Bernstein basis; each group is stored to
   `out` (4 floats per group). Returns `paramC`, or 0 when `track`, `cursor` or `out` is null or
   `paramC` is 0. */

/* half -> float bit expansion, inlined at every key read in the original */
static inline float GmoTrackHalf(u16 half)
{
    union { u32 u; float f; } bits;
    u32 exp = (half >> 10) & 0x1f;

    if ((half & 0x7fff) != 0) {
        exp += 0x70;
    }
    bits.u = ((half & 0x3ff) << 13) | ((half & 0x8000) << 16) | (exp << 23);
    return bits.f;
}

/* pre/post-infinity wrap of `frame` against [first, last]; inlined twice in the original */
static inline float GmoTrackWrapFrame(float frame, float first, float last, u32 flags)
{
    float rel = frame - first;
    float span = last - first;
    float period;
    float mirror;

    if (rel < 0.0f) {
        switch (flags & 0xf00) {
        case 0x000:
        case 0x300:
        case 0x400:
            rel = 0.0f;
            break;
        case 0x100:
            rel = span + (rel - span * (float)(s32)(rel / span));
            break;
        case 0x200:
            period = span + span;
            rel = period + (rel - period * (float)(s32)(rel / period));
            mirror = period - rel;
            if (mirror < rel) {
                rel = mirror;
            }
            break;
        default:
            break;
        }
    } else if (span <= rel) {
        switch (flags & 0xf000) {
        case 0x0000:
        case 0x3000:
        case 0x4000:
            rel = span;
            break;
        case 0x1000:
            rel = rel - span * (float)(s32)(rel / span);
            break;
        case 0x2000:
            period = span + span;
            rel = rel - period * (float)(s32)(rel / period);
            mirror = period - rel;
            if (mirror < rel) {
                rel = mirror;
            }
            break;
        default:
            break;
        }
    }
    return rel + first;
}

/* mode 1: linear blend a + (b - a)*t (vsub/vscl/vadd) */
static inline void GmoTrackLerp4(float *dst, const float *a, const float *b, float t)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        dst[i] = a[i] + (b[i] - a[i]) * t;
    }
}

/* mode 2: cubic Hermite as a Bezier with handles 3*(c0 + p0), 3*(c1 + p1) and the weights
   (u^3, t^3, t*u^2, u*t^2), u = 1 - t (vtfm4 over the columns p0, p1, h0, h1) */
static inline void GmoTrackHermite4(float *dst, const float *p0, const float *p1, const float *c0,
                                    const float *c1, float t)
{
    float u = 1.0f - t;
    float uu = u * u;
    float tt = t * t;
    float w0 = u * uu;
    float w1 = t * tt;
    float w2 = t * uu;
    float w3 = u * tt;
    s32 i;

    for (i = 0; i < 4; i++) {
        float h0 = (c0[i] + p0[i]) * 3.0f;
        float h1 = (c1[i] + p1[i]) * 3.0f;

        dst[i] = p0[i] * w0 + p1[i] * w1 + h0 * w2 + h1 * w3;
    }
}

/* mode 4: quaternion slerp, as `GmoInterpSlerpF32`: dot `d` clamped to [-1, 1];
   `!(d < 0.998046875f)` (also NaN) blends linearly, `d < -0.998046875f` blends linearly towards
   `-b`; otherwise the shorter arc (b negated when `d < 0`) at `theta = acos|d|` in quarter turns. */
static inline void GmoTrackSlerp4(float *dst, const float *a, const float *key1, float t)
{
    float b[4];
    float diff[4];
    float sum[4];
    float d;
    float s;
    float theta;
    float w0;
    float w1;
    float inv;
    s32 i;

    for (i = 0; i < 4; i++) {
        b[i] = key1[i];
    }
    d = VfSat1(a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
    for (i = 0; i < 4; i++) {
        diff[i] = (b[i] - a[i]) * t;
        sum[i] = (b[i] + a[i]) * t;
    }
    s = 1.0f - d * d;
    if (d < 0.0f) {
        for (i = 0; i < 4; i++) {
            b[i] = -b[i];
        }
    }
    if (!(d < 0.998046875f)) {
        for (i = 0; i < 4; i++) {
            dst[i] = a[i] + diff[i];
        }
    } else if (d < -0.998046875f) {
        for (i = 0; i < 4; i++) {
            dst[i] = a[i] - sum[i];
        }
    } else {
        if (__builtin_fabsf(d) < 0.707106769f) {
            theta = 1.0f - __builtin_fabsf(VfAsinQuarter(d));
        } else {
            theta = VfAsinQuarter(__builtin_sqrtf(s));
        }
        w0 = VfSinQuarter((1.0f - t) * theta);
        w1 = VfSinQuarter(t * theta);
        inv = VfRcp(VfSinQuarter(theta));
        for (i = 0; i < 4; i++) {
            dst[i] = (a[i] * w0 + b[i] * w1) * inv;
        }
    }
}

u32 GmoMotionEvalTrack(float frame, const GmoMotionTrack *track, u16 *cursor, float *out)
{
    const GmoMotionTrack *trk = track;
    float tmp[4];
    float va[4];
    float vb[4];
    float vc[4];
    float vd[4];
    const u8 *keys;
    const u8 *last;
    const u8 *key;
    const u8 *key1;
    u32 flags;
    u32 mode;
    u32 group;
    u32 values;
    u32 count;
    u32 n;
    s32 stride;
    s32 step;
    s32 i;
    s32 j;
    float t;
    float t0;
    float t1;

    if (track == NULL || cursor == NULL || out == NULL) {
        return 0;
    }
    flags = trk->param8;
    mode = flags & 0xf;
    if (mode == 2) {
        group = 3;
    } else if (mode == 3) {
        group = 5;
    } else {
        group = 1;
    }
    count = trk->paramA;

    if ((flags & 0x80) != 0) {
        /* half-float keys: time in the leading half */
        if (count < 2) {
            t0 = 0.0f;
            key = (const u8 *)PspPtr(trk->data);
            t1 = t0;
            key1 = key;
        } else {
            values = trk->paramC;
            if (mode == 2) {
                values = values * 3;
            } else if (mode == 3) {
                values = values * 5;
            }
            keys = (const u8 *)PspPtr(trk->data);
            stride = (s32)(values * 4 + 4) >> 1;
            last = keys + stride * (s32)(count - 1);
            key = keys + *cursor;
            t = frame;
            if ((flags & 0xff00) != 0) {
                t = GmoTrackWrapFrame(frame, GmoTrackHalf(*(const u16 *)keys),
                                      GmoTrackHalf(*(const u16 *)last), flags);
            }
            if (key < last) {
                key = key + stride;
            }
            step = stride;
            t1 = GmoTrackHalf(*(const u16 *)key);
            t0 = t1;
            if (t1 <= t) {
                for (;;) {
                    if (!(key < last)) {
                        step = 0;
                        break;
                    }
                    t1 = GmoTrackHalf(*(const u16 *)(key + stride));
                    if (t < t1) {
                        break;
                    }
                    key = key + stride;
                    t0 = t1;
                }
            } else {
                for (;;) {
                    if (!(keys < key)) {
                        step = 0;
                        break;
                    }
                    key = key - stride;
                    t0 = GmoTrackHalf(*(const u16 *)key);
                    if (t0 <= t) {
                        break;
                    }
                    t1 = t0;
                }
            }
            *cursor = (u16)(key - keys);
            if (t == t0) {
                step = 0;
            }
            key1 = key + step;
        }

        if (trk->paramC == 0) {
            return 0;
        }
        i = 0;
        do {
            if (key == key1) {
                t = 0.0f;
            } else {
                t = (frame - t0) / (t1 - t0);
            }
            mode = trk->param8 & 0xf;
            if (mode == 0) {
                const u16 *h0 = (const u16 *)key;

                for (j = 0; j < 4; j++) {
                    out[j] = VfH2f(h0[1 + j]);
                }
            } else if (mode == 1 || mode == 4) {
                const u16 *h0 = (const u16 *)key;
                const u16 *h1 = (const u16 *)key1;

                for (j = 0; j < 4; j++) {
                    va[j] = VfH2f(h0[1 + j]);
                    vb[j] = VfH2f(h1[1 + j]);
                }
                if (mode == 1) {
                    GmoTrackLerp4(out, va, vb, t);
                } else {
                    GmoTrackSlerp4(out, va, vb, t);
                }
            } else if (mode == 2) {
                /* 3 halves per component: value, in tangent, out tangent */
                const u16 *h0 = (const u16 *)key;
                const u16 *h1 = (const u16 *)key1;

                for (j = 0; j < 4; j++) {
                    va[j] = VfH2f(h0[1 + 3 * j]);
                    vb[j] = VfH2f(h1[1 + 3 * j]);
                    vc[j] = VfH2f(h0[3 + 3 * j]);
                    vd[j] = VfH2f(h1[2 + 3 * j]);
                }
                GmoTrackHermite4(out, va, vb, vc, vd, t);
            } else {
                /* Bezier: 5 halves per component (time, value, in/out handles) */
                const u16 *k0 = (const u16 *)key;
                const u16 *k1 = (const u16 *)key1;

                for (j = 0; j < 4; j++) {
                    float s = GmoBezierSolveParamB(t0, t0 + GmoTrackHalf(k0[4]),
                                                   t1 + GmoTrackHalf(k1[2]), t1, frame);
                    float u = 1.0f - s;
                    float s2 = s * s;
                    float u2 = u * u;
                    float b1 = s * u2 * 3.0f;
                    float b2 = u * s2 * 3.0f;
                    float b0 = b1 + u * u2;
                    float b3 = b2 + s * s2;

                    tmp[j] = b0 * GmoTrackHalf(k0[1]) + b1 * GmoTrackHalf(k0[5]) +
                             b2 * GmoTrackHalf(k1[3]) + b3 * GmoTrackHalf(k1[1]);
                    k0 += 5;
                    k1 += 5;
                }
                for (j = 0; j < 4; j++) {
                    out[j] = tmp[j];
                }
            }
            n = trk->paramC;
            i += 4;
            key += group * 8;
            key1 += group * 8;
            out += 4;
        } while (i < (s32)n);
        return n;
    }

    /* float keys: time in the leading float */
    if (count < 2) {
        t0 = 0.0f;
        key = (const u8 *)PspPtr(trk->data);
        t1 = t0;
        key1 = key;
    } else {
        values = trk->paramC;
        if (mode == 2) {
            values = values * 3;
        } else if (mode == 3) {
            values = values * 5;
        }
        keys = (const u8 *)PspPtr(trk->data);
        stride = (s32)(values * 4 + 4);
        last = keys + stride * (s32)(count - 1);
        key = keys + *cursor;
        t = frame;
        if ((flags & 0xff00) != 0) {
            t = GmoTrackWrapFrame(frame, *(const float *)keys, *(const float *)last, flags);
        }
        if (key < last) {
            key = key + stride;
        }
        step = stride;
        t1 = *(const float *)key;
        t0 = t1;
        if (t1 <= t) {
            for (;;) {
                if (!(key < last)) {
                    step = 0;
                    break;
                }
                t1 = *(const float *)(key + stride);
                if (t < t1) {
                    break;
                }
                key = key + stride;
                t0 = t1;
            }
        } else {
            for (;;) {
                if (!(keys < key)) {
                    step = 0;
                    break;
                }
                key = key - stride;
                t0 = *(const float *)key;
                if (t0 <= t) {
                    break;
                }
                t1 = t0;
            }
        }
        *cursor = (u16)(key - keys);
        if (t == t0) {
            step = 0;
        }
        key1 = key + step;
    }

    if (trk->paramC == 0) {
        return 0;
    }
    i = 0;
    do {
        if (key == key1) {
            t = 0.0f;
        } else {
            t = (frame - t0) / (t1 - t0);
        }
        mode = trk->param8 & 0xf;
        if (mode == 0) {
            const float *f0 = (const float *)key;

            for (j = 0; j < 4; j++) {
                out[j] = f0[1 + j];
            }
        } else if (mode == 1 || mode == 4) {
            const float *f0 = (const float *)key;
            const float *f1 = (const float *)key1;

            for (j = 0; j < 4; j++) {
                va[j] = f0[1 + j];
                vb[j] = f1[1 + j];
            }
            if (mode == 1) {
                GmoTrackLerp4(out, va, vb, t);
            } else {
                GmoTrackSlerp4(out, va, vb, t);
            }
        } else if (mode == 2) {
            /* 3 floats per component: value, in tangent, out tangent */
            const float *f0 = (const float *)key;
            const float *f1 = (const float *)key1;

            for (j = 0; j < 4; j++) {
                va[j] = f0[1 + 3 * j];
                vb[j] = f1[1 + 3 * j];
                vc[j] = f0[3 + 3 * j];
                vd[j] = f1[2 + 3 * j];
            }
            GmoTrackHermite4(out, va, vb, vc, vd, t);
        } else {
            /* Bezier: 5 floats per component (time, value, in/out handles) */
            const float *k0 = (const float *)key;
            const float *k1 = (const float *)key1;

            for (j = 0; j < 4; j++) {
                float s = GmoBezierSolveParamB(t0, t0 + k0[4], k1[2] + t1, t1, frame);
                float u = 1.0f - s;
                float s2 = s * s;
                float u2 = u * u;
                float b1 = s * u2 * 3.0f;
                float b2 = u * s2 * 3.0f;
                float b0 = b1 + u * u2;
                float b3 = b2 + s * s2;

                tmp[j] = b0 * k0[1] + b1 * k0[5] + b2 * k1[3] + b3 * k1[1];
                k0 += 5;
                k1 += 5;
            }
            for (j = 0; j < 4; j++) {
                out[j] = tmp[j];
            }
        }
        n = trk->paramC;
        i += 4;
        key += group * 16;
        key1 += group * 16;
        out += 4;
    } while (i < (s32)n);
    return n;
}
