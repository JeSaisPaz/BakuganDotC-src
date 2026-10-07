// bdc 0x0890cb00 UiLoadingHandleIconInput
#include "bdc.h"

/* Input step of the icon mini-game of the now-loading screen (task 10100 / 0x2774,
   `UiLoadingCtor`, update `UiLoadingUpdate`, draw `UiLoadingDraw`; shared objects
   `g_uiLoadingShared` from `UiLoadingInitShared`); does nothing while `iconGame` is clear.
   On a press of pad button `0x4000` it scans the icon sprites 11..16 of the shared sprite list:
   among those in phase 2 (`(int)params80[0] == 2`) the one with the lowest `(int)angle` gets
   `angle = 0`; when none is in phase 2, the phase-10 icon with the largest `scaleZ` (> 0) has its
   phase advanced by one. Either hit sets `iconHit`. When 6 or more icons were counted in those
   phases, every icon's `angle` is reset to `i * 80` (+240 for i >= 3). Finally every icon is
   animated with `UiLoadingAnimateIcon`. */

void UiLoadingHandleIconInput(UiLoading *self)
{
  GfxSprite **sprites;
  GfxSprite *icon;
  s32 count;
  s32 sel;
  s32 bestKey;
  s32 key;
  s32 found;
  s32 i;
  float best;
  float angle;

  if (self->iconGame == 0) {
    return;
  }
  count = 0;
  if ((g_padState->pressed & 0x4000) != 0) {
    sprites = g_uiLoadingShared->sprites;
    sel = -1;
    found = 0;
    bestKey = 0x7fffffff;
    for (i = 11; i < 17; i++) {
      icon = sprites[i];
      key = (s32)icon->angle;
      if ((s32)icon->maybe_billboardParams80[0] == 2) {
        if (key < bestKey) {
          bestKey = key;
          sel = i;
        }
        count++;
      }
    }
    if (sel != -1) {
      found = 1;
      sprites[sel]->angle = 0.0f;
      self->iconHit = 1;
    }
    if (!found) {
      sprites = g_uiLoadingShared->sprites;
      best = 0.0f;
      for (i = 11; i < 17; i++) {
        icon = sprites[i];
        if ((s32)icon->maybe_billboardParams80[0] == 10) {
          if (best < icon->scaleZ) {
            best = icon->scaleZ;
            sel = i;
          }
          count++;
        }
      }
      if (sel != -1) {
        sprites[sel]->maybe_billboardParams80[0] = sprites[sel]->maybe_billboardParams80[0] + 1.0f;
        self->iconHit = 1;
      }
    }
  }
  if (count >= 6) {
    for (i = 11; i < 17; i++) {
      angle = (float)((i - 11) * 80);
      if (!(i - 11 < 3)) {
        angle = angle + 240.0f;
      }
      g_uiLoadingShared->sprites[i]->angle = angle;
    }
  }
  for (i = 11; i < 17; i++) {
    UiLoadingAnimateIcon(self, g_uiLoadingShared->sprites[i], i - 11);
  }
}
