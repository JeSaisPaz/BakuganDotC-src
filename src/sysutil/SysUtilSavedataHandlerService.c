// bdc 0x089cd230 SysUtilSavedataHandlerService
#include "bdc.h"

/* Savedata handler virtual slot `+0x24`, polled by `SysUtilPoll`: drives the busy word of
   `g_savedataParams`. Busy 1 (requested) calls `sceUtilitySavedataInitStart` once the dialog state
   (`SysUtilHandlerGetState`) is not 1..4 and moves to 2; busy 3 (retry) goes back to 1; busy 2/4/5
   with the dialog in state 3 calls `sceUtilitySavedataShutdownStart` (→ 5); busy 5 with the dialog
   back to state 0 clears busy, disables the handler and converts the SCE result into the game result
   word read by `SysUtilSavedataGetResult`; the autoload request (3) then moves on to the next save
   slot unless a slot was found. Always returns 1. */

s32 SysUtilSavedataHandlerService(SysUtilSavedataHandler *self)
{
  s32 state;
  s32 sceResult;
  s32 slot;
  s32 sizeCheck;
  s32 advance;
  s32 result;

  switch (g_savedataParams->busy) {
  case 1:
    state = SysUtilHandlerGetState(&self->base);
    if (state > 0 && state < 5) {
      return 1;
    }
    if (sceUtilitySavedataInitStart(&g_savedataParams->params) == 0) {
      g_savedataParams->busy = 2;
    }
    return 1;
  case 3:
    g_savedataParams->busy = 1;
    return 1;
  case 2:
  case 4:
  case 5:
    break;
  default:
    return 1;
  }

  state = SysUtilHandlerGetState(&self->base);
  if (state == 3) {
    if (sceUtilitySavedataShutdownStart() == 0) {
      g_savedataParams->busy = 5;
    }
    return 1;
  }
  if (state != 0) {
    return 1;
  }
  if (g_savedataParams->busy != 5) {
    return 1;
  }
  g_savedataParams->busy = 0;
  SysUtilHandlerSetEnabled(&self->base, 0);
  g_savedataParams->result = 3;

  switch ((u32)g_savedataParams->params.base.result) {
  case 0x80110301: /* LOAD_NO_MS */
  case 0x80110302: /* LOAD_EJECT_MS */
  case 0x80110341: /* DELETE_NO_MS */
  case 0x80110342: /* DELETE_EJECT_MS */
  case 0x80110381: /* SAVE_NO_MS */
  case 0x80110382: /* SAVE_EJECT_MS */
  case 0x801103c1: /* SIZES_NO_MS */
  case 0x801103c2: /* SIZES_EJECT_MS */
    g_savedataParams->result = 5;
    break;
  case 0x80110307: /* LOAD_NO_DATA */
  case 0x80110347: /* DELETE_NO_DATA */
  case 0x801103c7: /* SIZES_NO_DATA */
    g_savedataParams->result = 6;
    break;
  case 0x80110344: /* DELETE_MS_PROTECTED */
  case 0x80110384: /* SAVE_MS_PROTECTED */
    g_savedataParams->result = 4;
    break;
  case 0x80110383: /* SAVE_NO_SPACE */
    g_savedataParams->result = 7;
    break;
  case 2:
    if (g_savedataParams->params.abortStatus == 0) {
      g_savedataParams->result = 1;
    } else if (g_savedataParams->params.abortStatus == 1) {
      g_savedataParams->result = 3;
    }
    break;
  case 1:
    g_savedataParams->result = 2;
    break;
  case 0:
    g_savedataParams->result = 1;
    if (self->request <= 1 || self->request == 13) {
      for (slot = 0; slot < 8; slot++) {
        if (strcmp(g_savedataParams->params.saveName, g_savedataParams->saveNameList[slot]) == 0) {
          g_saveSlotIndex = slot;
          break;
        }
      }
    }
    break;
  default:
    break;
  }

  /* Size check: after a success, a missing-data size query, or a save that ran out of space. */
  sizeCheck = 0;
  if (g_savedataParams->result == 1) {
    sizeCheck = 1;
  } else {
    sceResult = g_savedataParams->params.base.result;
    if (sceResult > (s32)0x801103c5) {
      if (sceResult <= (s32)0x801103c7) {
        g_savedataParams->result = 1;
        sizeCheck = 1;
      }
    } else if (sceResult == (s32)0x80110383) {
      sizeCheck = 1;
    }
  }
  if (sizeCheck && g_savedataParams->params.msFree != NULL &&
      g_savedataParams->params.utilityData != NULL) {
    if ((u32)g_savedataParams->params.msFree->freeClusters <
        (u32)g_savedataParams->params.utilityData->usedClusters) {
      g_savedataParams->result = 7;
    }
    g_saveRequiredBytes = g_savedataParams->params.utilityData->usedClusters;
    g_saveRequiredBytes = g_saveRequiredBytes * g_savedataParams->params.msFree->clusterSize;
  }

  /* Autoload: stop on a found slot (result 1) or result 5, else retry with the next save name. */
  if (self->request == 3) {
    result = g_savedataParams->result;
    advance = 1;
    if (result < 2) {
      if (result > 0) {
        advance = 0;
        self->result = self->slotIndex;
      }
    } else if (result == 5) {
      advance = 0;
    }
    if (advance) {
      self->slotIndex = self->slotIndex + 1;
      if (self->slotIndex < 8) {
        strcpy(g_savedataParams->params.saveName, g_savedataParams->saveNameList[self->slotIndex]);
        g_savedataParams->result = 0;
        g_savedataParams->busy = 3;
        SysUtilHandlerSetEnabled(&self->base, 1);
      }
    }
  }
  return 1;
}
