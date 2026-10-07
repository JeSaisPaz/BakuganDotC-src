// bdc 0x0899b2d0 UiWorldMapStartPreviewTween
#include "bdc.h"

/* Starts the slide tween (64 px, `UiTweenBeginSlide`, flags 7) of the area preview of
   `UiWorldMap`: flag sprite 0x1b (data `+0x6c`), stage picture 0x1a (`+0x68`) and
   the two frame sprites 0x20/0x21 (`+0x80`, `+0x84`), each with its `spriteTween[slot]`. Coming in
   (`out` = 0, scale 1.5, from -64 to 0) first sets up their contents for the selected area
   `areaId`: each is shown only if the area's bit in `unlockMask` is set; the flag also needs
   `areaHasFlag[area]` (`UiWorldMapSetFlag`), the picture is set via `UiWorldMapSetStageImage`
   for `stage`; frame 0x20 gets tint (0, 0, 0) and frame 0x21 tint (0.5, 1, 0), both alpha 0.
   Going out (scale 1, from 0 to -64) only starts the four tweens. */

static GfxSprite *Spr(UiScreen *screen, int slot)
{
  return ((GfxSprite **)screen->data)[slot];
}

void UiWorldMapStartPreviewTween(UiScreen *screen, u8 out)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *sprite;
  s8 area;

  sprite = Spr(screen, 0x1b);
  if (out == 0) {
    area = map->areaId;
    if ((map->unlockMask & (1 << area)) == 0) {
      sprite->flags &= ~1u;
    } else if (map->areaHasFlag[area] == 0) {
      sprite->flags &= ~1u;
    } else {
      UiWorldMapSetFlag(screen, sprite, (u8)area);
      Spr(screen, 0x1b)->flags |= 1;
    }
    UiTweenBeginSlide(1.5f, -64.0f, 0.0f, out, Spr(screen, 0x1b), &map->spriteTween[0x1b], 7);

    area = map->areaId;
    sprite = Spr(screen, 0x1a);
    if ((map->unlockMask & (1 << area)) == 0) {
      sprite->flags &= ~1u;
    } else {
      UiWorldMapSetStageImage(screen, sprite, (u8)area, (u8)map->stage);
      Spr(screen, 0x1a)->flags |= 1;
    }
    UiTweenBeginSlide(1.5f, -64.0f, 0.0f, out, Spr(screen, 0x1a), &map->spriteTween[0x1a], 7);

    sprite = Spr(screen, 0x20);
    if ((map->unlockMask & (1 << map->areaId)) == 0) {
      sprite->flags &= ~1u;
    } else {
      sprite->flags |= 1;
    }
    sprite = Spr(screen, 0x20);
    sprite->tint[0] = 0.0f;
    sprite->tint[1] = 0.0f;
    sprite->tint[2] = 0.0f;
    sprite->alpha = 0.0f;
    UiTweenBeginSlide(1.5f, -64.0f, 0.0f, out, Spr(screen, 0x20), &map->spriteTween[0x20], 7);

    sprite = Spr(screen, 0x21);
    if ((map->unlockMask & (1 << map->areaId)) == 0) {
      sprite->flags &= ~1u;
    } else {
      sprite->flags |= 1;
    }
    sprite = Spr(screen, 0x21);
    sprite->tint[1] = 1.0f;
    sprite->tint[0] = 0.5f;
    sprite->tint[2] = 0.0f;
    sprite->alpha = 0.0f;
    UiTweenBeginSlide(1.5f, -64.0f, 0.0f, out, Spr(screen, 0x21), &map->spriteTween[0x21], 7);
  } else {
    UiTweenBeginSlide(1.0f, 0.0f, -64.0f, out, sprite, &map->spriteTween[0x1b], 7);
    UiTweenBeginSlide(1.0f, 0.0f, -64.0f, out, Spr(screen, 0x1a), &map->spriteTween[0x1a], 7);
    UiTweenBeginSlide(1.0f, 0.0f, -64.0f, out, Spr(screen, 0x20), &map->spriteTween[0x20], 7);
    UiTweenBeginSlide(1.0f, 0.0f, -64.0f, out, Spr(screen, 0x21), &map->spriteTween[0x21], 7);
  }
}
