// bdc 0x0893ec28 UiPasscodeCheckEntry
#include "bdc.h"

/* Compares the entry `+0x7f2` of `UiPasscode` with the answer `+0x7ec` over the
   answer length `+0x7e4`; returns 1 when they match, else 0. */

s32 UiPasscodeCheckEntry(UiScreen *screen)
{
  UiPasscode *self = (UiPasscode *)screen;
  s32 i;

  for (i = 0; i < (s32)self->answerLen; i++) {
    if (self->entry[i] != self->answer[i]) {
      return 0;
    }
  }
  return 1;
}
