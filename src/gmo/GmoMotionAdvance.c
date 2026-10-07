// bdc 0x089e1400 GmoMotionAdvance
#include "bdc.h"

/* Advances one motion slot and applies it to a model. With `flags & 1` steps the slot's `frame`
   by `frameRate * dt`: crossing `endFrame` forward wraps to `startFrame` (and zeroes the per-track
   key cursors) when `loop` is set, else clamps to `endFrame`; crossing `startFrame` backward wraps
   to `endFrame` / clamps to `startFrame`. With `flags & 2` (and a non-null slot) evaluates every
   track at the frame (rounded to a whole frame unless `model->flags28 & 4`): tracks without kind
   bit 0 go to `GmoMotionApplyTrack`; the others scan from their cursor for the surrounding keys
   (float keys: `GmoMotionEvalKeyF32`; half-float keys, `param8 & 0x80`: `GmoHalfToFloatBits`,
   `GmoMotionEvalKeyF16`) and blend the value into the node field given by
   `g_gmoChannelDescs` with `GmoMotionBlendValue`, updating the node's `flags42`. Called by
   `GmoMotionUpdate`/`GmoMotionAdvanceBlended`. */

static float GmoMotionKeyTimeH(const u8 *key)
{
    return GmoHalfToFloatBits(*(const u16 *)key);
}

void GmoMotionAdvance(float dt, float weight, GmoModel *model, GmoMotionSlot *slot, u32 flags)
{
    GmoMotionTrack *track;
    u16 *cursor;
    const GmoChannelDesc *desc;
    GmoNode *node;
    const u8 *keys;
    const u8 *last;
    const u8 *key;
    const u8 *key1;
    float frame;
    float next;
    float start;
    float end;
    float t0;
    float t1;
    bool blend;
    s32 i;
    s32 count;
    s32 stride;
    s32 step;
    s32 idx;
    u32 mode;
    u32 type;
    ScePspFVector4 value; /* key evaluation result (VFPU C000 hand-off) */

    if (flags & 1) {
        frame = slot->frame;
        next = frame + slot->frameRate * dt;
        start = slot->startFrame;
        end = slot->endFrame;
        if (end <= next && !(end < frame) && !(frame == next)) {
            /* crossed the end going forward */
            if (slot->loop != 0) {
                next = start;
                memset(slot->cursors, 0, (u32)slot->trackCount * 2);
            } else {
                next = end;
            }
        } else if (next <= start && !(frame < start) && !(frame == next)) {
            /* crossed the start going backward */
            next = end;
            if (slot->loop == 0) {
                next = start;
            }
        }
        slot->frame = next;
    }

    if ((flags & 2) == 0 || slot == NULL) {
        return;
    }

    frame = slot->frame;
    if ((model->flags28 & 4) == 0) {
        frame = (float)(s32)__builtin_floorf(frame + 0.5f); /* floor.w.s */
    }
    blend = weight < 1.0f;

    track = slot->tracks;
    cursor = slot->cursors;
    for (i = (s32)slot->trackCount - 1; i >= 0; i--, track++, cursor++) {
        if ((track->kind & 0x100) != 0 && !((s32)track->ref < (s32)model->nodeCount)) {
            continue;
        }
        if ((track->kind & 1) == 0) {
            GmoMotionApplyTrack(frame, weight, track, cursor, model);
            continue;
        }

        count = track->paramA;
        keys = (const u8 *)track->data;
        if ((track->param8 & 0x80) == 0) {
            /* full-precision keys, time in the leading float */
            if (count < 2) {
                t0 = 0.0f;
                t1 = 0.0f;
                key = keys;
                key1 = keys;
            } else {
                stride = track->paramC;
                mode = track->param8 & 0xf;
                if (mode == 2) {
                    stride = stride * 3;
                }
                if (mode == 3) {
                    stride = stride * 5;
                }
                stride = (stride + 1) * 4;
                if ((track->param8 & 0x80) != 0) {
                    stride = stride / 2;
                }
                step = stride;
                last = keys + (count - 1) * stride;
                key = keys + *cursor;
                if (key < last) {
                    key = key + stride;
                }
                t1 = *(const float *)key;
                t0 = t1;
                if (t0 <= frame) {
                    for (;;) {
                        if (!(key < last)) {
                            step = 0;
                            break;
                        }
                        t1 = *(const float *)(key + stride);
                        if (!(t1 <= frame)) {
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
                        if (t0 <= frame) {
                            break;
                        }
                        t1 = t0;
                    }
                }
                *cursor = (u16)(key - keys);
                if (t0 == frame) {
                    step = 0;
                }
                key1 = key + step;
            }
            value = GmoMotionEvalKeyF32(frame, t0, t1, track, key, key1, false);
        } else {
            /* half-precision keys, time in the leading half */
            if (count < 2) {
                t0 = 0.0f;
                t1 = 0.0f;
                key = keys;
                key1 = keys;
            } else {
                stride = track->paramC;
                mode = track->param8 & 0xf;
                if (mode == 2) {
                    stride = stride * 3;
                }
                if (mode == 3) {
                    stride = stride * 5;
                }
                stride = (stride + 1) * 4;
                if ((track->param8 & 0x80) != 0) {
                    stride = stride / 2;
                }
                step = stride;
                last = keys + (count - 1) * stride;
                key = keys + *cursor;
                if (key < last) {
                    key = key + stride;
                }
                t1 = GmoMotionKeyTimeH(key);
                t0 = t1;
                if (t0 <= frame) {
                    for (;;) {
                        if (!(key < last)) {
                            step = 0;
                            break;
                        }
                        t1 = GmoMotionKeyTimeH(key + stride);
                        if (!(t1 <= frame)) {
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
                        t0 = GmoMotionKeyTimeH(key);
                        if (t0 <= frame) {
                            break;
                        }
                        t1 = t0;
                    }
                }
                *cursor = (u16)(key - keys);
                if (t0 == frame) {
                    step = 0;
                }
                key1 = key + step;
            }
            value = GmoMotionEvalKeyF16(frame, t0, t1, track, (const u16 *)key, (const u16 *)key1, false);
        }

        node = &model->nodes[track->ref];
        type = track->paramD;
        idx = (s32)type - 0x48;
        /* types below 0x48 index before the table, as in the original */
        desc = &g_gmoChannelDescs[(idx < 7) ? idx : 6];
        if ((node->tag & 0x4000) != 0 && type == 0x4b) {
            /* sticks for the remaining tracks of this call */
            if (slot == (GmoMotionSlot *)model->motions + model->motionIndex) {
                weight = 1.0f;
            } else {
                weight = 0.0f;
            }
        }
        GmoMotionBlendValue(weight, (float *)((u8 *)node + desc->offset), blend, (s32)type, value);
        node->flags42 = (u16)((node->flags42 & ~(u32)desc->clearMask) | desc->setMask);
    }
}
