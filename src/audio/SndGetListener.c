// bdc 0x089bffb0 SndGetListener
#include "bdc.h"

/* Returns the 3D-sound listener singleton `g_soundListener` (NULL before the emitter system is
   initialised). About 38 callers fetch it right after `SndHasListener`. */

SndListener *SndGetListener(void)

{
  return g_soundListener;
}

