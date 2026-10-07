// bdc 0x088954a0 BtlAiSeekItemCheck
#include "bdc.h"

/* Check method of behaviour layer 2 of `BtlAi` (seek an item, `MemberFnPtr` pair at
   `0x08a80300`, run `BtlAiSeekItemRun`); returns 0 for owner kinds 0x15..0x20. While the layer
   (`seekItem`) is not active (vtable slot 2 returns 0), every 1 s it rolls `seekItemChance` percent (`CoreRandNext`) and scans
   `g_btlItemList` (count `BtlItemListCount`, scratch flags from the low heap) for items within
   1000 units and 90° of view (`BtlAiDistanceInView`) and reachable (`BtlAiRaycastToPoint`);
   items of type 3 (`BtlItem` `type`) count only while the owner's HP is at most 70 % (`BtlCombatGetHp` /
   `BtlCombatGetMaxHp`). With at least one candidate it stores list entry `candidates - 1` in
   `seekItem.item` and sets `active`. Returns vtable slot 2 again (whether it wants control). */

s32 BtlAiSeekItemCheck(BtlAi *self)
{
    BtlAiSeekItemLayer *layer = &self->seekItem;
    const VtblEntry *isActive;
    s32 kind;

    kind = -1;
    if (self->owner != NULL) {
        kind = (s32)self->owner->base.base.unk08;
    }
    if (kind >= 0x15 && kind <= 0x20) {
        return 0;
    }

    isActive = &layer->base.vtbl[2];
    if (((s32 (*)(void *))isActive->fn)((u8 *)layer + isActive->delta) == 0) {
        if (!layer->base.timerExpired) {
            layer->base.timerElapsed = layer->base.timerElapsed + 0.0333333351f;
            if (!(layer->base.timerElapsed < layer->base.timerLimit)) {
                layer->base.timerElapsed = layer->base.timerLimit;
                layer->base.timerExpired = 1;
            }
        }
        if (layer->base.timerExpired) {
            s8 chance = (s8)self->seekItemChance;

            if ((s32)CoreRandNext(99) < chance) {
                u32 flagCount = 0;
                s32 *flags = NULL;
                s32 candidates = 0;
                u32 index = 0;
                bool eligible = true;
                CoreObject *node;
                u32 count;

                count = (u32)BtlItemListCount();
                if (count != 0) {
                    bool fromLow;
                    u32 i;

                    flagCount = count;
                    MemLock();
                    fromLow = MemIsAllocFromLow();
                    MemSetAllocFromLow(true);
                    flags = MemAlloc(count << 2, NULL, 0);
                    MemSetAllocFromLow(fromLow);
                    MemUnlock();
                    for (i = 0; i < flagCount; i++) {
                        flags[i] = 0;
                    }
                }

                for (node = g_btlItemList; node != NULL; node = node->next) {
                    BtlItem *item = (BtlItem *)node;

                    if (item->type == 3) {
                        BtlCombatState *combat = &self->owner->combat;
                        float hp = BtlCombatGetHp(combat);
                        /* max HP converted as unsigned (cvt.s.w + 2^32 when negative) */
                        float maxHp = (float)(u32)BtlCombatGetMaxHp(combat);

                        eligible = (hp / maxHp) * 100.0f <= 70.0f;
                    }
                    if (eligible) {
                        float target[4];

                        /* 16-byte copy of item->pos (lv.q/sv.q through C000) */
                        target[0] = item->pos[0];
                        target[1] = item->pos[1];
                        target[2] = item->pos[2];
                        target[3] = item->pos[3];
                        if (BtlAiDistanceInView(1000.0f, 90.0f, self, target) != 0.0f &&
                            BtlAiRaycastToPoint(self, item->pos) == 0) {
                            if (index < flagCount && flags[index] != 1) {
                                flags[index] = 1;
                            }
                            candidates++;
                        }
                    }
                    index = (u8)(index + 1);
                }

                if (candidates != 0) {
                    s32 pick = 0;

                    node = g_btlItemList;
                    while (pick < candidates - 1) {
                        pick = (u8)(pick + 1);
                        node = node->next;
                    }
                    if (node != NULL) {
                        layer->base.active = 1;
                        layer->item = node;
                    }
                }
                if (flags != NULL) {
                    MemLock();
                    MemFree(flags, NULL, 0);
                    MemUnlock();
                }
            }
            layer->base.timerElapsed = 0.0f;
            layer->base.timerLimit = 1.0f;
            layer->base.timerExpired = 0;
        }
    }
    /* vtbl pointer reloaded after the calls above */
    isActive = &layer->base.vtbl[2];
    return ((s32 (*)(void *))isActive->fn)((u8 *)layer + isActive->delta);
}
