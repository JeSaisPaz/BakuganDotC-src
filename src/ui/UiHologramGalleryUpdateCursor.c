// bdc 0x0891d5bc UiHologramGalleryUpdateCursor
#include "bdc.h"

/* Re-places the selection cursor of the hologram gallery screen (`UiHologramGalleryCtor`,
   task 391) after a cursor move: resets the shared glow (`UiCursorGlowReset`) and `focusScale`,
   then by the active `panel`:
   - 0 (menu): cursor sprite 182 over window frame `180 + menuCursor` (menu items 0/1), or
     sprite 131 over list frame 128 (item 2), the other cursor hidden; window frames 180..181 and
     list frame 128 lit/unlit by `menuCursor`; sprites 183..186 and 130 reset to scale 1.
   - 1 (slot list): cursor 107 over slot frame `103 + slot` when `listMode` is 0, else cursor 131
     over list frame 128; slot frames 103..106 lit for the selected slot (only with `listMode` 0),
     the per-slot rows (sprites 108, 116, 112, 124, 90, 120, 8, four each) re-anchored at
     `slotRowOrigin - slotRowOffset[k]` / slot frame y minus the offset; list frame 128 lit when
     `listMode` is 1.
   - 2 (help list): cursor 140 over entry `134 + helpCursor`; the attribute icon cells (sprites
     61/62, `UiAttributeGetIconCell`), the page cells of sprites 102 and 133, the price
     (`UiHologramGallerySetPriceDigits`), picture (sprite 89) and HP gauge (sprite 78) of
     `g_hologramParams` `[helpOrder[helpCursor] * 3 + helpPage]`; entries 134..139, 141..146 and
     150..155 reset to scale 1.
   Any other panel does nothing more. The cursor gets scale 1, alpha 1, add colour 0.3 grey, the
   cursor's own z, the target's x/y, and restarts its pulse (`UiPulseReset`, `UiPulseInit`
   with ghost sprite 188). */

void UiHologramGalleryUpdateCursor(UiHologramGallery *self)
{
  UiHologramGalleryData *data;
  GfxSprite *sprite;
  GfxSprite *gauge;
  HologramParam *param;
  const u8 rowFirst[7] = {0x6c, 0x74, 0x70, 0x7c, 0x5a, 0x78, 0x08};
  s32 cursor;
  s32 target;
  s32 other;
  s32 i;
  s32 k;
  u32 cell;
  s32 iconCol;
  s32 iconRow;
  s32 price;
  s32 hpLevel;
  s8 panel;

  UiCursorGlowReset();
  panel = self->panel;
  self->focusScale = 0.0f;

  if (panel <= 0) {
    if (panel < 0) {
      return;
    }
    /* panel 0: menu */
    if (self->menuCursor < 2) {
      cursor = 0xb6;
      other = 0x83;
    } else {
      cursor = 0x83;
      other = 0xb6;
    }
    UiPulseReset((UiPulse *)&self->tweens[cursor]);
    data = (UiHologramGalleryData *)self->base.data;
    data->sprites[cursor]->flags |= 1;
    UiSpriteSetScaleRotation(data->sprites[cursor], 1.0f, 1.0f, 0.0f);
    sprite = data->sprites[cursor];
    sprite->alpha = 1.0f;
    sprite->addColor[0] = 0.3f;
    sprite->addColor[1] = 0.3f;
    sprite->addColor[2] = 0.3f;
    sprite->addColor[3] = 1.0f;
    sprite->posZ = self->spriteZ[cursor];
    target = (cursor == 0xb6) ? 0xb4 + self->menuCursor : 0x80;
    sprite->posX = data->sprites[target]->posX;
    sprite->posY = data->sprites[target]->posY;
    UiPulseInit(sprite, data->sprites[0xbc], (UiPulse *)&self->tweens[0xbc]);
    data->sprites[other]->flags &= ~1u;

    for (i = 0xb4; i < 0xb6; i++) {
      UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
      sprite = data->sprites[i];
      sprite->posZ = self->spriteZ[i];
      if (i - 0xb4 == self->menuCursor) {
        sprite->addColor[0] = 0.3f;
        sprite->addColor[1] = 0.3f;
        sprite->addColor[2] = 0.3f;
        sprite->addColor[3] = 1.0f;
        UiHologramGallerySetWindowFrameLit(self, data->sprites[i], true);
      } else {
        sprite->addColor[0] = 0.0f;
        sprite->addColor[1] = 0.0f;
        sprite->addColor[2] = 0.0f;
        sprite->addColor[3] = 1.0f;
        UiHologramGallerySetWindowFrameLit(self, data->sprites[i], false);
      }
    }
    for (i = 0xb7; i < 0xb9; i++) {
      UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
      data->sprites[i]->posZ = self->spriteZ[i];
    }
    for (i = 0xb9; i < 0xbb; i++) {
      UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
      data->sprites[i]->posZ = self->spriteZ[i];
    }

    UiSpriteSetScaleRotation(data->sprites[0x80], 1.0f, 1.0f, 0.0f);
    sprite = data->sprites[0x80];
    sprite->posZ = self->spriteZ[0x80];
    if (self->menuCursor == 2) {
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiHologramGallerySetItemFrameLit(self, data->sprites[0x80], true);
    } else {
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiHologramGallerySetItemFrameLit(self, data->sprites[0x80], false);
    }

    UiSpriteSetScaleRotation(data->sprites[0x82], 1.0f, 1.0f, 0.0f);
    data->sprites[0x82]->posZ = self->spriteZ[0x82];
    return;
  }

  if (panel < 2) {
    /* panel 1: slot list */
    if (self->listMode[0] == 0) {
      cursor = 0x6b;
      other = 0x83;
    } else {
      cursor = 0x83;
      other = 0x6b;
    }
    UiPulseReset((UiPulse *)&self->tweens[cursor]);
    data = (UiHologramGalleryData *)self->base.data;
    data->sprites[cursor]->flags |= 1;
    UiSpriteSetScaleRotation(data->sprites[cursor], 1.0f, 1.0f, 0.0f);
    sprite = data->sprites[cursor];
    sprite->alpha = 1.0f;
    sprite->addColor[0] = 0.3f;
    sprite->addColor[1] = 0.3f;
    sprite->addColor[2] = 0.3f;
    sprite->addColor[3] = 1.0f;
    sprite->posZ = self->spriteZ[cursor];
    target = (cursor == 0x6b) ? 0x67 + self->slot : 0x80;
    sprite->posX = data->sprites[target]->posX;
    sprite->posY = data->sprites[target]->posY;
    UiPulseInit(sprite, data->sprites[0xbc], (UiPulse *)&self->tweens[0xbc]);
    data->sprites[other]->flags &= ~1u;

    for (i = 0x67; i < 0x6b; i++) {
      UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
      sprite = data->sprites[i];
      sprite->posZ = self->spriteZ[i];
      if (i - 0x67 == self->slot && self->listMode[0] == 0) {
        sprite->addColor[0] = 0.3f;
        sprite->addColor[1] = 0.3f;
        sprite->addColor[2] = 0.3f;
        sprite->addColor[3] = 1.0f;
        UiHologramGallerySetSlotFrameLit(self, data->sprites[i], true);
      } else {
        sprite->addColor[0] = 0.0f;
        sprite->addColor[1] = 0.0f;
        sprite->addColor[2] = 0.0f;
        sprite->addColor[3] = 1.0f;
        UiHologramGallerySetSlotFrameLit(self, data->sprites[i], false);
      }
    }

    /* per-slot rows: row k's sprite j sits at the slot-row origin x / slot frame j y, minus
       slotRowOffset[k] */
    for (k = 0; k < 7; k++) {
      for (i = rowFirst[k]; i < rowFirst[k] + 4; i++) {
        u8 j = (u8)(i - rowFirst[k]);

        UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
        data->sprites[i]->posZ = self->spriteZ[i];
        data->sprites[i]->posX = self->slotRowOrigin[0] - self->slotRowOffset[k][0];
        data->sprites[i]->posY = data->sprites[0x67 + j]->posY - self->slotRowOffset[k][1];
      }
    }

    UiSpriteSetScaleRotation(data->sprites[0x80], 1.0f, 1.0f, 0.0f);
    sprite = data->sprites[0x80];
    sprite->posZ = self->spriteZ[0x80];
    if (self->listMode[0] == 1) {
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiHologramGallerySetItemFrameLit(self, data->sprites[0x80], true);
    } else {
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiHologramGallerySetItemFrameLit(self, data->sprites[0x80], false);
    }

    UiSpriteSetScaleRotation(data->sprites[0x81], 1.0f, 1.0f, 0.0f);
    data->sprites[0x81]->posZ = self->spriteZ[0x81];
    return;
  }

  if (panel >= 3) {
    return;
  }

  /* panel 2: help list */
  UiPulseReset((UiPulse *)&self->tweens[0x8c]);
  data = (UiHologramGalleryData *)self->base.data;
  data->sprites[0x8c]->flags |= 1;
  UiSpriteSetScaleRotation(data->sprites[0x8c], 1.0f, 1.0f, 0.0f);
  sprite = data->sprites[0x8c];
  sprite->alpha = 1.0f;
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = 0.3f;
  sprite->addColor[1] = 0.3f;
  sprite->addColor[2] = 0.3f;
  sprite->posZ = self->spriteZ[0x8c];
  sprite->posX = data->sprites[0x86 + self->helpCursor]->posX;
  sprite->posY = data->sprites[0x86 + self->helpCursor]->posY;
  UiPulseInit(sprite, data->sprites[0xbc], (UiPulse *)&self->tweens[0xbc]);

  cell = UiAttributeGetIconCell(self->helpOrder[self->helpCursor]);
  iconCol = (cell >> 8) & 0xff;
  iconRow = (cell >> 16) & 0xff;
  for (i = 0x3d; i < 0x3f; i++) {
    if (i == 0x3d) {
      GfxSpriteSetCell(data->sprites[i], (float)(iconCol / 3), (float)(iconCol % 3));
    } else {
      GfxSpriteSetCell(data->sprites[i], (float)(iconRow / 3), (float)(iconRow % 3));
    }
  }

  GfxSpriteSetCell(data->sprites[0x66], 0.0f, (float)self->helpPage);

  param = &g_hologramParams[self->helpOrder[self->helpCursor] * 3 + self->helpPage];
  price = param->price;
  hpLevel = param->hpLevel;
  UiHologramGallerySetPriceDigits(self, price, 0x61, data->sprites[0x66]->posX,
                                  data->sprites[0x66]->posY + 14.0f);

  UiHologramGallerySetHologramPicture(self, data->sprites[0x59], (u8)self->helpPage,
                                      self->helpOrder[self->helpCursor]);

  gauge = data->sprites[0x4e];
  UiHologramGallerySetGaugeWidth(self, gauge,
                                 (u8)UiHologramGalleryScaleLevel(self, (u8)hpLevel));

  GfxSpriteSetCell(data->sprites[0x85], 0.0f, (float)self->helpPage);

  for (i = 0x86; i < 0x8c; i++) {
    UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
    data->sprites[i]->posZ = self->spriteZ[i];
  }
  for (i = 0x8d; i < 0x93; i++) {
    UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
    data->sprites[i]->posZ = self->spriteZ[i];
  }
  for (i = 0x96; i < 0x9c; i++) {
    UiSpriteSetScaleRotation(data->sprites[i], 1.0f, 1.0f, 0.0f);
    data->sprites[i]->posZ = self->spriteZ[i];
  }
}
