// bdc 0x089315cc UiGauntletSetupLoadCardSlots
#include "bdc.h"

/* Initialises the card slots of `UiGauntletSetup` (task 373) from the save
   profile (`SaveGetProfile`): resets `slotCard` to 0xff x4, `slotMark`/`slotNew` to 0 x4 and
   `chosenCard` to 0xff x2. For the current Bakugan (`curBakugan`, slot block `cur = curBakugan - 1`
   truncated to u8) each slot i (0..3) takes card `cur * 4 + i` if its bit is set in the owned bitset
   `ownedItems`; an owned card whose seen bit (`ownedItems[0x17 + card / 8]`, profile `+0x531`) is
   clear gets `slotNew[i] = 1`. Then the two equipped cards `equipSlots[cur + 1][0..1]` (0xff =
   empty) are copied to `chosenCard`; the slot holding each one gets mark bit 1, and the first such
   slot also bit 2. */

void UiGauntletSetupLoadCardSlots(UiGauntletSetup *self)
{
  u8 cur;
  u8 firstMarked;
  u8 card;
  int idx;
  int i;
  int k;

  memset(self->slotCard, 0xff, 4);
  memset(self->slotMark, 0, 4);
  memset(self->slotNew, 0, 4);
  memset(self->chosenCard, 0xff, 2);
  cur = (u8)(SaveGetProfile()->data->curBakugan - 1);
  for (i = 0; i < 4; i++) {
    idx = cur * 4 + i;
    if ((u8)(SaveGetProfile()->data->ownedItems[idx / 8] & (1 << (idx % 8))) != 0) {
      self->slotCard[i] = (u8)idx;
      if ((u8)(SaveGetProfile()->data->ownedItems[0x17 + idx / 8] & (1 << (idx % 8))) == 0) {
        self->slotNew[i] = 1;
      }
    }
  }
  firstMarked = 0;
  for (i = 0; i < 2; i++) {
    card = SaveGetProfile()->data->equipSlots[cur + 1][i];
    if (card == 0xff) {
      continue;
    }
    self->chosenCard[i] = card;
    for (k = 0; k < 4; k++) {
      if (self->slotCard[k] == card) {
        self->slotMark[k] |= 1;
        if (firstMarked == 0) {
          self->slotMark[k] |= 2;
          firstMarked++;
        }
        break;
      }
    }
  }
}
