// bdc 0x0899c840 UiWorldMapStartAreaListFade
#include "bdc.h"

/* Starts the fade tweens of the area-selection sprites of `UiWorldMap` (checked by
   `UiWorldMapAreaListFadeDone`). Coming back in (`out` = 0) also decides visibility: buttons 0..7
   and labels 8..0xf for selectable areas (`selectableMask`); "new" markers 0x10..0x17 for unlocked
   areas (`unlockMask`) not yet visited (profile `areaVisited` bits, outside rank mode only); and
   rank-mode markers 0x2a..0x31 for unlocked, selectable areas whose stages are all cleared but
   unranked (`newRankFlag`). */

void UiWorldMapStartAreaListFade(UiScreen *screen, u8 out)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites;
  int i;

  if (out == 0) {
    for (i = 0; i < 8; i++) {
      sprites = (GfxSprite **)screen->data;
      if ((map->selectableMask & (1 << i)) != 0) {
        sprites[i]->flags |= 1;
      } else {
        sprites[i]->flags &= ~1u;
      }
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
    for (i = 8; i < 0x10; i++) {
      sprites = (GfxSprite **)screen->data;
      if ((map->selectableMask & (1 << (i - 8))) != 0) {
        sprites[i]->flags |= 1;
      } else {
        sprites[i]->flags &= ~1u;
      }
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
    for (i = 0x10; i < 0x18; i++) {
      if ((map->unlockMask & (1 << (i - 0x10))) == 0) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      } else if (UiWorldMapIsRankMode(screen)) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      } else {
        SaveProfile *profile = SaveGetProfile();
        int group = map->areaGroup[i - 0x10];
        GfxSprite *sprite = ((GfxSprite **)screen->data)[i];

        if ((u8)(profile->data->areaVisited[group / 8] & (1 << (group % 8))) == 0) {
          sprite->flags |= 1;
        } else {
          sprite->flags &= ~1u;
        }
      }
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
    for (i = 0x2a; i < 0x32; i++) {
      int bit = 1 << (i - 0x2a);

      if ((map->unlockMask & bit) == 0) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      } else {
        bool rankMode = UiWorldMapIsRankMode(screen);
        GfxSprite *sprite = ((GfxSprite **)screen->data)[i];

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
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
  } else {
    for (i = 0; i < 8; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
    for (i = 8; i < 0x10; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
    for (i = 0x10; i < 0x18; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
    for (i = 0x2a; i < 0x32; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 1);
    }
  }
}
