// bdc 0x089aa618 UiMainMenuStartCarouselMove
#include "bdc.h"

/* Starts the carousel rotation after a cursor move: flashes the arrow of the move direction
   (layout sprite 3 + moveDir, `UiFlashStart`), hides the old item's label
   (`UiMainMenuShowItemLabel`), slides the old item's sprite (layout sprite 5+item) off-screen
   (to x 704 when moveDir is 0, else -224) and the new one in from the other side to `itemHomeX`
   (tween slots 5+item), sets the base-model and item-model rotation start/target angles (steps of
   1.256 rad from 1.57, `UiMainMenuItemDistance`, `UiWrapAngle`), and switches the light mode
   on when the new item is 4 (item box) or off when the old one was (`UiMainMenuSetLightMode`). */

void UiMainMenuStartCarouselMove(UiMainMenu *self)
{
  GfxSprite **sprites;
  UiTween *tween;
  u8 moveDir;
  s8 prev;
  s8 cursor;
  float base;
  int i;

  UiFlashStart(2.0f, ((GfxSprite **)self->base.data)[3 + self->moveDir], 0, 0);
  self->slots[1].tween.toggle07 = 0;
  UiMainMenuShowItemLabel(self, 0, (u8)self->prevCursor);

  prev = self->prevCursor;
  sprites = (GfxSprite **)self->base.data;
  tween = &self->slots[5 + prev].tween;
  tween->slideStart = (s16)sprites[5 + prev]->posX;
  moveDir = self->moveDir;
  tween->slideEnd = (moveDir == 0) ? 0x2c0 : -0xe0;
  tween->slideDelta = (s16)UiAbsDiff(sprites[5 + prev]->posX, (float)tween->slideEnd);

  if (moveDir == 0) {
    sprites[5 + self->cursor]->posX = -224.0f;
  } else {
    sprites[5 + self->cursor]->posX = 704.0f;
  }
  sprites = (GfxSprite **)self->base.data;
  cursor = self->cursor;
  moveDir = self->moveDir;
  tween = &self->slots[5 + cursor].tween;
  base = self->baseAngle;
  tween->slideEnd = (s16)self->itemHomeX;
  tween->slideStart = (s16)sprites[5 + cursor]->posX;
  tween->slideDelta = (s16)UiAbsDiff(sprites[5 + cursor]->posX, (float)tween->slideEnd);
  self->baseStartAngle = base;
  if (moveDir == 0) {
    base = UiWrapAngle(base + 1.256f);
  } else {
    base = UiWrapAngle(base - 1.256f);
  }
  self->baseTargetAngle = base;

  for (i = 0; i < 5; i++) {
    if (self->models[i] != NULL) {
      base = UiWrapAngle((float)self->items[i].from.slot * 1.256f + 1.57f);
      self->items[i].angle = base;
      self->items[i].from.startAngle = base;
      self->items[i].from.slot = UiMainMenuItemDistance(self, (u8)i, (u8)cursor);
      self->items[i].targetAngle = UiWrapAngle((float)self->items[i].from.slot * 1.256f + 1.57f);
      cursor = self->cursor;
    }
  }

  if (cursor == 4) {
    UiMainMenuSetLightMode(self, 1);
  } else if (self->prevCursor == 4) {
    UiMainMenuSetLightMode(self, 0);
  }
}
