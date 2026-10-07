// bdc 0x0886340c BtlBakuganStopLoopSounds
#include "bdc.h"

/* Stops a unit's sounds on its sound object (`base.sound`, `+0x12c`; nothing when there is none):
   all of them when `all`, otherwise clears flag 0x100000 of `stateFlags` (`+0x144`) and stops the
   loop sounds 0x2001f5 and 0x2001f7 (`SndObjectStopSound`). */

void BtlBakuganStopLoopSounds(BtlBakugan *self, bool all)
{
    SndObject *sound;

    sound = self->base.sound;
    if (sound == NULL) {
        return;
    }
    if (all) {
        SndObjectStopSound(sound, -1);
        return;
    }
    self->stateFlags = self->stateFlags & ~0x100000u;
    SndObjectStopSound(sound, 0x2001f5);
    SndObjectStopSound(self->base.sound, 0x2001f7);
}
