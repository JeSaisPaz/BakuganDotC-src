// bdc 0x089bff94 SndHasListener
#include "bdc.h"

/* Returns whether the 3D-sound listener `g_soundListener` exists (`g_soundListener != NULL`);
   `BootEndOfFrame` and about 25 game-side callers use it as the guard before touching the
   positional-sound system. */

bool SndHasListener(void)

{
  return g_soundListener != (SndListener *)0x0;
}

