// bdc 0x0894ecec UiNetMenuStartCursorMove
#include "bdc.h"

/* Starts the button swap of `UiNetMenu` after a move: flashes the arrow sprite of
   the direction (sprite 2+`cursorMove.dir`, `UiFlashStart`), hides the old button's glow, and sets
   up the slides of the outgoing button (sprite 4+old, off to X 704 for dir 0 or −224 for dir 1)
   and the incoming one (sprite 4+new, placed at the opposite edge and sliding to `buttonX`) with
   their distances. */

void UiNetMenuStartCursorMove(UiScreen *screen)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite **sprites;
  UiNetMenuButtonSlide *slide;
  GfxSprite *sprite;
  s32 idx;
  s16 toX;
  s16 x;
  float dist;

  sprites = (GfxSprite **)screen->data;
  UiFlashStart(2.0f, sprites[2 + menu->cursorMove.dir], 0, 0);
  menu->titleFadingOut = 0;
  UiNetMenuResetButtonGlow(screen, 0, (u8)menu->prevChoice);

  /* outgoing button */
  idx = menu->prevChoice;
  slide = &menu->buttonSlide[idx];
  sprite = ((GfxSprite **)screen->data)[4 + idx];
  slide->fromX = (s16)(s32)sprite->posX;
  toX = -0xe0;
  if (menu->cursorMove.dir == 0) {
    toX = 0x2c0;
  }
  slide->toX = toX;
  dist = UiAbsDiff(((GfxSprite **)screen->data)[4 + idx]->posX, (float)slide->toX);
  slide->distance = (s16)(s32)dist;

  /* incoming button */
  idx = menu->choice;
  slide = &menu->buttonSlide[idx];
  sprite = ((GfxSprite **)screen->data)[4 + idx];
  if (menu->cursorMove.dir == 0) {
    sprite->posX = -224.0f;
  } else {
    sprite->posX = 704.0f;
  }
  x = (s16)(s32)menu->buttonX;
  sprite = ((GfxSprite **)screen->data)[4 + idx];
  slide->toX = x;
  slide->fromX = (s16)(s32)sprite->posX;
  dist = UiAbsDiff(((GfxSprite **)screen->data)[4 + idx]->posX, (float)slide->toX);
  slide->distance = (s16)(s32)dist;
}
