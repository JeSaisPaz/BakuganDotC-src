// bdc 0x088c0f88 GameFieldRestoreReturnLocation
#include "bdc.h"

/* Copies the return-location backup (`g_gameReturnLocationPos`, `g_gameReturnLocationHeading`,
   `GameFieldSaveReturnLocation`) back into the current location record `*g_gameEventLocationBlock`
   and asks the quest camera for a snap (`GameFieldCameraSnapQuestCam`). */

void GameFieldRestoreReturnLocation(CoreTask *task)
{
  u32 *rec = *(u32 **)g_gameEventLocationBlock;

  rec[0] = g_gameReturnLocationPos[0];
  rec[1] = g_gameReturnLocationPos[1];
  rec[2] = g_gameReturnLocationPos[2];
  ((s16 *)rec)[7] = g_gameReturnLocationHeading;
  GameFieldCameraSnapQuestCam((GameFieldCamera *)(task + 2));
}
