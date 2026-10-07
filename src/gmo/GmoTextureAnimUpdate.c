// bdc 0x08a10dec GmoTextureAnimUpdate
#include "bdc.h"

/* Advances and applies the active animation track of a `GmoTexture` (`lastTrack`, or `trackCount`
   when it is negative or past it). Does nothing for a NULL texture or one without flag 0x10.
   With `flags` bit 0 the track time moves by `frameRate * dt`; on crossing `endFrame` going forward
   (or `startFrame` going back) it clamps to that end when `mode` is 0 and wraps to the other end
   otherwise. With bit 1 the time is evaluated: a track without keys or channels just sets the frame
   pairs to `floor(time)`/`floor(time) + 1` and `blend` to the fraction; otherwise every
   `GmoTexChannel` is sampled at `time` and passed to `GmoTextureAnimApply`, except kinds 3/11
   with channel flag 0x80, which store the frame pair and `blend` into the track directly. */

void GmoTextureAnimUpdate(float dt, void *texp, u32 flags)
{
    GmoTexture *tex = (GmoTexture *)texp;
    GmoTexTrack *track;
    s32 idx;

    if (tex == NULL) {
        return;
    }
    if ((tex->flags & 0x10) == 0) {
        return;
    }
    idx = (s8)tex->lastTrack;
    if (idx < 0 || (s32)tex->trackCount < idx) {
        idx = tex->trackCount;
    }
    track = (GmoTexTrack *)tex->tracks + idx;
    if (track == NULL) {
        return;
    }

    if ((flags & 1) != 0) {
        float time = track->time;
        float start = track->startFrame;
        float end = track->endFrame;
        float t = time + track->frameRate * dt;
        s32 clamp = (track->mode == 0);

        if (end <= t && time <= end && !(time == t)) {
            t = clamp ? end : start;
        } else if (t <= start && start <= time && !(time == t)) {
            t = clamp ? start : end;
        }
        track->time = t;
    }

    if ((flags & 2) != 0) {
        float time = track->time;
        s32 i;

        /* lhu of keyCount and frameCount together */
        if (track->keyCount == 0 && track->frameCount == 0) {
            s32 n = (s32)__builtin_floorf(time); /* floor.w.s */
            s16 a = (s16)n;
            s16 b = (s16)(a + 1);

            track->frameA1 = a;
            track->blend = time - (float)n;
            track->frameB1 = b;
            track->frameA0 = a;
            track->frameB0 = b;
            return;
        }

        for (i = 0; i < (s32)track->frameCount; i++) {
            GmoTexChannel *ch = &track->frames[i];
            s16 *seq = ch->seq;
            s32 ipos;
            float pos;
            s32 n;
            s32 from;
            s32 to;
            float frac;
            u32 kind;
            u32 interp;

            if (time < 0.0f) {
                ipos = 0;
                pos = 0.0f;
            } else {
                ipos = (s32)time;
                pos = time;
            }

            /* walk the segments to the one holding ipos */
            n = seq[0];
            for (;;) {
                s16 *vals = seq + 1;

                if (n == 0) {
                    from = vals[0];
                    frac = 0.0f;
                    to = from;
                    break;
                }
                if (n > 0) {
                    if (ipos < n) {
                        from = vals[0];
                        to = seq[3];
                        frac = pos / (float)n;
                        break;
                    }
                    seq += 2;
                } else {
                    n = -n;
                    if (ipos < n) {
                        s32 next = ipos + 1;

                        if (next == n) {
                            next = ipos + 2; /* skip the next segment's count */
                        }
                        frac = pos - (float)ipos;
                        from = vals[ipos];
                        to = vals[next];
                        break;
                    }
                    seq = vals + n;
                }
                ipos -= n;
                pos -= (float)n;
                n = seq[0];
            }

            kind = ch->kind;
            interp = ch->flags & 0xf;
            if ((ch->flags & 0x80) != 0 && (kind == 3 || kind == 11)) {
                if (interp == 1 && from != to) {
                    float v = frac * (float)(to - from) + (float)from;
                    s32 f = (s32)__builtin_floorf(v); /* floor.w.s */

                    if (from < to) {
                        frac = v - (float)f;
                        from = f;
                        to = f + 1;
                    } else {
                        from = f + 1;
                        frac = (float)from - v;
                        to = f;
                    }
                }
                track->blend = frac;
                if (kind == 3) {
                    track->frameA0 = (s16)from;
                    track->frameB0 = (s16)to;
                } else {
                    track->frameA1 = (s16)from;
                    track->frameB1 = (s16)to;
                }
            } else {
                float value = (float)from;

                if (interp == 1) {
                    value = value + frac * (float)(to - from);
                }
                GmoTextureAnimApply(value, tex, kind);
            }
        }
    }
}
