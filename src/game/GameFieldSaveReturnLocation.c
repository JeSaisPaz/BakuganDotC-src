// bdc 0x088bf348 GameFieldSaveReturnLocation
#include "bdc.h"

/* Copies the current location record (pointed to by the first word of `g_gameEventLocationBlock`:
   position words and heading) into the return-location backup `g_gameReturnLocationPos` /
   `g_gameReturnLocationHeading`; `GameFieldRestoreReturnLocation` copies it back. */

void GameFieldSaveReturnLocation(void)
{
  GameFieldPlacedChar *rec = g_gameEventLocationBlock.placed[0];

  g_gameEventLocationBlock.returnPos[0] = rec->pos[0];
  g_gameEventLocationBlock.returnPos[1] = rec->pos[1];
  g_gameEventLocationBlock.returnPos[2] = rec->pos[2];
  g_gameEventLocationBlock.returnHeading = rec->rot[1];
}
