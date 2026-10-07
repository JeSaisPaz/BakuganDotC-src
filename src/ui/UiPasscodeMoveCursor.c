// bdc 0x0893e990 UiPasscodeMoveCursor
#include "bdc.h"

/* D-pad handling of `UiPasscode` (pad `repeat`): Up/Down toggle `onCommandRow`
   between the symbol pad (0) and the command row (1, `command` = `focusSymbol` / 5); Left/Right
   move the column of the current row (`(&focusSymbol)[onCommandRow]`), wrapping over the 10
   symbols or the 2 commands. Returns 1 when a direction was pressed, else 0. */

s32 UiPasscodeMoveCursor(UiScreen *screen)
{
  UiPasscode *pc = (UiPasscode *)screen;
  PadState *pad = screen->pad;
  signed char *col;
  s32 row;

  if ((pad->repeat & 0x10) != 0) {
    if (pc->onCommandRow == 1) {
      pc->onCommandRow = 0;
      return 1;
    }
    pc->onCommandRow = 1;
    pc->command = pc->focusSymbol / 5;
    return 1;
  }
  if ((pad->repeat & 0x40) != 0) {
    if (pc->onCommandRow != 0) {
      pc->onCommandRow = 0;
      return 1;
    }
    pc->onCommandRow = 1;
    pc->command = pc->focusSymbol / 5;
    return 1;
  }
  if ((pad->repeat & 0x80) != 0) {
    row = pc->onCommandRow;
    col = &pc->focusSymbol + row;
    *col = *col - 1;
    if (*col < 0) {
      *col = (row == 0) ? 9 : 1;
    }
    return 1;
  }
  if ((pad->repeat & 0x20) != 0) {
    row = pc->onCommandRow;
    col = &pc->focusSymbol + row;
    *col = *col + 1;
    if (row == 0) {
      if (*col >= 10) {
        *col = 0;
      }
    } else if (*col >= 2) {
      *col = 0;
    }
    return 1;
  }
  return 0;
}
