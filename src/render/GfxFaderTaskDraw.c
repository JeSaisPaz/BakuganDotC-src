// bdc 0x089eebb0 GfxFaderTaskDraw
#include "bdc.h"

/* Draw method of the fader task (id 0x274c): draws the active fader (`GfxFaderDrawStep`) when the
   fader system exists. */

void GfxFaderTaskDraw(CoreTask *task)

{
  GfxFader *self;
  
  if (GfxFaderIsReady()) {
    self = GfxGetActiveFader();
    GfxFaderDrawStep(self);
  }
  return;
}

