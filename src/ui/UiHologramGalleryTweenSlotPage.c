// bdc 0x08925440 UiHologramGalleryTweenSlotPage
#include "bdc.h"

/* Starts the open (`hide == 0`) or close tweens of the slot-list page of the hologram gallery
   screen (steps 0xb/0xe of `UiHologramGalleryMainPhase`; polled by
   `UiHologramGallerySlotPageDone`). Every sprite of the page gets `UiTweenBegin` (scale 1,
   flags 1) on its own tween record `tweens[i]`. When opening it first lays the page out: sprites
   0x15, 0x1b, 0x18 (0x18 on cell (0,1)), 0x80 and 0x81 are shown (0x81 tinted grey while
   `UiHologramGalleryIsTutorialLocked`, white otherwise, alpha 0); then one row per slot of the
   area (`slotCount` rows, 26 px apart, centred on `slotRowOrigin[1]`): frame 0x67+slot
   (`UiHologramGallerySetSlotFrameLit` unlit, add-colour cleared), slot number 0x5a+slot (cell
   ((slot+1)/5, (slot+1)%5)), empty-slot marker 0x6c+slot (shown when nothing is placed), and,
   when the save profile has a hologram placed in the slot, its attribute icon 0x74+slot
   (`UiHologramGalleryMapAttribute` of `(id-14)/3`, attribute colour, alpha 0), its level
   0x70+slot (cell row `(id-14)%3`, UVs inset half a texel), sprite 0x78+slot and button icon 8+slot
   (`UiSetButtonIcon` 2). Row sprites are placed at `slotRowOrigin - slotRowOffset[k]`; sprites
   of rows past `slotCount` are hidden. */

#define SPRITE(i) (((GfxSprite **)self->base.data)[i])

void UiHologramGalleryTweenSlotPage(UiHologramGallery *self, u8 hide)
{
  int rowY;
  int i;
  u8 slot;
  u8 placed;
  u8 attr;
  GfxSprite *sprite;

  rowY = (int)(self->slotRowOrigin[1] - (float)(self->slotCount * 13 - 13));
  if (hide == 0) {
    for (i = 0x15; i < 0x16; i++) {
      SPRITE(i)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    for (i = 0x1b; i < 0x1c; i++) {
      SPRITE(i)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    for (i = 0x18; i < 0x19; i++) {
      SPRITE(i)->flags |= 1;
      GfxSpriteSetCell(SPRITE(i), 0.0f, 1.0f);
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    /* slot frames */
    for (i = 0x67; i < 0x6b; i++) {
      slot = (u8)(i - 0x67);
      sprite = SPRITE(i);
      if (slot < self->slotCount) {
        sprite->flags |= 1;
        sprite = SPRITE(i);
        sprite->addColor[0] = 0.0f;
        sprite->addColor[1] = 0.0f;
        sprite->addColor[2] = 0.0f;
        sprite->addColor[3] = 1.0f;
        UiHologramGallerySetSlotFrameLit(self, SPRITE(i), false);
      } else {
        sprite->flags &= ~1u;
      }
      SPRITE(i)->posY = (float)(rowY + slot * 26);
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    /* empty-slot markers */
    for (i = 0x6c; i < 0x70; i++) {
      slot = (u8)(i - 0x6c);
      if (slot < self->slotCount) {
        placed = SaveGetProfile()->data->placedHolograms[slot];
        if (placed != 0) {
          SPRITE(i)->flags &= ~1u;
        } else {
          SPRITE(i)->flags |= 1;
        }
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[0][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[0][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    /* attribute icons of the placed holograms */
    for (i = 0x74; i < 0x78; i++) {
      slot = (u8)(i - 0x74);
      if (slot < self->slotCount) {
        placed = SaveGetProfile()->data->placedHolograms[slot];
        if (placed != 0) {
          SPRITE(i)->flags |= 1;
          placed = SaveGetProfile()->data->placedHolograms[slot];
          attr = (u8)UiHologramGalleryMapAttribute(true, (u8)((placed - 14) / 3));
          GfxSpriteSetCell(SPRITE(i), 0.0f, (float)attr);
          UiHologramGallerySetAttributeColor(self, SPRITE(i), attr);
          SPRITE(i)->alpha = 0.0f;
        } else {
          SPRITE(i)->flags &= ~1u;
        }
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[1][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[1][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    /* levels of the placed holograms */
    for (i = 0x70; i < 0x74; i++) {
      slot = (u8)(i - 0x70);
      if (slot < self->slotCount) {
        placed = SaveGetProfile()->data->placedHolograms[slot];
        if (placed != 0) {
          SPRITE(i)->flags |= 1;
          placed = SaveGetProfile()->data->placedHolograms[slot];
          GfxSpriteSetCell(SPRITE(i), 0.0f, (float)(u8)((placed - 14) % 3));
          GfxSpriteInsetUv(0.5f, SPRITE(i));
        } else {
          SPRITE(i)->flags &= ~1u;
        }
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[2][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[2][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    for (i = 0x7c; i < 0x80; i++) {
      slot = (u8)(i - 0x7c);
      if (slot < self->slotCount) {
        SPRITE(i)->flags |= 1;
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[3][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[3][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    /* slot numbers */
    for (i = 0x5a; i < 0x5e; i++) {
      slot = (u8)(i - 0x5a);
      if (slot < self->slotCount) {
        SPRITE(i)->flags |= 1;
        GfxSpriteSetCell(SPRITE(i), (float)((slot + 1) / 5), (float)((slot + 1) % 5));
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[4][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[4][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    for (i = 0x78; i < 0x7c; i++) {
      slot = (u8)(i - 0x78);
      if (slot < self->slotCount) {
        placed = SaveGetProfile()->data->placedHolograms[slot];
        if (placed != 0) {
          SPRITE(i)->flags |= 1;
        } else {
          SPRITE(i)->flags &= ~1u;
        }
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[5][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[5][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    /* button icons of the occupied slots */
    for (i = 8; i < 0xc; i++) {
      slot = (u8)(i - 8);
      if (slot < self->slotCount) {
        placed = SaveGetProfile()->data->placedHolograms[slot];
        if (placed != 0) {
          SPRITE(i)->flags |= 1;
          UiSetButtonIcon(SPRITE(i), 2);
        } else {
          SPRITE(i)->flags &= ~1u;
        }
      } else {
        SPRITE(i)->flags &= ~1u;
      }
      SPRITE(i)->posX = self->slotRowOrigin[0] - self->slotRowOffset[6][0];
      SPRITE(i)->posY = (float)(rowY + slot * 26) - self->slotRowOffset[6][1];
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    for (i = 0x80; i < 0x81; i++) {
      SPRITE(i)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
    for (i = 0x81; i < 0x82; i++) {
      if (UiHologramGalleryIsTutorialLocked() == 1) {
        sprite = SPRITE(i);
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
      } else {
        sprite = SPRITE(i);
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.0f;
      }
      SPRITE(i)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    }
  } else {
    for (i = 0x15; i < 0x16; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x1b; i < 0x1c; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x18; i < 0x19; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x67; i < 0x6b; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x6c; i < 0x70; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x74; i < 0x78; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x70; i < 0x74; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x7c; i < 0x80; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x5a; i < 0x5e; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x78; i < 0x7c; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 8; i < 0xc; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x80; i < 0x81; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
    for (i = 0x81; i < 0x82; i++)
      UiTweenBegin(1.0f, hide, SPRITE(i), &self->tweens[i], 1);
  }
}

#undef SPRITE
