// bdc 0x0894eb1c UiNetMenuMoveCursor
#include "bdc.h"

/* Moves the choice of `UiNetMenu` between its two buttons: a queued move
   (`cursorMove.queued`, direction `cursorMove.dir`) or a held pad button (`buttons` bit 0x80 /
   0x20) steps `choice` forward/back (previous in `prevChoice`, wrapping over 0..1), clears the
   12-byte `cursorMove`, sets its direction (0 forward, 1 back) and speed 8, and returns 1;
   otherwise returns 0. */

s32 UiNetMenuMoveCursor(UiScreen *screen)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  s8 choice;

  if (menu->cursorMove.queued != 0) {
    choice = menu->choice;
    if (menu->cursorMove.dir == 0) {
      menu->prevChoice = choice;
      menu->choice = choice + 1;
      if (menu->choice >= 2) {
        menu->choice = 0;
      }
      memset(&menu->cursorMove, 0, 0xc);
      menu->cursorMove.dir = 0;
      menu->cursorMove.speed = 8.0f;
      return 1;
    }
    menu->prevChoice = choice;
    menu->choice = choice - 1;
    if (menu->choice < 0) {
      menu->choice = 1;
    }
    memset(&menu->cursorMove, 0, 0xc);
    menu->cursorMove.dir = 1;
    menu->cursorMove.speed = 8.0f;
    return 1;
  }
  if (screen->pad->buttons & 0x80) {
    choice = menu->choice;
    menu->choice = choice + 1;
    menu->prevChoice = choice;
    if (menu->choice >= 2) {
      menu->choice = 0;
    }
    memset(&menu->cursorMove, 0, 0xc);
    menu->cursorMove.dir = 0;
    menu->cursorMove.speed = 8.0f;
    return 1;
  }
  if (screen->pad->buttons & 0x20) {
    choice = menu->choice;
    menu->choice = choice - 1;
    menu->prevChoice = choice;
    if (menu->choice < 0) {
      menu->choice = 1;
    }
    memset(&menu->cursorMove, 0, 0xc);
    menu->cursorMove.dir = 1;
    menu->cursorMove.speed = 8.0f;
    return 1;
  }
  return 0;
}
