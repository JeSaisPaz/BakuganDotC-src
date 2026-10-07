// bdc 0x08862a44 BtlBakuganPlayKindSound
#include "bdc.h"

/* Plays sound slot `slot` of the unit's kind: looks up the sound id in
   `g_btlBakuganKindSoundIds` (24 slots per kind, 1-based kind in the object's `unk08`) and starts
   it on the unit's sound object with `SndObjectAddEmitter`. */
void BtlBakuganPlayKindSound(BtlBakugan *bakugan, int slot, u8 loop, u8 skipIfPlaying)
{
    SndObjectAddEmitter(bakugan->base.sound, g_btlBakuganKindSoundIds[bakugan->base.base.unk08 - 1][slot],
                        loop, skipIfPlaying);
}
