// bdc 0x088c2818 GameFieldRestoreProgress
#include "bdc.h"

/* Initialises the field globals for a new or loaded game (caller `UiMainMenuApplySelection`): clears
   `g_gameFieldReturnKind`, the location block `g_gameEventLocationBlock` (`GameEventClearBlockD40`)
   and the story event state `g_gameEventState` (`GameEventStateClear`). Without stored event flags
   (profile `eventFlagsStored == 0`) it starts on stage 0x25 (script variable 1, map id from
   `GameStageToMapId` into variable 15) and sets the event counter `g_gameEventFlags[5]` to the
   profile's `fieldCounter` capped at 99. Otherwise it copies the saved 0x108-byte event block and the
   16 unlock bytes back into `g_gameEventFlags` / `g_gameUnlockFlags`; with `eventFlagsStored == 2`
   it again starts on stage 0x25 (with its map id), else on stage 0x20 with `g_gameFieldEntryMode` = 0. */

void GameFieldRestoreProgress(void)
{
  u32 *dst;
  const u32 *src;
  int i;
  s8 counter;
  u8 capped;

  g_gameFieldReturnKind = 0;
  GameEventClearBlockD40();
  GameEventStateClear();
  if (SaveGetProfile()->data->eventFlagsStored == 0) {
    g_scriptGlobalVars[1] = 0x25;
    g_scriptGlobalVars[15] = GameStageToMapId(g_scriptGlobalVars[1]);
    counter = SaveGetProfile()->data->fieldCounter;
    capped = 99;
    if (counter < 100) {
      capped = counter;
    }
    g_gameEventFlags[5] = capped;
  }
  else {
    src = (const u32 *)SaveGetProfile()->data->eventFlags;
    dst = (u32 *)g_gameEventFlags;
    for (i = 0x21; i != 0; i--) {
      dst[0] = src[0];
      dst[1] = src[1];
      dst += 2;
      src += 2;
    }
    src = (const u32 *)SaveGetProfile()->data->unlockFlags;
    dst = (u32 *)g_gameUnlockFlags;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    if (SaveGetProfile()->data->eventFlagsStored == 2) {
      g_scriptGlobalVars[1] = 0x25;
      g_scriptGlobalVars[15] = GameStageToMapId(g_scriptGlobalVars[1]);
    }
    else {
      g_scriptGlobalVars[1] = 0x20;
      g_gameFieldEntryMode = 0;
    }
  }
}
