// bdc 0x08862a94 BtlBakuganStopKindSound
#include "bdc.h"

/* Stops sound slot `slot` of the unit's kind on its sound object `+0x12c` (`SndObjectStopSound`);
   the id is `g_btlBakuganKindSoundIds``[kind - 1][slot]`, the same table as
   `BtlBakuganPlayKindSound`. */
void BtlBakuganStopKindSound(BtlBakugan *self, int slot)
{
    SndObjectStopSound(self->base.sound, g_btlBakuganKindSoundIds[self->base.base.unk08 - 1][slot]);
}
