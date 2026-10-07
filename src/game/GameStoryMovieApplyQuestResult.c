// bdc 0x088ce098 GameStoryMovieApplyQuestResult
#include "bdc.h"

/* Applies the quest result of the story movie task (`GameStoryMovie`, task id 520) when its
   `questMode` is nonzero: sets the event area/block (`g_gameEventFlags[0]`/`[2]`) to stage/4 and
   stage%4 (script variable 1); in clear mode (2) sets or clears the event flags listed in the
   `GameStoryQuestData` (`GameEventFlagSet`/`GameEventFlagClear`). Then by the data's
   `nextMode` (2 when there is no data or the mode is not 2): 0 goes to the hub (stage 0x20, field
   entry mode 1); 1 does the same when the profile's `playthrough` is nonzero, else requests 10;
   2 sets field entry mode 2, records the stage in the profile (`SaveProfileClearPlacedHolograms`,
   `SaveProfileSetMapMode`, words 0x33/0x2e/0x31) and requests 2, or 8 when the stage info's
   word 1 is 1 or global bit 0x1c is clear, and on stage 0x25 with bit 0x1d sets script variable 15
   to 0x11; any other value requests 0. The request goes to script variable 3, then global bit 0x21
   is set (`CoreBitsetSet`). */

void GameStoryMovieApplyQuestResult(GameStoryMovie *self)

{
  const GameStoryQuestData *data;
  const GameStoryQuestData *list;
  s32 nextMode;
  s32 request;
  s32 stage;
  s32 i;
  u16 info[10];

  if (self->questMode == 0) {
    return;
  }
  g_gameEventFlags[0] = g_scriptGlobalVars[1] / 4;
  g_gameEventFlags[2] = g_scriptGlobalVars[1] % 4;
  request = 0;
  data = self->questData;
  if (self->questMode == 2 && data != NULL && data->flagCount != 0) {
    list = data;
    i = 0;
    do {
      if (list->flags[i].set != 0) {
        GameEventFlagSet(list->flags[i].id);
      }
      else {
        GameEventFlagClear(list->flags[i].id);
      }
      data = self->questData;
      i++;
    } while (i < data->flagCount);
  }
  nextMode = 2;
  if (data != NULL && self->questMode == 2) {
    nextMode = data->nextMode;
  }
  if (nextMode <= 0) {
    if (nextMode >= 0) {
      g_gameFieldEntryMode = 1;
      g_scriptGlobalVars[1] = 0x20;
    }
  }
  else if (nextMode < 2) {
    if (SaveGetProfile()->data->playthrough > 0) {
      g_gameFieldEntryMode = 1;
      g_scriptGlobalVars[1] = 0x20;
    }
    else {
      g_gameFieldEntryMode = 1;
      request = 10;
    }
  }
  else if (nextMode < 3) {
    g_gameFieldEntryMode = 2;
    stage = g_scriptGlobalVars[1];
    request = 2;
    GameStageGetInfo(info, (u8)(stage / 4), (u8)(stage % 4));
    if (info[1] == 1 || !CoreBitsetTest(0x1c, g_scriptGlobalBits)) {
      request = 8;
    }
    SaveProfileClearPlacedHolograms();
    SaveProfileSetMapMode(2, (u8)g_scriptGlobalVars[1]);
    SaveProfileSetWord(SaveGetProfile(), 0x33, g_scriptGlobalVars[1]);
    SaveProfileSetWord(SaveGetProfile(), 0x2e, 0);
    SaveProfileSetWord(SaveGetProfile(), 0x31, stage);
    if (g_scriptGlobalVars[1] == 0x25 && CoreBitsetTest(0x1d, g_scriptGlobalBits)) {
      g_scriptGlobalVars[15] = 0x11;
    }
  }
  g_scriptGlobalVars[3] = request;
  CoreBitsetSet(0x21, g_scriptGlobalBits);
}
