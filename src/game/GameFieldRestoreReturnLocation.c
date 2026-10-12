// bdc 0x088c0f88 GameFieldRestoreReturnLocation
#include "bdc.h"

/* Copies the return-location backup (`g_gameReturnLocationPos`, `g_gameReturnLocationHeading`,
   `GameFieldSaveReturnLocation`) back into the current location record `*g_gameEventLocationBlock`
   and asks the quest camera for a snap (`GameFieldCameraSnapQuestCam`). */

void GameFieldRestoreReturnLocation(CoreTask *task)
{
  GameFieldPlacedChar *rec = g_gameEventLocationBlock.placed[0];

  rec->pos[0] = g_gameEventLocationBlock.returnPos[0];
  rec->pos[1] = g_gameEventLocationBlock.returnPos[1];
  rec->pos[2] = g_gameEventLocationBlock.returnPos[2];
  rec->rot[1] = g_gameEventLocationBlock.returnHeading;
  GameFieldCameraSnapQuestCam((GameFieldCamera *)((GameFieldTask *)task)->camera);
}
