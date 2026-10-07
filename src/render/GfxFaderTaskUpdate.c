// bdc 0x089eeb7c GfxFaderTaskUpdate
#include "bdc.h"

/* Update method of the fader task (id 0x274c): steps the active fader (`GfxFaderStep`) when the
   fader system exists. */

void GfxFaderTaskUpdate(CoreTask *task)

{
  GfxFader *self;
  
  if (GfxFaderIsReady()) {
    self = GfxGetActiveFader();
    GfxFaderStep(self);
  }
  return;
}

