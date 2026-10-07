// bdc 0x08a17f18 GmoMotionApplyTrack
#include "bdc.h"

/* Evaluates a motion track at `frame` (`GmoMotionEvalTrack` into a 16-float buffer) and, when
   it yields values, applies them with `weight` to the track's target in `model` (a `GmoModel`),
   with attribute type `paramD` and the value count: with `kind` bit 0x100 node `ref` (bounded by
   `nodeCount`) through `GmoNodeApplyAttr`, else with bit 0x200 material `ref` (bounded by
   `materialCount`) through `GmoMaterialApplyAttr`. Returns early when `track`, `cursor` or
   `model` is NULL, the evaluation gives 0 values or `ref` is out of range. A `ref` of 0xffff
   skips the range check and passes the value 0xffff itself as the target pointer (see Notes). */

void GmoMotionApplyTrack(float frame, float weight, void *track, u16 *cursor, void *model)
{
    const GmoMotionTrack *t = (const GmoMotionTrack *)track;
    GmoModel *m = (GmoModel *)model;
    float out[16] __attribute__((aligned(16)));
    u32 count;
    u32 idx;
    GmoNode *node;
    GmoMaterial *mat;

    if (t == NULL || cursor == NULL || m == NULL) {
        return;
    }
    count = GmoMotionEvalTrack(frame, track, cursor, out);
    if (count == 0) {
        return;
    }
    if ((t->kind & 0x100) != 0) {
        idx = t->ref;
        if (((idx + 1) & 0xffff0000) != 0) {
            node = (GmoNode *)(uintptr_t)idx;
        } else {
            if (idx >= m->nodeCount) {
                return;
            }
            node = &m->nodes[idx];
        }
        if (node != NULL) {
            GmoNodeApplyAttr(weight, node, t->paramD, 0, out, count);
        }
    } else if ((t->kind & 0x200) != 0) {
        idx = t->ref;
        if (((idx + 1) & 0xffff0000) != 0) {
            mat = (GmoMaterial *)(uintptr_t)idx;
        } else {
            if (idx >= m->materialCount) {
                return;
            }
            mat = &((GmoMaterial *)m->materials)[idx];
        }
        if (mat != NULL) {
            GmoMaterialApplyAttr(weight, mat, t->paramD, 0, out, count);
        }
    }
}
