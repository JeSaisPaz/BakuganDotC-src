// bdc 0x0880c81c SaveProfileSetBgmVolume
#include "bdc.h"

/* Sets the BGM volume option: clamps `level` to 0..10, stores it at `*profile + 0x6a8` and applies
   `level × 0.1 × 0.7` with `SndManagerSetBgmVolume`. Does nothing (not even the store) when no
   sound manager exists. */

void SaveProfileSetBgmVolume(SaveProfile *self, s32 level)

{
  
  
  SndManager *mgr;
  
  
  if (SndHasManager()) {
    if (level < 0) {
      level = 0;
    }
    else if (10 < level) {
      level = 10;
    }
    self->data->bgmVolume = (u8)level;
    mgr = SndGetManager();
    SndManagerSetBgmVolume((float)level * 0.1f * 0.7f,mgr);
  }
  return;
}

