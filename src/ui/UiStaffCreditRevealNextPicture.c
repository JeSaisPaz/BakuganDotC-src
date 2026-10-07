// bdc 0x089450f0 UiStaffCreditRevealNextPicture
#include "bdc.h"

/* Shows the next of the 13 credit pictures of `UiStaffCredit` when its time
   comes: picture n (`+0x80`, sprite 0x0a+n) appears once the roll frame `+0x74` reaches `0x08ac1be8
   + n·0x08ac1bec − +0xdc`; marks its slot active (`+0x104 + 8n`), adds 4 to the delay `+0xdc`
   and advances n. */

void UiStaffCreditRevealNextPicture(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  UiStaffCreditData *data;
  s32 n;

  n = credit->pictureIndex;
  if (n < 13 &&
      g_staffCreditLayout.picturesStart + g_staffCreditLayout.pictureInterval * n -
              credit->pictureDelay <=
          credit->rollFrame) {
    data = (UiStaffCreditData *)credit->base.data;
    data->pictures[n]->flags |= 1;
    credit->pictures[credit->pictureIndex].state = 1;
    credit->pictureDelay += 4;
    credit->pictureIndex++;
  }
}
