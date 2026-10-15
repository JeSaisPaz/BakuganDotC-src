// bdc 0x08862adc BtlBakuganHasKindSound
#include "bdc.h"

/* Returns whether sound slot `slot` of the unit's kind (`g_btlBakuganKindSoundIds`, see
   `BtlBakuganPlayKindSound`) is playing on the unit's sound object (`SndObjectHasSound`).
   The table has rows for kinds 1..32; the field crystals' units (kinds 33/34) read past it in the
   original (a float, then a jump table of code addresses: ids that name no sound), so here a kind
   outside the table has no sound playing. */
int BtlBakuganHasKindSound(BtlBakugan *bakugan, int slot)
{
    u32 row = bakugan->base.base.unk08 - 1;

    if (row >= sizeof(g_btlBakuganKindSoundIds) / sizeof(g_btlBakuganKindSoundIds[0])) {
        return 0;
    }
    return SndObjectHasSound(bakugan->base.sound, g_btlBakuganKindSoundIds[row][slot]);
}
