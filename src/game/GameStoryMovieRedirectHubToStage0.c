// bdc 0x088cdd3c GameStoryMovieRedirectHubToStage0
#include "bdc.h"

/* For the story movie task (task id 520, `GameStoryMovieCtor`) with `questMode` 0 and movie set 0
   on the hub stage 0x20 (script variable 1) while movie 0 of map 1 (`mapMovieWatched[1][0]`) was
   not watched: stores the hub's area/block in `g_gameEventFlags` bytes 0/2, switches to stage 0
   (script variable 1 = 0; profile `stageProfileByte`/`stageSlot` = area/block of the new stage,
   script variable 15 = its map id via `GameStageToMapId`), sets `g_gameFieldEntryMode` = 1,
   script variable 13 = 6 and script variable 3 = 0. */

void GameStoryMovieRedirectHubToStage0(GameStoryMovie *self)

{
  SaveProfile *profile;
  s32 mapId;

  if (self->questMode == 0 && self->set == 0 && g_scriptGlobalVars[1] == 0x20 &&
      SaveGetProfile()->data->mapMovieWatched[1][0] == 0) {
    g_gameEventFlags[0] = (u8)(g_scriptGlobalVars[1] / 4);
    g_gameEventFlags[2] = (u8)(g_scriptGlobalVars[1] % 4);
    g_scriptGlobalVars[1] = 0;
    profile = SaveGetProfile();
    profile->data->stageProfileByte = (u8)(g_scriptGlobalVars[1] / 4);
    profile = SaveGetProfile();
    profile->data->stageSlot = (u8)(g_scriptGlobalVars[1] % 4);
    mapId = GameStageToMapId(g_scriptGlobalVars[1]);
    g_scriptGlobalVars[15] = mapId;
    g_gameFieldEntryMode = 1;
    g_scriptGlobalVars[13] = 6;
    g_scriptGlobalVars[3] = 0;
  }
  return;
}
