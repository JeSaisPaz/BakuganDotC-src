// bdc 0x08965a8c UiEquipShowHelpPopup
#include "bdc.h"

/* Runs the help pop-up of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`), stepped by `popupStep`, on the current player's tab sprite
   (`spriteIdx[0x13] + editPlayer`):
   0: brings the tab to the front (flag bit 0, layer mask 0x10, posZ -500), starts its grow tween,
      prints the name and help text of the panel card under the cursor
      (`UiEquipPrintCardName`, `UiEquipSetCardHelpText`) with both texts at alpha 0;
   1: grows the tab 1.5 -> 1.0 over 8 frames while fading the texts in (`popupFade + 1 - (t-1)^2`);
   2: waits while pad button 0x8000 is held;
   3: starts the shrink tween (`popupT` 0, `popupFade` 1);
   4: shrinks the tab 1.0 -> 1.5 over 8 frames while fading the texts out (`popupFade - t^2`);
   past 4: clears both text printers and returns 1. Returns 0 otherwise. */

s32 UiEquipShowHelpPopup(UiEquip *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  UiTextPrinter *printer;
  s32 idx;
  float t;
  float alpha;

  switch (self->popupStep) {
  case 0:
    idx = self->spriteIdx[0x13] + self->editPlayer;
    sprites[idx]->flags |= 1;
    sprites[idx]->layerMask = 0x10;
    sprites[idx]->posZ = -500.0f;
    UiTweenBegin(1.5f, 0, sprites[idx], &self->tweens[idx], 3);
    UiEquipPrintCardName(self, self->panelCards[self->editPlayer * 4 + self->panelEntry]);
    UiEquipSetCardHelpText(self, self->panelCards[self->editPlayer * 4 + self->panelEntry]);
    self->nameDirty = 1;
    self->helpDirty = 1;
    self->nameAlpha = 0.0f;
    self->helpAlpha = 0.0f;
    self->popupStep++;
    break;

  case 1:
    idx = self->spriteIdx[0x13] + self->editPlayer;
    UiTweenUpdate(1.5f, 1.0f, 8.0f, 0, sprites[idx], &self->tweens[idx], 3);
    t = self->popupT + 0.125f;
    self->popupT = t;
    self->nameDirty = 1;
    self->helpDirty = 1;
    alpha = self->popupFade + (1.0f - (t - 1.0f) * (t - 1.0f));
    self->nameAlpha = alpha;
    self->helpAlpha = alpha;
    if (!(t < 1.0f)) {
      self->popupStep++;
    }
    break;

  case 2:
    if ((self->base.pad->buttons & 0x8000) == 0) {
      self->popupStep = 3;
    }
    break;

  case 3:
    idx = self->spriteIdx[0x13] + self->editPlayer;
    UiTweenBegin(1.5f, 1, sprites[idx], &self->tweens[idx], 3);
    self->popupT = 0.0f;
    self->popupFade = 1.0f;
    self->popupStep++;
    break;

  case 4:
    idx = self->spriteIdx[0x13] + self->editPlayer;
    UiTweenUpdate(1.0f, 1.5f, 8.0f, 1, sprites[idx], &self->tweens[idx], 3);
    t = self->popupT + 0.125f;
    self->nameDirty = 1;
    self->helpDirty = 1;
    self->popupT = t;
    alpha = self->popupFade - t * t;
    self->nameAlpha = alpha;
    self->helpAlpha = alpha;
    if (!(t < 1.0f)) {
      self->popupStep++;
    }
    break;

  default:
    printer = self->namePrinter;
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    printer = self->helpPrinter;
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    return 1;
  }
  return 0;
}
