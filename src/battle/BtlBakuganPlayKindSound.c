// bdc 0x08862a44 BtlBakuganPlayKindSound
#include "bdc.h"

/* Plays sound slot `slot` of the unit's kind: looks up the sound id in
   `g_btlBakuganKindSoundIds` (24 slots per kind, 1-based kind in the object's `unk08`) and starts
   it on the unit's sound object with `SndObjectAddEmitter`. A kind outside the table (the field
   crystals' kinds 33/34, which the original reads past it into a float and a jump table) plays
   nothing. */
void BtlBakuganPlayKindSound(BtlBakugan *bakugan, int slot, u8 loop, u8 skipIfPlaying)
{
    u32 row = bakugan->base.base.unk08 - 1;

    if (row >= sizeof(g_btlBakuganKindSoundIds) / sizeof(g_btlBakuganKindSoundIds[0])) {
        return;
    }
    SndObjectAddEmitter(bakugan->base.sound, g_btlBakuganKindSoundIds[row][slot], loop, skipIfPlaying);
}
