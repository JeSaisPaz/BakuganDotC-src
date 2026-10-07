// bdc 0x0899efe4 UiWorldMapStartAreaButtonsTween
#include "bdc.h"

/* Starts the staggered slide-in/out of the area list of `UiWorldMap`: for the
   buttons 0..7 and labels 8..0xf it sets `spriteTween[i]` to a 64 px horizontal slide
   (`slideStart`/`slideEnd`/`slideDelta` via `UiAbsDiff`) with start delays 0, 2, 4… frames (run by
   `UiWorldMapAreaButtonsDone`); every sprite of the list (also the markers 0x10..0x17 and
   0x2a..0x31) gets a `UiTweenBegin` (scale 1.5 coming in, 1.0 going out). Coming in (`out` = 0)
   the sprites start 64 px to the right and slide back to their place; it also shows only selectable
   areas (`selectableMask`), sets each label's cell to `areaLabelCell[i]`, greys out (tint 0.5,
   alpha 0) locked areas (`unlockMask`, also cell 8) and blocked ones (`blockedMask`), and decides
   the "new" and rank-mode markers like `UiWorldMapStartAreaListFade`. Going out the slide runs
   from the current position to 64 px to the right. */

void UiWorldMapStartAreaButtonsTween(UiScreen *screen, u8 out)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *sprite;
  UiTween *tween;
  int i;

  if (out == 0) {
    for (i = 0; i < 8; i++) {
      tween = &map->spriteTween[i];
      sprite = ((GfxSprite **)screen->data)[i];
      if ((map->selectableMask & (1 << i)) != 0) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      UiTweenBegin(1.5f, out, ((GfxSprite **)screen->data)[i], tween, 3);
      sprite = ((GfxSprite **)screen->data)[i];
      tween->slideEnd = (s16)sprite->posX;
      sprite->posX = sprite->posX + 64.0f;
      tween->slideStart = (s16)((GfxSprite **)screen->data)[i]->posX;
      tween->slideDelta = (s16)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
      tween->delay0b = (u8)(i * 2);
    }
    for (i = 8; i < 0x10; i++) {
      int bit;

      tween = &map->spriteTween[i];
      GfxSpriteSetCell(((GfxSprite **)screen->data)[i], 0.0f, (float)map->areaLabelCell[i - 8]);
      bit = 1 << (i - 8);
      sprite = ((GfxSprite **)screen->data)[i];
      if ((map->selectableMask & bit) != 0) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      UiTweenBegin(1.5f, out, ((GfxSprite **)screen->data)[i], tween, 3);
      sprite = ((GfxSprite **)screen->data)[i];
      tween->slideEnd = (s16)sprite->posX;
      sprite->posX = sprite->posX + 64.0f;
      tween->slideStart = (s16)((GfxSprite **)screen->data)[i]->posX;
      tween->slideDelta = (s16)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
      tween->delay0b = (u8)((i - 8) * 2);
      if ((map->unlockMask & bit) == 0) {
        sprite = ((GfxSprite **)screen->data)[i];
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
        GfxSpriteSetCell(((GfxSprite **)screen->data)[i], 0.0f, 8.0f);
      }
      if ((map->blockedMask & bit) != 0) {
        sprite = ((GfxSprite **)screen->data)[i];
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
      }
    }
    for (i = 0x10; i < 0x18; i++) {
      if ((map->unlockMask & (1 << (i - 0x10))) == 0) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      } else if (UiWorldMapIsRankMode(screen)) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      } else {
        SaveProfile *profile = SaveGetProfile();
        int group = map->areaGroup[i - 0x10];

        sprite = ((GfxSprite **)screen->data)[i];
        if ((u8)(profile->data->areaVisited[group / 8] & (1 << (group % 8))) == 0) {
          sprite->flags |= 1;
        } else {
          sprite->flags &= ~1u;
        }
      }
      UiTweenBegin(1.5f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 3);
    }
    for (i = 0x2a; i < 0x32; i++) {
      int bit = 1 << (i - 0x2a);

      if ((map->unlockMask & bit) == 0) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      } else {
        bool rankMode = UiWorldMapIsRankMode(screen);

        sprite = ((GfxSprite **)screen->data)[i];
        if (!rankMode) {
          sprite->flags &= ~1u;
        } else if (map->newRankFlag[map->areaGroup[i - 0x2a]] == 0) {
          sprite->flags &= ~1u;
        } else if ((map->selectableMask & bit) == 0) {
          sprite->flags &= ~1u;
        } else {
          sprite->flags |= 1;
        }
      }
      UiTweenBegin(1.5f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 3);
    }
  } else {
    for (i = 0; i < 8; i++) {
      tween = &map->spriteTween[i];
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], tween, 3);
      sprite = ((GfxSprite **)screen->data)[i];
      tween->slideEnd = (s16)(sprite->posX + 64.0f);
      tween->slideStart = (s16)sprite->posX;
      tween->slideDelta = (s16)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
      tween->delay0b = (u8)(i * 2);
    }
    for (i = 8; i < 0x10; i++) {
      tween = &map->spriteTween[i];
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], tween, 3);
      sprite = ((GfxSprite **)screen->data)[i];
      tween->slideEnd = (s16)(sprite->posX + 64.0f);
      tween->slideStart = (s16)sprite->posX;
      tween->slideDelta = (s16)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
      tween->delay0b = (u8)((i - 8) * 2);
    }
    for (i = 0x10; i < 0x18; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 3);
    }
    for (i = 0x2a; i < 0x32; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 3);
    }
  }
}
