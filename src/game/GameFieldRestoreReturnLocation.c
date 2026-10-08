// bdc 0x088c0f88 GameFieldRestoreReturnLocation
#include "bdc.h"

/* Copies the return-location backup (`g_gameReturnLocationPos`, `g_gameReturnLocationHeading`,
   `GameFieldSaveReturnLocation`) back into the current location record `*g_gameEventLocationBlock`
   and asks the quest camera for a snap (`GameFieldCameraSnapQuestCam`). */

void GameFieldRestoreReturnLocation(CoreTask *task)
{
  GameFieldPlacedChar *rec = g_gameEventLocationBlock[0];

  rec->pos[0] = g_gameReturnLocationPos[0];
  rec->pos[1] = g_gameReturnLocationPos[1];
  rec->pos[2] = g_gameReturnLocationPos[2];
  rec->rot[1] = g_gameReturnLocationHeading;
  GameFieldCameraSnapQuestCam((GameFieldCamera *)((GameFieldTask *)task)->camera);
}
