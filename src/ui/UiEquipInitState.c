// bdc 0x08956994 UiEquipInitState
#include "bdc.h"

/* Initialises the state of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`). Coming from task 410 (`g_lastScreenTaskId` == 0x19a) it first resets profile
   words 3..6 to -1 (3/4 to 0 in network mode) and the four player records of the profile battle
   block (words 0x3e+p*2+i and 0x36+p*2+i = 0xff, word 0x46+p = 100). Then reads the player count
   (word 0x16) and the per-player Bakugan picks (words 3..6), places the grid cursor on the last
   picked Bakugan (all four players done in network mode), hides the gauges, allocates the 2 or 4
   slot cameras, sets the player icon rows, flags re-editing when coming from task 310, resets the
   battle options when coming from task 300 and builds the candidate lists
   (`UiEquipBuildGearLists`). */

void UiEquipInitState(UiEquip *self)
{
  SaveProfile *profile;
  void *block;
  GfxCamera *cameras;
  bool fromLow;
  u8 icon;
  u32 p;
  u32 i;

  g_equipPendingFlag = 0;
  if (g_lastScreenTaskId == 0x19a) {
    SaveProfileSetWord(SaveGetProfile(), 3, 0xffffffff);
    SaveProfileSetWord(SaveGetProfile(), 4, 0xffffffff);
    SaveProfileSetWord(SaveGetProfile(), 5, 0xffffffff);
    SaveProfileSetWord(SaveGetProfile(), 6, 0xffffffff);
    if (SaveGetProfileFlag0() != 0) {
      SaveProfileSetWord(SaveGetProfile(), 3, 0);
      SaveProfileSetWord(SaveGetProfile(), 4, 0);
    }
    for (p = 0; p < 4; p++) {
      for (i = 0; i < 2; i++) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x3e + p * 2 + i] = 0xff;
        }
      }
      for (i = 0; i < 2; i++) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x36 + p * 2 + i] = 0xff;
        }
      }
      profile = SaveGetProfile();
      if (profile->words != NULL) {
        profile->words[0x46 + p] = 100;
      }
    }
  }
  self->playerCount = (s8)SaveProfileGetWord(SaveGetProfile(), 0x16);
  for (i = 0; i < 4; i++) {
    self->bakuganPick[i] = (u8)SaveProfileGetWord(SaveGetProfile(), i + 3);
  }
  self->gridCursor = 0;
  self->onRandom = 0;
  self->editPlayer = 0;
  self->doneCount = 0;
  if (SaveGetProfileFlag0() != 0) {
    self->doneCount = (s8)i;
  }
  else {
    for (i = 0; i < 4; i++) {
      if ((s8)self->bakuganPick[i] == -1) {
        break;
      }
      icon = UiEquipMapBakuganIndex(self, 1, self->bakuganPick[i]);
      self->editPlayer = (s8)i;
      self->gridCursor = (s8)icon;
    }
    self->doneCount = (s8)i;
  }
  for (i = 0; i < 4; i++) {
    self->gaugeScroll[i].scrollFast = -1.0f;
    self->gaugeScroll[i].scrollSlow = -1.0f;
  }
  memset(self->bakuganModels, 0, 0x10);
  memset(self->pedestalModels, 0, 0x10);
  memset(&self->slotCameras, 0, 4);
  if (self->playerCount < 3) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(2 * sizeof(GfxCamera) + 0x10 /* PSP: CxxVecNew cookie */, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    cameras = NULL;
    if (block != NULL) {
      cameras = CxxVecNew((u8 *)block + g_cxxVecCookieSize, 2, sizeof(GfxCamera), GfxCameraCtor, 0);
    }
    self->slotCameras = cameras;
  }
  else {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(4 * sizeof(GfxCamera) + 0x10 /* PSP: CxxVecNew cookie */, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    cameras = NULL;
    if (block != NULL) {
      cameras = CxxVecNew((u8 *)block + g_cxxVecCookieSize, 4, sizeof(GfxCamera), GfxCameraCtor, 0);
    }
    self->slotCameras = cameras;
  }
  memset(self->stampRec, 0, 0x30);
  memset(self->playerIconCell, 0, 4);
  if (self->playerCount < 3) {
    icon = 2;
    if (SaveGetProfileFlag0() != 0) {
      icon = 1;
    }
    self->playerIconCell[1] = icon;
  }
  else if (SaveGetProfileFlag0() != 0) {
    self->playerIconCell[1] = 1;
    self->playerIconCell[2] = 5;
    self->playerIconCell[3] = 6;
  }
  else {
    self->playerIconCell[1] = 2;
    self->playerIconCell[2] = 3;
    self->playerIconCell[3] = 4;
  }
  if (g_lastScreenTaskId == 0x136) {
    self->reEditLast = 1;
  }
  else {
    self->reEditLast = 0;
  }
  memset(&self->pulseOn, 0, 0xc);
  self->pulseAlpha = 1.0f;
  memset(&self->markerState, 0, 4);
  if (g_lastScreenTaskId == 300) {
    UiEquipResetBattleOptions();
  }
  UiEquipBuildGearLists(self);
}
