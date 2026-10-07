// bdc 0x08963404 UiEquipRandomPick
#include "bdc.h"

/* Random Bakugan pick of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`), stepped by `rouletteStep`:
   0: collects the Bakugan ids 1..20 the profile owns (`bakuganBitsA`) into `rouletteList` and draws
      the hop count `rouletteSpinTarget` (10 or 11, `CoreRtcGetMicrosecond`);
   1: hops the grid cursor to a random owned Bakugan different from the current cell (up to 65 random
      tries, then the first different one in list order; with one candidate it just takes it and
      stops after one hop), plays sound 1, resets the cursor and frees the current model;
   2: previews the hovered Bakugan and waits 2 frames;
   3: pulses the cursor; when the wait runs out counts the hop and goes back to step 1, or on to step
      4 once `rouletteSpinTarget` hops are done;
   4: marks finished (step 5).
   Returns 1 once the step is past 4, else 0. */

s32 UiEquipRandomPick(UiEquip *self)
{
  GfxSprite **sprites;
  SaveProfile *profile;
  u32 rtc;
  u8 tries;
  s32 i;

  switch (self->rouletteStep) {
  case 0:
    self->rouletteCount = 0;
    i = 0;
    do {
      profile = SaveGetProfile();
      i++;
      if ((u8)(profile->data->bakuganBitsA[i / 8] & (1 << (i % 8))) != 0) {
        self->rouletteList[self->rouletteCount] = (u8)i;
        self->rouletteCount++;
      }
    } while (i < 20);
    self->onRandom = 0;
    self->roulettePick = 0xff;
    rtc = CoreRtcGetMicrosecond();
    self->rouletteSpinTarget = (u8)((s32)rtc % 2 + 10);
    self->rouletteStep = 1;
    break;

  case 1:
    tries = 0;
    if (self->rouletteCount == 1) {
      self->gridCursor = UiEquipMapBakuganIndex(self, 1, self->rouletteList[0]);
      self->rouletteSpinTarget = 1;
    } else {
      for (;;) {
        rtc = CoreRtcGetMicrosecond();
        self->roulettePick = (u8)((s32)rtc % (s32)self->rouletteCount);
        if (self->gridCursor !=
            UiEquipMapBakuganIndex(self, 1, self->rouletteList[self->roulettePick])) {
          self->gridCursor =
              UiEquipMapBakuganIndex(self, 1, self->rouletteList[self->roulettePick]);
          goto picked;
        }
        if (tries == 64) {
          break;
        }
        tries++;
      }
      for (i = 0; i < self->rouletteCount; i++) {
        self->roulettePick = (u8)i;
        if (self->gridCursor !=
            UiEquipMapBakuganIndex(self, 1, self->rouletteList[self->roulettePick])) {
          self->gridCursor =
              UiEquipMapBakuganIndex(self, 1, self->rouletteList[self->roulettePick]);
          break;
        }
      }
    }
  picked:
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
    UiEquipResetCursor(self);
    UiEquipFreeCurrentModel(self);
    self->rouletteStep++;
    break;

  case 2:
    UiEquipPreviewHoveredBakugan(self);
    self->rouletteWait = 2;
    self->rouletteStep++;
    break;

  case 3:
    UiEquipUpdateCursorPulse(self);
    UiEquipPulseGridCursor(self);
    sprites = (GfxSprite **)self->base.data;
    UiPulseStep(sprites[self->spriteCount], (UiPulse *)&self->tweens[self->spriteCount]);
    if (self->rouletteWait != 0) {
      self->rouletteWait--;
    } else {
      self->rouletteSpins++;
      if (self->rouletteSpins == self->rouletteSpinTarget) {
        self->rouletteStep = self->rouletteStep + 1;
      } else {
        self->rouletteStep = 1;
      }
    }
    break;

  case 4:
    self->rouletteStep = 5;
    break;

  default:
    return 1;
  }
  return 0;
}
