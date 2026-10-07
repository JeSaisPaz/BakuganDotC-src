// bdc 0x0899b188 UiWorldMapAreaButtonsDone
#include "bdc.h"

/* Advances the slide tweens of the area buttons 0..7 and 8..15 of `UiWorldMap`:
   a tween whose start delay (`spriteTween[i].delay0b`) is nonzero only counts it down, otherwise
   `UiWorldMapSlideTweenStep` runs and a finished step is counted (u8 counter). Then copies the
   alpha of sprites 0..7 to their decoration sprites 16..23 and 42..49. Returns true when the count
   is exactly 16. */

bool UiWorldMapAreaButtonsDone(UiScreen *screen, u8 out)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites;
  u8 finished = 0;
  int i;

  for (i = 0; i < 8; i++) {
    if (map->spriteTween[i].delay0b != 0) {
      map->spriteTween[i].delay0b--;
    } else {
      finished += UiWorldMapSlideTweenStep(screen, out, i);
    }
  }
  for (i = 8; i < 16; i++) {
    if (map->spriteTween[i].delay0b != 0) {
      map->spriteTween[i].delay0b--;
    } else {
      finished += UiWorldMapSlideTweenStep(screen, out, i);
    }
  }
  for (i = 0; i < 8; i++) {
    sprites = (GfxSprite **)screen->data;
    sprites[i + 0x10]->alpha = sprites[i]->alpha;
  }
  for (i = 0; i < 8; i++) {
    sprites = (GfxSprite **)screen->data;
    sprites[i + 0x2a]->alpha = sprites[i]->alpha;
  }
  return finished == 0x10;
}
