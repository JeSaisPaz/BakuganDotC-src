// bdc 0x0899a3d8 UiWorldMapPlaceAreaLabel
#include "bdc.h"

/* Keeps the two label sprites of area button `area` of `UiWorldMap` (sprites
   `area + 0x10` and `area + 0x2a`) attached to the button: copies its scale (angle 0) and places
   them at the button position minus `labelAOffsetY` / `labelBOffsetX,labelBOffsetY` scaled by the
   button's `scaleY`. */

void UiWorldMapPlaceAreaLabel(UiScreen *screen, u8 area)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites;
  GfxSprite *button;

  sprites = (GfxSprite **)screen->data;
  button = sprites[area];
  UiSpriteSetScaleRotation(sprites[area + 0x10], button->scaleX, button->scaleY, 0.0f);
  sprites = (GfxSprite **)screen->data;
  sprites[area + 0x10]->posX = sprites[area]->posX;
  sprites = (GfxSprite **)screen->data;
  button = sprites[area];
  sprites[area + 0x10]->posY = button->posY - button->scaleY * map->labelAOffsetY;

  sprites = (GfxSprite **)screen->data;
  button = sprites[area];
  UiSpriteSetScaleRotation(sprites[area + 0x2a], button->scaleX, button->scaleY, 0.0f);
  sprites = (GfxSprite **)screen->data;
  button = sprites[area];
  sprites[area + 0x2a]->posX = button->posX - button->scaleY * map->labelBOffsetX;
  sprites = (GfxSprite **)screen->data;
  button = sprites[area];
  sprites[area + 0x2a]->posY = button->posY - button->scaleY * map->labelBOffsetY;
}
