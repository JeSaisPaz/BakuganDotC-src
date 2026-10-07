// bdc 0x0896c74c UiCardEquipToggleCard
#include "bdc.h"

/* Turns card `card` of Bakugan `bakugan` in `UiCardEquip` on (`on` != 0) or off in
   the flag bytes `cardIds[0x10 + bakugan*4 + card]` (`+0x29fc`), keeping at most two active cards:
   bit0 = active, bit1 = "older" mark. On: with no active card sets bits 0|1; with one, sets bit0 and
   marks the other active card older; with two, clears the active card that was already older,
   marks the other one older and sets bit0 (three or more: nothing). Off: with one active card clears
   the byte; with two clears it and marks the remaining active card older. */

void UiCardEquipToggleCard(UiCardEquip *self, u8 on, u8 bakugan, u8 card)
{
  u8 *flags;
  s32 count;
  s32 k;
  u8 f;

  flags = &self->cardIds[0x10 + bakugan * 4];
  count = 0;
  for (k = 0; k < 4; k++) {
    if ((flags[k] & 1) != 0) {
      count++;
    }
  }
  if (on != 0) {
    switch (count) {
    case 0:
      flags[card] |= 1;
      flags[card] |= 2;
      return;
    case 1:
      flags[card] |= 1;
      for (k = 0; k < 4; k++) {
        if (k != card && (flags[k] & 1) != 0) {
          flags[k] |= 2;
        }
      }
      break;
    case 2:
      for (k = 0; k < 4; k++) {
        if (k != card) {
          f = flags[k];
          if ((f & 1) != 0) {
            flags[k] = ((f & 2) != 0) ? 0 : (u8)(f | 2);
          }
        }
      }
      flags[card] |= 1;
      return;
    }
  } else {
    switch (count) {
    case 1:
      flags[card] = 0;
      return;
    case 2:
      flags[card] = 0;
      for (k = 0; k < 4; k++) {
        if ((flags[k] & 1) != 0) {
          flags[k] |= 2;
        }
      }
      break;
    }
  }
}
