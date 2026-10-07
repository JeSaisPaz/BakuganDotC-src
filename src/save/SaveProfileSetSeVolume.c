// bdc 0x0880c8b4 SaveProfileSetSeVolume
#include "bdc.h"

/* Sets the sound-effect (master) volume option: clamps `level` to 0..10, stores it at `*profile +
   0x6a9` and applies `level × 0.1` with `SndManagerSetMasterVolume`. Does nothing when no sound
   manager exists. */

void SaveProfileSetSeVolume(SaveProfile *self, s32 level)
{
    if (SndHasManager()) {
        if (level < 0) {
            level = 0;
        } else if (level > 10) {
            level = 10;
        }
        self->data->seVolume = (u8)level;
        SndManagerSetMasterVolume((float)level * 0.1f, SndGetManager());
    }
}
