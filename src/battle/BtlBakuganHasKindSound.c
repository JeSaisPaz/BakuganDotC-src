// bdc 0x08862adc BtlBakuganHasKindSound
#include "bdc.h"

/* Returns whether sound slot `slot` of the unit's kind (`g_btlBakuganKindSoundIds`, see
   `BtlBakuganPlayKindSound`) is playing on the unit's sound object (`SndObjectHasSound`). */
int BtlBakuganHasKindSound(BtlBakugan *bakugan, int slot)
{
    return SndObjectHasSound(bakugan->base.sound,
                             g_btlBakuganKindSoundIds[bakugan->base.base.unk08 - 1][slot]);
}
