// bdc 0x0896e9e8 UiCardEquipUpdatePointerArrows
#include "bdc.h"

/* Animates the pointer arrows (group 20) of `UiCardEquip` while enabled
   (`arrowsOn`; otherwise returns at once). The tab cursor picks an arrow colour (index into
   `g_cardEquipArrowTints`, 0xff = none). In state 0 it hides the whole group, then shows the arrow
   of the current row (slot = row 0..2; row 0 needs a colour) tinted by its colour with alpha 0.4
   (row 0) or 0.8 (rows 1/2) and enters state 1..3. In states 1..3 the arrow alpha pulses down by
   up to 0.4/0.8 following (1 - cos(t*pi)) / 2, t += 1/30 per frame, until the row (state 1:
   also the tab cursor) changes, which returns to state 0. Finally latches the row and tab cursor
   for the next frame. */

void UiCardEquipUpdatePointerArrows(UiCardEquip *self)
{
  float tints[4][3];
  GfxSprite **sprites;
  GfxSprite *sprite;
  int colour;
  int stride;
  int tab;
  int row;
  int i;
  float base;
  float t;

  colour = 0;
  memcpy(tints, g_cardEquipArrowTints, 0x30);
  if (self->arrowsOn == 0) {
    return;
  }
  tab = self->rowCursor[0];
  if (self->bakuganCount < 3) {
    stride = 2;
    if (tab > 0) {
      if (tab < 3) {
        colour = 0xff;
      } else if (tab < 4) {
        colour = 1;
      }
    }
  } else {
    stride = 4;
    if ((u32)tab < 6) {
      switch (tab) {
      case 1:
        colour = 1;
        break;
      case 2:
      case 3:
        colour = 0xff;
        break;
      case 4:
        colour = 2;
        break;
      case 5:
        colour = 3;
        break;
      default:
        colour = 0;
        break;
      }
    }
  }

  if (self->arrowState == 0) {
    for (i = self->groups[0x14][0]; i < self->groups[0x14][0] + (s8)self->groups[0x14][1]; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags &= ~1u;
    }
    row = self->row;
    if (row > 0) {
      if (row < 2) {
        self->arrowSlot = 1;
        sprites = (GfxSprite **)self->base.data;
        sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->flags |= 1;
        sprites = (GfxSprite **)self->base.data;
        sprite = sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour];
        sprite->tint[0] = tints[colour][0];
        sprite->tint[1] = tints[colour][1];
        sprite->tint[2] = tints[colour][2];
        sprite->alpha = 0.8f;
        sprites = (GfxSprite **)self->base.data;
        base = sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->alpha;
        self->arrowT = 0.0f;
        self->arrowBaseAlpha = base;
        self->arrowState = 2;
      } else if (row < 3) {
        self->arrowSlot = 2;
        sprites = (GfxSprite **)self->base.data;
        sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->flags |= 1;
        sprites = (GfxSprite **)self->base.data;
        sprite = sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour];
        sprite->tint[0] = tints[colour][0];
        sprite->tint[1] = tints[colour][1];
        sprite->tint[2] = tints[colour][2];
        sprite->alpha = 0.8f;
        sprites = (GfxSprite **)self->base.data;
        base = sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->alpha;
        self->arrowT = 0.0f;
        self->arrowBaseAlpha = base;
        self->arrowState = 3;
      }
    } else if (row == 0 && colour != 0xff) {
      self->arrowSlot = 0;
      sprites = (GfxSprite **)self->base.data;
      sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->flags |= 1;
      sprites = (GfxSprite **)self->base.data;
      sprite = sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour];
      sprite->tint[0] = tints[colour][0];
      sprite->tint[1] = tints[colour][1];
      sprite->tint[2] = tints[colour][2];
      sprite->alpha = 0.4f;
      sprites = (GfxSprite **)self->base.data;
      base = sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->alpha;
      self->arrowT = 0.0f;
      self->arrowBaseAlpha = base;
      self->arrowState = 1;
    }
  } else if (self->arrowState < 2) {
    if (self->arrowRow == self->row && self->rowCursor[2] == tab) {
      t = self->arrowT + 0.033333335f;
      self->arrowT = t;
      base = self->arrowBaseAlpha;
      base = base - (1.0f - __builtin_cosf(t * 3.1415927f)) * 0.5f * 0.4f;
      sprites = (GfxSprite **)self->base.data;
      sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->alpha = base;
    } else {
      self->arrowState = 0;
    }
  } else if (self->arrowState < 4) {
    if (self->arrowRow == self->row) {
      t = self->arrowT + 0.033333335f;
      self->arrowT = t;
      base = self->arrowBaseAlpha;
      base = base - (1.0f - __builtin_cosf(t * 3.1415927f)) * 0.5f * 0.8f;
      sprites = (GfxSprite **)self->base.data;
      sprites[self->groups[0x14][0] + stride * self->arrowSlot + colour]->alpha = base;
    } else {
      self->arrowState = 0;
    }
  }
  self->arrowRow = self->row;
  self->rowCursor[2] = self->rowCursor[0];
}
