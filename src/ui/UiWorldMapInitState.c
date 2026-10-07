// bdc 0x08996734 UiWorldMapInitState
#include "bdc.h"

/* Class init of `UiWorldMap` (from `UiWorldMapCtor`): fills the 8-slot area
   tables from a local template (area ids `areaGroup` = {9, 0, 1, 4, 3, 2, 5, 6}, label cells
   `areaLabelCell` = {7, 0, 1, 4, 3, 2, 5, 6}, `areaHasFlag` all zero), computes the unlocked-area
   mask `unlockMask` (bit per slot from the profile bitfield `storyAreaBits` indexed by area id,
   `areaCleared` in rank mode, `UiWorldMapIsRankMode`) and the selectable-area mask
   `selectableMask` (all slots in story mode; in rank mode slots 1..5, slot 6 too when unlocked
   (also marked blocked in `blockedMask`), or every slot but 0 when profile byte `playthrough` is
   set), selects the first selectable slot (`areaId`; `shownAreaId` = -1), clears the turn/jet/
   confirm/text records, loads the stage ranks (`UiWorldMapLoadStageRanks`) and sets
   `randomEnabled` = !rank mode. */

void UiWorldMapInitState(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 tmpl[5][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, /* slot present */
    {9, 0, 1, 4, 3, 2, 5, 6}, /* areaGroup */
    {7, 0, 1, 4, 3, 2, 5, 6}, /* areaLabelCell */
    {0, 0, 0, 0, 0, 0, 0, 0}, /* areaHasFlag */
    {0, 1, 1, 1, 1, 1, 0, 0}, /* selectable in rank mode */
  };
  int i;
  int area;

  map->areaId = 0;
  map->shownAreaId = -1;
  map->stage = 0;
  map->globeMotion = 0;
  map->spinForward = 0;
  map->unlockMask = 0;
  for (i = 0; i < 8; i++) {
    if (tmpl[0][i] == 0) {
      continue;
    }
    if (!UiWorldMapIsRankMode(screen)) {
      area = tmpl[1][i];
      if ((SaveGetProfile()->data->storyAreaBits[area / 8] & (1 << (area % 8))) != 0) {
        map->unlockMask |= 1 << i;
      }
    } else {
      area = tmpl[1][i];
      if ((SaveGetProfile()->data->areaCleared[area / 8] & (1 << (area % 8))) != 0) {
        map->unlockMask |= 1 << i;
      }
    }
  }

  map->selectableMask = 0;
  map->blockedMask = 0;
  for (i = 0; i < 8; i++) {
    if (!UiWorldMapIsRankMode(screen)) {
      map->selectableMask |= 1 << i;
    } else if (SaveGetProfile()->data->playthrough != 0) {
      if (i != 0) {
        map->selectableMask |= 1 << i;
      }
    } else if (tmpl[4][i] != 0) {
      map->selectableMask |= 1 << i;
    } else if (i == 6 && (map->unlockMask & (1 << i)) != 0) {
      map->selectableMask |= 1 << i;
      map->blockedMask |= 1 << i;
    }
  }

  for (i = 0; i < 8; i++) {
    if ((map->selectableMask & (1 << i)) != 0) {
      map->areaId = i;
      break;
    }
  }
  for (i = 0; i < 8; i++) {
    map->areaGroup[i] = tmpl[1][i];
  }
  for (i = 0; i < 8; i++) {
    map->areaLabelCell[i] = tmpl[2][i];
  }
  for (i = 0; i < 8; i++) {
    map->areaHasFlag[i] = tmpl[3][i];
  }

  map->mapModel = NULL;
  map->globePitch = 0.0f;
  map->globeYaw = 0.0f;
  map->globeRoll = 0.0f;
  map->globeRotW = 0.0f;
  memset(map->turnFrom, 0, 0x60);
  map->unk2250 = 0.0f;
  map->unk2254[0] = 0;
  map->unk2254[1] = 0;
  map->unk2254[2] = 0;
  map->ringsHidden = 0;
  map->jetModel = NULL;
  map->jetPitch = 0.0f;
  map->jetYaw = 0.0f;
  map->jetRoll = 0.0f;
  map->unk227c = 0.0f;
  map->jetWobble = 0.0f;
  map->jetYawOffset = 0.0f;
  map->jetRollOffset = 0.0f;
  map->unk228c = 0.0f;
  map->jetHoverTimerY = 0.0f;
  map->unk229c = 0;
  map->jetHoverTimerX = 0.0f;
  map->unk2298 = 0.0f;
  map->jetWobbleTimer = 0.0f;
  map->jetActive = 0;
  map->jetScale = 0.0f;
  memset(&map->confirmActive, 0, 0x10);
  UiWorldMapLoadStageRanks(screen);
  memset(map->textSlot, 0, sizeof(map->textSlot));
  map->randomEnabled = !UiWorldMapIsRankMode(screen);
}
