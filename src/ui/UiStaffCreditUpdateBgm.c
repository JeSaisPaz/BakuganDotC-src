// bdc 0x08945de4 UiStaffCreditUpdateBgm
#include "bdc.h"

/* BGM track of `UiStaffCredit` (state `+0xec`): 0 → plays BGM 0x24 on player
   0 (`SndBgmPlayerPlayTrack`); 1 → at roll frame 0x42fe cancels channel 0 and queues a 0.1 s
   stop (`SndBgmQueueStop`); then 2. */

void UiStaffCreditUpdateBgm(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  SndBgmPlayer *player;

  if (credit->bgmTrack < 1) {
    if (credit->bgmTrack >= 0 && credit->rollFrame >= 0) {
      player = SndBgmPlayerGet(0);
      SndBgmPlayerPlayTrack(player, 0x24, 1, 0);
      credit->bgmTrack = 1;
    }
  } else if (credit->bgmTrack < 2 && credit->rollFrame > 0x42fd) {
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.1f, 0);
    credit->bgmTrack = 2;
  }
}
