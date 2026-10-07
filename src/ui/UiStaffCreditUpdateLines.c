// bdc 0x08945d58 UiStaffCreditUpdateLines
#include "bdc.h"

/* Text-line track of `UiStaffCredit` (state `+0xe8`): 0 → starts at once
   (resets the line cursor `+0xe0`); 1 → `UiStaffCreditScrollLines` every frame until roll frame
   0x42fe, then 2. */

void UiStaffCreditUpdateLines(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;

  if (credit->lineTrack < 1) {
    if (credit->lineTrack >= 0 && credit->rollFrame >= 0) {
      credit->lineTrack = 1;
      credit->lineCursor = 0;
      UiStaffCreditUpdateLines(screen);
    }
  } else if (credit->lineTrack < 2) {
    UiStaffCreditScrollLines(screen);
    if (credit->rollFrame > 0x42fd) {
      credit->lineTrack = 2;
    }
  }
}
