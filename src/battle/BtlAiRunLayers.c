// bdc 0x0888df44 BtlAiRunLayers
#include "bdc.h"

/* Behaviour-layer arbitration of `BtlAi` (from `BtlAiUpdate`): calls the `check`
   `MemberFnPtr` of each layer in `layerOrder[0..4]` (empty slots and all-zero pointers count as
   0); the first that returns non-zero is selected. If its index differs from `activeLayer`, clears
   pad bits 0x10/0x10000 of `pad.cur.aiActions`, resets the owner's combo
   (`BtlBakuganResetCombo`), stores the index in `activeLayer` and resets the commands
   (`BtlAiResetCommands`). Then, selected or not, clears move flag 0x4000 and calls the `run`
   member pointer of the `activeLayer` layer. Returns true when the active layer changed. */
bool BtlAiRunLayers(BtlAi *self)
{
    bool changed = false;
    BtlAiLayer *layer;
    const MemberFnPtr *mfp;
    u8 *obj;
    void *fn;
    s32 i;

    for (i = 0; i < 5; i++) {
        s32 taken = 0;

        layer = self->layerOrder[i];
        if (layer != NULL) {
            mfp = &layer->check;
            if (mfp->index != 0 || mfp->delta != 0 || mfp->pfn != NULL) {
                obj = (u8 *)self + mfp->delta;
                fn = mfp->pfn;
                /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
                if (mfp->index != 0) {
                    const VtblEntry *entry = &(*(const VtblEntry **)(obj + (intptr_t)mfp->pfn))[mfp->index];

                    obj += entry->delta;
                    fn = entry->fn;
                }
                taken = ((s32 (*)(void *))fn)(obj);
            }
        }
        if (taken != 0) {
            changed = self->activeLayer != i;
            if (changed) {
                self->pad.cur.aiActions &= 0xfffeffef;
                BtlBakuganResetCombo(self->owner);
                self->activeLayer = i;
                BtlAiResetCommands(self);
            }
            break;
        }
    }

    self->moveFlags &= ~0x4000u;
    layer = self->layerOrder[self->activeLayer];
    if (layer != NULL) {
        mfp = &layer->run;
        if (mfp->index != 0 || mfp->delta != 0 || mfp->pfn != NULL) {
            obj = (u8 *)self + mfp->delta;
            fn = mfp->pfn;
            if (mfp->index != 0) {
                const VtblEntry *entry = &(*(const VtblEntry **)(obj + (intptr_t)mfp->pfn))[mfp->index];

                obj += entry->delta;
                fn = entry->fn;
            }
            ((void (*)(void *))fn)(obj);
        }
    }
    return changed;
}
