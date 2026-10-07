// bdc 0x0886348c BtlBakuganDropItem
#include "bdc.h"

/* Drops a battle item where the unit is (used when a crystal breaks, `ActorCrystalBreak`, and in
   the knock-out state `BtlBakuganState06Update`). Does nothing while bit 3 of
   `g_scriptGlobalBits` is set. Otherwise copies the unit's position (quad copy), raises y by 100 and drops it onto the ground (`CollisionRaycastPoint`, in place), picks
   the item-odds row (`BtlStageGetItemOdds`): 2 when virtual slot 12 returns non-zero, else 3 when
   slot 11 does, else 0 in battle rule mode 2 (script global variable 8) and 1 otherwise. Rolls the
   kind with `BtlItemRollKind``(pctB, pctA, pctC, 0)` and, unless it is 6 (none), spawns the item
   there with a 600-frame lifetime, appear animation 1 and no spawner (`BtlItemCreate`). */

void BtlBakuganDropItem(BtlBakugan *self)
{
    float pos[4] __attribute__((aligned(16)));
    const VtblEntry *slot12;
    const VtblEntry *slot11;
    BtlItemOdds *odds;
    int row;
    int kind;

    if (CoreBitsetTest(3, g_scriptGlobalBits)) {
        return;
    }
    pos[0] = self->base.pos[0];
    pos[1] = self->base.pos[1];
    pos[2] = self->base.pos[2];
    pos[3] = self->base.pos[3];
    pos[1] += 100.0f;
    CollisionRaycastPoint(pos, pos);
    slot12 = &((const VtblEntry *)self->base.base.vtable)[12];
    if (((int (*)(void *))slot12->fn)((u8 *)self + slot12->delta) != 0) {
        row = 2;
    } else {
        row = 3;
        slot11 = &((const VtblEntry *)self->base.base.vtable)[11];
        if (((int (*)(void *))slot11->fn)((u8 *)self + slot11->delta) == 0) {
            row = g_scriptGlobalVars[8] != 2;
        }
    }
    odds = (BtlItemOdds *)BtlStageGetItemOdds(row);
    kind = BtlItemRollKind(odds->pctB, odds->pctA, odds->pctC, 0);
    if (kind != 6) {
        BtlItemCreate(kind, (u32 *)pos, 600, 1, NULL);
    }
}
