// bdc 0x089edf40 GfxFaderIsFinished
#include "bdc.h"

/* Returns the fader's finished flag (byte `+1`, cleared by `GfxFaderStart`). `ScriptOpFade` cmd
   1 polls it to wait for a fade to end. */

bool GfxFaderIsFinished(GfxFader *self)

{
  return self->done != '\0';
}

