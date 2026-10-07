// bdc 0x089fb5d8 IoUmdIsMediaReady
#include "bdc.h"

/* Returns 1 when both status bytes `g_umdDiscPresent` and `g_umdDriveReady` are set and
   `sceUmdCheckMedium()` reports a medium in the drive, else 0. `NetPlayLateUpdate` runs it every
   frame while in a session: 600 consecutive failures abort the session. */

bool IoUmdIsMediaReady(void)

{
  bool ready;

  ready = false;
  if ((g_umdDiscPresent != 0) && (g_umdDriveReady != 0) && (sceUmdCheckMedium() != 0)) {
    ready = true;
  }
  return ready;
}
