// bdc 0x08858ee8 ActorCrystalModeAttack
#include "bdc.h"

/* Crystal mode 1 (vtable slot 33, attack): unless a cut-in runs (`BtlIsCutInRunning`) sets flag
   0x400000 and calls the fire-type handler `+0x90c` from the table `0x08a67104` (0
   `ActorCrystalMode3ReturnFalse`, 1 `ActorCrystalMode4Update`, 2 `ActorCrystalMode5Update`,
   3 `ActorCrystalFireType3`, 4 `ActorCrystalFireType4`; plain member pointers, no this-adjust
   or virtual entries); returns to mode 0 when it reports done. */

void ActorCrystalModeAttack(ActorCrystal *self)
{
    static int (*const handlers[5])(ActorCrystal *) = {
        ActorCrystalMode3ReturnFalse, ActorCrystalMode4Update, ActorCrystalMode5Update,
        ActorCrystalFireType3,        ActorCrystalFireType4,
    };

    if (!BtlIsCutInRunning()) {
        int fireType = self->fireType;

        self->base.stateFlags |= 0x400000;
        if (handlers[fireType](self) != 0) {
            ActorCrystalSetMode(self, 0, false);
        }
    }
}
