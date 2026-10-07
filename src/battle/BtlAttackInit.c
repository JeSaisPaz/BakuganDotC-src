// bdc 0x08876ba8 BtlAttackInit
#include "bdc.h"

/* Per-type initialisation of a new attack object (called by `BtlAttackCtor`): stores the result
   of the owner's virtual entry 20 in `ownerValue`; sets `targetId` to the owner's target id, or,
   when the type's linked-target nibble in `g_btlAttackTypeInfo` is non-zero, to the id of the
   owner's `attackObj` if `BtlBakuganListFind` still finds it (else the owner's target id);
   clears phase, parameters, age, end frame, the end/reflect flags, both effect pointers,
   `pendingHit` and `reflected`, sets `turnRate` to 0.02, zeroes `summonVec` (the VFPU bank's
   zero column C720) and fills `params` for `type` (`BtlAttackParamsCopy`). */
void BtlAttackInit(BtlAttack *self, s32 type)
{
    BtlBakugan *owner = self->owner;
    const VtblEntry *entry = &((const VtblEntry *)owner->base.base.vtable)[20];
    void *linked;

    self->ownerValue = ((u32 (*)(void *))entry->fn)((u8 *)owner + entry->delta);
    if (((g_btlAttackTypeInfo[type] >> 20) & 0xf) == 0) {
        self->targetId = self->owner->targetId;
    } else {
        linked = BtlBakuganListFind(self->owner->attackObj);
        if (linked == NULL) {
            self->targetId = self->owner->targetId;
        } else {
            self->targetId = ((CoreObject *)linked)->id;
        }
    }
    self->phase = 0;
    self->param0 = 0;
    self->param1 = 0;
    self->paramF0 = 0.0f;
    self->paramF1 = 0.0f;
    self->paramF2 = 0.0f;
    self->age = 0;
    self->turnRate = 0.0199999996f;
    self->endFrame = 0;
    self->cancelled = 0;
    self->silentEnd = 0;
    self->reflect = 0;
    self->noSpark = 0;
    self->effect = NULL;
    self->effect2 = NULL;
    self->pendingHit = 0;
    self->reflected = 0;
    self->summonVec[0] = 0.0f;
    self->summonVec[1] = 0.0f;
    self->summonVec[2] = 0.0f;
    self->summonVec[3] = 0.0f;
    BtlAttackParamsCopy(&self->params, type);
}
