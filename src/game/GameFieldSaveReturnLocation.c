// bdc 0x088bf348 GameFieldSaveReturnLocation
#include "bdc.h"

/* Copies the current location record (pointed to by the first word of `g_gameEventLocationBlock`:
   position words and heading) into the return-location backup `g_gameReturnLocationPos` /
   `g_gameReturnLocationHeading`; `GameFieldRestoreReturnLocation` copies it back. */

void GameFieldSaveReturnLocation(void)
{
  u32 *rec = *(u32 **)g_gameEventLocationBlock;

  g_gameReturnLocationPos[0] = rec[0];
  g_gameReturnLocationPos[1] = rec[1];
  g_gameReturnLocationPos[2] = rec[2];
  g_gameReturnLocationHeading = ((s16 *)rec)[7];
}
