// bdc 0x0886b25c BtlBakuganStartHitWindow
#include "bdc.h"

/* Arms the unit's pending hit window from the `BtlHitWindowDef` `def` (no-op when NULL): sets
   `hitWindowActive`, clears the unused record fields, copies the flags byte, attack id and size,
   and stores 0.8 x the reach. Called by states 7 and 11. */

void BtlBakuganStartHitWindow(BtlBakugan *self, BtlHitWindowDef *def)
{
    if (def == NULL) {
        return;
    }
    self->hitWindowActive = 1;
    self->hitWindowByte1 = 0;
    self->hitWindowByte2 = 0;
    self->hitWindowFlags = (u8)def->flags;
    self->hitWindowHalf6 = 0;
    self->hitWindowAttackId = def->attackId;
    self->hitWindowSize = def->size;
    self->hitWindowParam10 = 0.0f;
    self->hitWindowReach = def->reach * 0.8f;
}
