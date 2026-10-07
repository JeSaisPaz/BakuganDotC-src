// bdc 0x089895dc UiCollectionTheaterResetCursor
#include "bdc.h"

/* Resets the cursor of the theater collection screen (task 315, `maybe_UiScreen315Ctor`;
   replays unlocked scenes: buttons `"collection_scene_%02d"` in `"waku_4_a"`/`"waku_4_b"` frames,
   thumbnails `"cinema_%02d"`): resets the cursor glow (`UiCursorGlowReset`), `zoom` and
   `cursorPulse`; shows the cursor sprite (data sprite 6) centred, unscaled, at full alpha with a
   30 % grey add colour, at its saved depth, on scene button `cursor`, and starts the pulse of its
   highlight copy (sprite 48, `ghostPulse`). The six scene buttons (sprites 0..5) are unscaled and
   put back to their saved depth; the selected one gets a 30 % grey add colour and the
   `"waku_4_a"` frame, the others none and `"waku_4_b"` (`UiCollectionTheaterSetCellFrame`).
   Sprites 31..36 are unscaled and put back to their saved depth, and the unlocked thumbnails of
   the page (sprites 13..18) get the current bob offset (as `UiCollectionTheaterUpdateThumbBob`).
   The bob offset is `(1 - cos(thumbBobT * pi)) * 0.5 * 4`. */

void UiCollectionTheaterResetCursor(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite *sprite;
  float c;
  int i;

  UiCursorGlowReset();
  self->zoom = 0.0f;
  UiPulseReset(&self->cursorPulse);
  sprite = ((GfxSprite **)self->base.data)[6];
  sprite->flags = sprite->flags | 1;
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[6]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6], 1.0f, 1.0f, 0.0f);
  ((GfxSprite **)self->base.data)[6]->alpha = 1.0f;
  sprite = ((GfxSprite **)self->base.data)[6];
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = 0.3f;
  sprite->addColor[1] = 0.3f;
  sprite->addColor[2] = 0.3f;
  ((GfxSprite **)self->base.data)[6]->posZ = self->spriteZ[6];
  ((GfxSprite **)self->base.data)[6]->posX =
      ((GfxSprite **)self->base.data)[self->cursor]->posX;
  ((GfxSprite **)self->base.data)[6]->posY =
      ((GfxSprite **)self->base.data)[self->cursor]->posY;
  UiPulseInit(((GfxSprite **)self->base.data)[6], ((GfxSprite **)self->base.data)[48],
              &self->ghostPulse);

  for (i = 0; i < 6; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    if (i == self->cursor) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiCollectionTheaterSetCellFrame(screen, ((GfxSprite **)self->base.data)[i], 1);
    } else {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiCollectionTheaterSetCellFrame(screen, ((GfxSprite **)self->base.data)[i], 0);
    }
  }

  for (i = 31; i < 37; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZTail[i - 19];
  }

  for (i = 0; i < 6; i++) {
    if (self->sceneId[i + self->page * 6] != 0xff) {
      c = __builtin_cosf(self->thumbBobT * 3.1415927f);
      ((GfxSprite **)self->base.data)[13 + i]->posY =
          self->thumbPos[i][1] - (1.0f - c) * 0.5f * 4.0f;
    }
  }
}
