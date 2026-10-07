// bdc 0x0880c93c SaveProfileSetVoiceVolume
#include "bdc.h"

/* Sets the voice volume option: clamps `level` to 0..10, stores it at `*profile + 0x6aa` and
   applies `level × 0.1` with `SndManagerSetVoiceVolume`. Does nothing when no sound manager
   exists. */

void SaveProfileSetVoiceVolume(SaveProfile *self, s32 level)
{
    if (SndHasManager()) {
        if (level < 0) {
            level = 0;
        } else if (level > 10) {
            level = 10;
        }
        self->data->voiceVolume = (u8)level;
        SndManagerSetVoiceVolume((float)level * 0.1f, SndGetManager());
    }
}
