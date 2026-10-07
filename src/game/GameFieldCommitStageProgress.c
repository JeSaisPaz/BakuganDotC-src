// bdc 0x088c1e74 GameFieldCommitStageProgress
#include "bdc.h"

/* Records the current stage (script variable 1) in the profile (words 0x33 and 0x31, clears 0x2e),
   sets `g_gameFieldEntryMode` = 2, the leave byte `leaveRequest = 0xe` and `nextRequest = 2` of the field
   task, calls `SaveProfileClearPlacedHolograms`/`SaveProfileSetMapMode(2, stage)`; on stage 0x14 it
   redirects to stage 0x18 (script variable 15 = 0x11) when movie set 1 of map 17 was watched
   (profile byte `+0x456`), else to stage 0x25 with the fixed return location
   `g_gameFieldStage37ReturnPos` (heading `0x704f`, transition parameter from `g_gameEventFlags` +6). */

void GameFieldCommitStageProgress(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;
  u32 stage;

  stage = g_scriptGlobalVars[1];
  g_gameFieldEntryMode = 2;
  field->leaveRequest = 0xe;
  field->nextRequest = 2;
  SaveProfileClearPlacedHolograms();
  SaveProfileSetMapMode(2, (u8)stage);
  SaveProfileSetWord(SaveGetProfile(), 0x33, stage);
  SaveProfileSetWord(SaveGetProfile(), 0x2e, 0);
  SaveProfileSetWord(SaveGetProfile(), 0x31, stage);
  if (g_scriptGlobalVars[1] == 0x14) {
    if (SaveGetProfile()->data->mapMovieWatched[17][1] == 0) {
      g_scriptGlobalVars[1] = 0x25;
      SaveProfileSetWord(SaveGetProfile(), 0x33, 0x25);
      g_gameTransitionParam = *(s16 *)&g_gameEventFlags[6];
      g_gameReturnLocationPos[0] = g_gameFieldStage37ReturnPos[0];
      g_gameReturnLocationPos[1] = g_gameFieldStage37ReturnPos[1];
      g_gameReturnLocationPos[2] = g_gameFieldStage37ReturnPos[2];
      g_gameReturnLocationHeading = 0x704f;
    }
    else {
      g_scriptGlobalVars[1] = 0x18;
      SaveProfileSetWord(SaveGetProfile(), 0x33, 0x18);
      g_scriptGlobalVars[0xf] = 0x11;
    }
  }
  return;
}
