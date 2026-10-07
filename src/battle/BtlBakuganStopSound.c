// bdc 0x08862b48 BtlBakuganStopSound
#include "bdc.h"

/* Stops sound `soundId` (all sounds for −1) on the unit's sound object `+0x12c`
   (`SndObjectStopSound`). */
void BtlBakuganStopSound(BtlBakugan *bakugan, s32 soundId)
{
    SndObjectStopSound(bakugan->base.sound, soundId);
}
