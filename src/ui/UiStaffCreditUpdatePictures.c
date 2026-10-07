// bdc 0x08945448 UiStaffCreditUpdatePictures
#include "bdc.h"

/* Picture track of `UiStaffCredit` (state `+0xe4`): 0 → waits until the roll
   frame `+0x74` reaches `g_staffCreditLayout.picturesStart`, then resets the picture index `+0x80`
   and starts; 1 → `UiStaffCreditAnimatePictures` until frame 0x42fe, then 2 (done). */

void UiStaffCreditUpdatePictures(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;

  if (credit->pictureTrack < 1) {
    if (credit->pictureTrack >= 0 && credit->rollFrame >= g_staffCreditLayout.picturesStart) {
      credit->pictureTrack = 1;
      credit->pictureIndex = 0;
      UiStaffCreditUpdatePictures(screen);
    }
  } else if (credit->pictureTrack < 2) {
    UiStaffCreditAnimatePictures(screen);
    if (credit->rollFrame > 0x42fd) {
      credit->pictureTrack = 2;
    }
  }
}
