// bdc 0x0890a8e8 UiLoadingCtor
#include "bdc.h"

/* Constructor of the now-loading screen task (task id 10100 / 0x2774, 0x240 bytes, created by
   `CoreTaskNewById`, vtable `g_uiLoadingVtbl`: update `UiLoadingUpdate`, draw
   `UiLoadingDraw`, destructor `UiLoadingDtor`): `CoreTaskInit`, then builds the shared
   objects `g_uiLoadingShared` (`UiLoadingInitShared`, clearing `unk3c`) or clears the existing
   message box (`UiTextBoxClear`). State 1 / step 0; hides every shared sprite, then shows
   sprites 21 and 22 (sprite 21 with alpha 0); resets the tip, ball and propeller fields
   (`propRot` zeroed from the bank zero vector C720) and clears the shared tip request/texture.
   Takes the tip index from script global 13 (`g_scriptGlobalVars[13]`, set by
   `ScriptOpPickLoadingTip`) into `theme` and looks it up in `g_loadingTipKinds` (`{kind, n}`):
   kind 1 requests `data/nowloading/tips_t_%03d.tm2` with message `n-1`, kind 2 `tips_%03d.tm2`
   (language-specific `tips_%03d_%s.tm2` for n = 3 and 6, `SaveGetLanguageDirName`) with message
   `n+7`; other kinds set `noTip`. A requested file is loaded asynchronously into the shared
   `tipBuffer` (`IoDataMngRequest` with path `g_loadingTipPath`, `IoDataAddFlags` 2); when the
   request exists, the message comes from `DRMesLoading` (`SaveFindLocalizedBin`,
   `UiMesTableRelocate`) and is in range, it is copied to `tipText` (else `tipText` stays "").
   Saves and zeroes `g_gfxDisplay->frameSkip`, then runs the first update. Returns `self`. */

UiLoading *UiLoadingCtor(UiLoading *self)
{
  char name[36];
  const s32 *entry;
  u32 *table;
  const char *lang;
  IoDataMng *mng;
  bool load;
  s32 kind;
  s32 n;
  s32 msg;
  s32 i;

  CoreTaskInit(&self->base);
  self->base.vtable = &g_uiLoadingVtbl;
  if (g_uiLoadingShared == NULL) {
    UiLoadingInitShared();
    g_uiLoadingShared->unk3c = 0;
  } else {
    UiTextBoxClear(g_uiLoadingShared->box);
  }
  self->state = 1;
  self->step = 0;
  for (i = 0; i < g_uiLoadingShared->spriteCount; i++) {
    g_uiLoadingShared->sprites[i]->flags &= ~1u;
  }
  g_uiLoadingShared->sprites[21]->alpha = 0.0f;
  g_uiLoadingShared->sprites[21]->flags |= 1;
  g_uiLoadingShared->sprites[22]->flags |= 1;
  self->tipScroll = 0.0f;
  self->tipScrollEnd = -1.0f;
  self->tipScrollTimer = 0.0f;
  self->noTip = 0;
  g_uiLoadingShared->tipData = NULL;
  g_uiLoadingShared->tipTexture = NULL;
  self->ballSwing = 0.05f;
  self->ballPhase = 0.0f;
  strcpy(self->tipText, "");
  self->frameScale = 0.0f;
  self->frameCounter = 0;
  self->propTimer = 0;
  self->propPhase = 0;
  self->propPos[0] = 0.0f;
  self->propPos[1] = 0.0f;
  self->propPos[2] = 0.0f;
  self->propPos[3] = 0.0f;
  self->propVel[0] = 0.0f;
  self->propVel[1] = 0.0f;
  self->propVel[2] = 0.0f;
  self->propVel[3] = 0.0f;
  self->propAccel[0] = -0.03f;
  self->propAccel[1] = 0.01f;
  self->propAccel[2] = 0.0f;
  self->propAccel[3] = 0.0f;
  self->propRot[0] = 0.0f;
  self->propRot[1] = 0.0f;
  self->propRot[2] = 0.0f;
  self->propRot[3] = 0.0f;
  self->propSpin = 0.0f;
  self->propFrame = 0;
  self->iconGame = 0;
  self->iconHit = 0;
  self->unk23a = 0;
  self->tipWidth = 0.0f;
  self->tipHeight = 0.0f;
  self->theme = g_scriptGlobalVars[13];

  load = false;
  msg = 0;
  table = SaveFindLocalizedBin("DRMesLoading");
  entry = g_loadingTipKinds[self->theme];
  kind = entry[0];
  if (kind < 2) {
    if (kind > 0) {
      sprintf(name, "tips_t_%03d.tm2", entry[1]);
      load = true;
      msg = g_loadingTipKinds[self->theme][1];
      self->noTip = 0;
      msg = msg - 1;
    } else {
      self->noTip = 1;
    }
  } else if (kind < 3) {
    n = entry[1];
    if (n == 3 || n == 6) {
      lang = SaveGetLanguageDirName();
      sprintf(name, "tips_%03d_%s.tm2", n, lang);
    } else {
      sprintf(name, "tips_%03d.tm2", n);
    }
    msg = g_loadingTipKinds[self->theme][1] + 7;
    load = true;
    self->noTip = 0;
  } else {
    self->noTip = 1;
  }

  if (load) {
    sprintf(g_loadingTipPath, "data/nowloading/%s", name);
    mng = IoGetDataMng();
    g_uiLoadingShared->tipData =
        IoDataMngRequest(mng, &g_uiLoadingShared->tipData, g_loadingTipPath,
                         (u32)(uintptr_t)g_uiLoadingShared->tipBuffer, false, false);
    if (g_uiLoadingShared->tipData != NULL) {
      IoDataAddFlags(g_uiLoadingShared->tipData, 2);
      if (table != NULL && msg < (s32)UiMesTableRelocate(table)) {
        strcpy(self->tipText, (const char *)(uintptr_t)table[msg]);
      }
    }
  }
  g_uiLoadingShared->frameSkip = g_gfxDisplay->frameSkip;
  g_gfxDisplay->frameSkip = 0;
  self->waitTimer = 0;
  self->unk01c = 0;
  self->hasTipImage = 0;
  UiLoadingUpdate(self);
  return self;
}
