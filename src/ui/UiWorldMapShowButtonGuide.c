// bdc 0x089993c0 UiWorldMapShowButtonGuide
#include "bdc.h"

/* Shows (`show` = 1) or hides the button-guide sprites 0x22..0x29 and 0x5b/0x5c of
   `UiWorldMap`: assigns button icons (`UiSetButtonIcon`: 2, 1, 3, 0 for guides
   0x23..0x26), hides guide 0x26 and sprites 0x5b/0x5c in rank mode (`UiWorldMapIsRankMode`),
   guides 0x25/0x29 follow `randomEnabled`, every guide but 0x26 gets layer mask 4, all get full
   alpha. Hiding clears the visible bit of all ten. For the second player of a network session
   (profile flag 0, `NetGetLocalPlayerIndex` = 1) only sprite 0x22 is shown/hidden, together with
   help message 5 placed 6 px above it (`UiHelpLineShow`/`UiHelpLineHide`). */

void UiWorldMapShowButtonGuide(UiScreen *screen, u8 show)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *sprite;
  int i;

  if (SaveGetProfileFlag0() != 0 && NetGetLocalPlayerIndex() == 1) {
    sprite = ((GfxSprite **)screen->data)[0x22];
    if (show != 0) {
      sprite->flags |= 1;
      ((GfxSprite **)screen->data)[0x22]->alpha = 1.0f;
      sprite = ((GfxSprite **)screen->data)[0x22];
      UiHelpLineShow(sprite->posX, sprite->posY - 6.0f, 5);
    }
    else {
      sprite->flags &= ~1u;
      UiHelpLineHide();
    }
    return;
  }

  if (show == 0) {
    for (i = 0x22; i < 0x2a; i++) {
      ((GfxSprite **)screen->data)[i]->flags &= ~1u;
    }
    for (i = 0x5b; i < 0x5d; i++) {
      ((GfxSprite **)screen->data)[i]->flags &= ~1u;
    }
    return;
  }

  for (i = 0x22; i < 0x2a; i++) {
    if (i == 0x23) {
      UiSetButtonIcon(((GfxSprite **)screen->data)[i], 2);
    }
    else if (i == 0x24) {
      UiSetButtonIcon(((GfxSprite **)screen->data)[i], 1);
    }
    else if (i == 0x25) {
      UiSetButtonIcon(((GfxSprite **)screen->data)[i], 3);
    }
    else if (i == 0x26) {
      UiSetButtonIcon(((GfxSprite **)screen->data)[i], 0);
    }

    if (UiWorldMapIsRankMode(screen) == 1 && i == 0x26) {
      ((GfxSprite **)screen->data)[i]->flags &= ~1u;
    }
    else {
      ((GfxSprite **)screen->data)[i]->flags |= 1;
    }

    if (i == 0x25 || i == 0x29) {
      if (map->randomEnabled == 0) {
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      }
      else {
        ((GfxSprite **)screen->data)[i]->flags |= 1;
      }
    }

    if (i != 0x26) {
      ((GfxSprite **)screen->data)[i]->layerMask = 4;
    }
    ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
  }

  for (i = 0x5b; i < 0x5d; i++) {
    if (UiWorldMapIsRankMode(screen) == 1) {
      ((GfxSprite **)screen->data)[i]->flags &= ~1u;
    }
    else {
      ((GfxSprite **)screen->data)[i]->flags |= 1;
    }
    ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
  }
}
