// bdc 0x089dcaa8 GmoDlWriteMorphWeights
#include "bdc.h"

/* For a morphing node (flag 0x40000) writes a GE `BASE 0` and the morph weights `0x2c..`: the
   node's `morphCount` entries of `morphWeights`, or, with no table, all 8 weights as a triangle
   ramp around `morphPos` (`max(0, 1 - |morphPos - i|)`). Weights are emitted as float24 (the
   float's bits >> 8). Does nothing for other nodes. */

static inline u32 GmoMorphFloatBits(float f)
{
    union {
        float f;
        u32 u;
    } v;

    v.f = f;
    return v.u;
}

void GmoDlWriteMorphWeights(GmoDlContext *self)
{
    int count;
    int i;
    float pos;
    float w;

    if ((self->node->flags & 0x40000) == 0) {
        return;
    }
    *self->cur++ = 0xff000000;
    count = self->node->morphCount;
    if (count > 0) {
        for (i = 0; i < count; i++) {
            w = self->node->morphWeights[i];
            *self->cur++ = (u32)(i + 0x2c) << 24 | GmoMorphFloatBits(w) >> 8;
        }
        return;
    }
    pos = self->node->morphPos;
    for (i = 0; i < 8; i++) {
        w = 1.0f - fabsf(pos - (float)i);
        if (w < 0.0f) {
            w = 0.0f;
        }
        *self->cur++ = (u32)(i + 0x2c) << 24 | GmoMorphFloatBits(w) >> 8;
    }
}
