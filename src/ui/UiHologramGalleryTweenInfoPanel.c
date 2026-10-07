// bdc 0x0892279c UiHologramGalleryTweenInfoPanel
#include "bdc.h"

/* Starts the open/close slide tweens of the information panel of the hologram gallery screen
   (sprites listed by `UiHologramGalleryInfoPanelSprite`, terminated by 0xff). Each sprite gets a
   `UiTweenBeginSlide` (scale 1, flags 9) into `self->tweens[id]`.
   Hiding (`hide != 0`) just slides every listed sprite from 0 to -32 with fade-out.
   Showing (`hide == 0`) first refreshes the panel contents per sprite id, then makes the sprite
   visible (`flags |= 1`), sets its `layerMask` to `1 << ` byte 2 of the list entry and slides it
   from -32 to 0:
   - 0x35: attribute row of the default Bakugan, mirrored horizontally;
   - 0x36 / 0x37: attribute cell / full name image of the default Bakugan, placed relative to
     sprite 0x35 at (`nameLabelBDx`, `nameLabelBDy` + 32) / (`nameLabelADx`, `nameLabelADy` + 32);
   - 0x3f / 0x40: attribute icon cell (column / row byte of `UiAttributeGetIconCell`), laid out
     3 per row;
   - 0x84: brawler portrait cell of the current stage (`g_scriptGlobalVars[1]`);
   - 0xb3: brawler picture; 0xbb: field area picture;
   - 0x82 / 0xb7, 0xb9 / 0xb8, 0xba: dimmed (tint 0.5, alpha 0) when `disabledItems` bit 2 / 0 / 1
     is set. */

void UiHologramGalleryTweenInfoPanel(UiHologramGallery *self, u8 hide)
{
  GfxSprite *sprite;
  GfxSprite *panel;
  u32 cell;
  u32 entry;
  u32 id;
  u32 i;
  s32 iconCol;
  s32 iconRow;
  s32 brawler;
  int dim;

  if (hide != 0) {
    i = 0;
    for (;;) {
      entry = UiHologramGalleryInfoPanelSprite(self, i & 0xff);
      id = entry & 0xffff;
      if (id == 0xff) {
        break;
      }
      UiTweenBeginSlide(1.0f, 0.0f, -32.0f, hide, ((GfxSprite **)self->base.data)[id],
                        &self->tweens[id], 9);
      i++;
    }
    return;
  }

  cell = UiAttributeGetIconCell(self->attribute);
  iconCol = (cell >> 8) & 0xff;
  iconRow = (cell >> 16) & 0xff;
  i = 0;
  for (;;) {
    entry = UiHologramGalleryInfoPanelSprite(self, i & 0xff);
    id = entry & 0xffff;
    if (id == 0xff) {
      break;
    }
    sprite = ((GfxSprite **)self->base.data)[id];
    dim = 0;
    switch (id) {
    case 0x35:
      UiBakuganSetAttributeRow(sprite, self->bakugan);
      GfxSpriteFlipU(((GfxSprite **)self->base.data)[id]);
      break;
    case 0x36:
      UiBakuganSetAttributeCell(sprite, self->bakugan);
      panel = ((GfxSprite **)self->base.data)[0x35];
      ((GfxSprite **)self->base.data)[id]->posX = panel->posX + self->nameLabelBDx;
      panel = ((GfxSprite **)self->base.data)[0x35];
      ((GfxSprite **)self->base.data)[id]->posY =
          (panel->posY + self->nameLabelBDy) - (-32.0f);
      break;
    case 0x37:
      UiBakuganSetFullNameTexture(sprite, self->bakugan);
      panel = ((GfxSprite **)self->base.data)[0x35];
      ((GfxSprite **)self->base.data)[id]->posX = panel->posX + self->nameLabelADx;
      panel = ((GfxSprite **)self->base.data)[0x35];
      ((GfxSprite **)self->base.data)[id]->posY =
          (panel->posY + self->nameLabelADy) - (-32.0f);
      break;
    case 0x3f:
      GfxSpriteSetCell(sprite, (float)(iconCol / 3), (float)(iconCol % 3));
      break;
    case 0x40:
      GfxSpriteSetCell(sprite, (float)(iconRow / 3), (float)(iconRow % 3));
      break;
    case 0x82:
      dim = (self->disabledItems & 4) != 0;
      break;
    case 0x84:
      brawler = (s32)UiHologramGalleryAreaBrawler(self, g_scriptGlobalVars[1] & 0xff);
      GfxSpriteSetCell(sprite, 0.0f, (float)brawler);
      break;
    case 0xb3:
      UiHologramGallerySetBrawlerPicture(self, sprite, self->bakugan);
      break;
    case 0xb7:
    case 0xb9:
      dim = (self->disabledItems & 1) != 0;
      break;
    case 0xb8:
    case 0xba:
      dim = (self->disabledItems & 2) != 0;
      break;
    case 0xbb:
      UiHologramGallerySetAreaPicture(self, sprite, self->fieldId, self->areaId);
      break;
    default:
      break;
    }
    if (dim) {
      sprite->tint[0] = 0.5f;
      sprite->tint[1] = 0.5f;
      sprite->tint[2] = 0.5f;
      sprite->alpha = 0.0f;
    }
    /* every case that called out or dimmed re-reads the sprite pointer */
    sprite = ((GfxSprite **)self->base.data)[id];
    sprite->flags |= 1;
    ((GfxSprite **)self->base.data)[id]->layerMask = 1u << (((entry >> 16) & 0xff) & 0x1f); /* sllv: low 5 bits */
    UiTweenBeginSlide(1.0f, -32.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[id],
                      &self->tweens[id], 9);
    i++;
  }
}
