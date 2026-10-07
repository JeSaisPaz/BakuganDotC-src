// bdc 0x089c8f4c SndIsGroupLoaderIdle
#include "bdc.h"

/* Global wrapper around `SndGroupLoaderIsIdle`: returns 1 when there is no group loader at all or
   when the loader has no outstanding or pending sound-group requests (`id = -1` means 'any
   request'), else 0. The scene state machine `GameFieldPhaseExit` polls it in its state 4 to wait
   until the sound banks for the scene have finished loading before it continues. */

s32 SndIsGroupLoaderIdle(void)
{
  s32 idle = 1;

  if (SndHasGroupLoader()) {
    SndGroupLoader *loader = SndGetGroupLoader();
    idle = SndGroupLoaderIsIdle(loader, -1);
  }
  return idle;
}
