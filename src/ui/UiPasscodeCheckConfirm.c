// bdc 0x0893eae8 UiPasscodeCheckConfirm
#include "bdc.h"

/* Checks Cross (pad `pressed` 0x4000) in `UiPasscode`: returns 0 if not pressed;
   on the symbol pad 1 (enter a symbol) or 2 when the entry is already full (`+0x7fc` = length
   `+0x7e4`); on the command row 2 for command 0 with an empty entry, else 1. */

s32 UiPasscodeCheckConfirm(UiScreen *screen)
{
  UiPasscode *p = (UiPasscode *)screen;

  if ((screen->pad->pressed & 0x4000) == 0) {
    return 0;
  }
  if (p->onCommandRow == 0) {
    return p->entryCount == p->answerLen ? 2 : 1;
  }
  if (p->command == 0 && p->entryCount == 0) {
    return 2;
  }
  return 1;
}
