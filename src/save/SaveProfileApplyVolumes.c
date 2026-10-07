// bdc 0x0880d35c SaveProfileApplyVolumes
#include "bdc.h"

/* Pushes the profile's three stored volume options (`*profile + 0x6a8` BGM, `+0x6a9` SE, `+0x6aa`
   voice, 0..10) to the sound manager: `SndManagerSetBgmVolume``(v × 0.1 × 0.7)`,
   `SndManagerSetMasterVolume``(v × 0.1)`, `SndManagerSetVoiceVolume``(v × 0.1)`. No-op
   without a sound manager. */

void SaveProfileApplyVolumes(SaveProfile *self)
{
  if (SndHasManager()) {
    SndManagerSetBgmVolume((float)self->data->bgmVolume * 0.1f * 0.7f, SndGetManager());
    SndManagerSetMasterVolume((float)self->data->seVolume * 0.1f, SndGetManager());
    SndManagerSetVoiceVolume((float)self->data->voiceVolume * 0.1f, SndGetManager());
  }
}
