// bdc 0x088812e0 BtlAttackType5BUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x5b (handler table `0x08a685f0`, run by `BtlAttackUpdate`),
   a second owner-attached barrier with the same code as `BtlAttackType0CUpdate`. When the owner is no longer in the battle Bakugan
   list (`CoreObjectListContains`) it jumps to phase 2 with age 20. Every frame its matrix is
   the owner's model root matrix times its node's local matrix (column-major
   rootMatrix * localMatrix, VFPU vmmul.q) and its position is the matrix's
   row 3. Phase 0 sets the owner's guard flag 0x10 and resets the age; phase 1 runs
   `BtlAttackCheckClash` and stays while the flag is set and the owner's status 7 lasts
   (`BtlCombatGetStatusRatio` > 0); phase 2 sets its attached effects to state 2
   (`GfxEffectSetStateAttached`), clears the flag and ends (`BtlAttackEnd`). */
void BtlAttackType5BUpdate(BtlAttack *self)
{
    const float *a;
    const float *b;
    int i;
    int j;

    if (CoreObjectListContains(BtlGetBakuganList(), (CoreObject *)self->owner) == 0) {
        self->age = 20;
        self->phase = 2;
    }
    a = self->owner->base.data->rootMatrix;
    b = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = b[j * 4 + 0] * a[i] + b[j * 4 + 1] * a[4 + i] +
                              b[j * 4 + 2] * a[8 + i] + b[j * 4 + 3] * a[12 + i];
        }
    }
    self->pos[0] = self->mtx[3][0];
    self->pos[1] = self->mtx[3][1];
    self->pos[2] = self->mtx[3][2];
    self->pos[3] = self->mtx[3][3];
    switch (self->phase) {
    case 0:
        self->owner->flags |= 0x10;
        self->age = 0;
        self->phase++;
        break;
    case 1:
        BtlAttackCheckClash(self);
        if ((self->owner->flags & 0x10) != 0 &&
            !(BtlCombatGetStatusRatio(&self->owner->combat, 7) <= 0.0f)) {
            return;
        }
        self->phase++;
        break;
    case 2:
        GfxEffectSetStateAttached(g_btlAttackEffectMgr, -1, self->pos, 2);
        self->owner->flags &= ~0x10u;
        BtlAttackEnd(self);
        break;
    default:
        break;
    }
}
