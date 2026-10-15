// bdc 0x08862a94 BtlBakuganStopKindSound
#include "bdc.h"

/* Stops sound slot `slot` of the unit's kind on its sound object `+0x12c` (`SndObjectStopSound`);
   the id is `g_btlBakuganKindSoundIds``[kind - 1][slot]`, the same table as
   `BtlBakuganPlayKindSound`. A kind outside the table (the field crystals' kinds 33/34, which the
   original reads past it) has nothing to stop. */
void BtlBakuganStopKindSound(BtlBakugan *self, int slot)
{
    u32 row = self->base.base.unk08 - 1;

    if (row >= sizeof(g_btlBakuganKindSoundIds) / sizeof(g_btlBakuganKindSoundIds[0])) {
        return;
    }
    SndObjectStopSound(self->base.sound, g_btlBakuganKindSoundIds[row][slot]);
}
