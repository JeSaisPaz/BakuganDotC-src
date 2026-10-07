// bdc 0x089c25fc SndHasObjectMgr
#include "bdc.h"

/* Returns whether the sound-object system exists (`g_soundObjectMgr != NULL`). Guards the
   `SndObjectMgrUpdate` call in `BootEndOfFrame`. */

bool SndHasObjectMgr(void)

{
  return g_soundObjectMgr != (void **)0x0;
}

