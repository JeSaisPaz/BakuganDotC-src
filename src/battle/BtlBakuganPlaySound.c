// bdc 0x08862b24 BtlBakuganPlaySound
#include "bdc.h"

/* Starts sound `soundId` on the unit's sound object (`SndObjectAddEmitter`, passing `loop` and
   `skipIfPlaying` through); the emitter's success flag is dropped. */
void BtlBakuganPlaySound(BtlBakugan *bakugan, s32 soundId, u8 loop, u8 skipIfPlaying)
{
    SndObjectAddEmitter(bakugan->base.sound, soundId, loop, skipIfPlaying);
}
