// bdc 0x08935304 UiGauntletSetupToggleSlotMark
#include "bdc.h"

/* Marks (`mark != 0`) or unmarks (`mark == 0`) card slot `slot` in
   `UiGauntletSetup` (task 373). At most two slots are marked; their cards
   are kept in `chosenCard[0..1]` (0xff = free). `slotMark` bit 0 = marked, bit 1 = marked before
   the latest pick (replaceable by the next pick).
   Mark: with no slot marked, marks `slot` (bits 0|1) and stores its card in `chosenCard[0]`; with
   one marked, marks `slot`, sets bit 1 on the other marked slots and stores the card in the first
   free `chosenCard`; with two marked, unmarks the other marked slots that already had bit 1 (freeing
   their `chosenCard` entry), sets bit 1 on the remaining ones, then marks `slot` and stores its card
   in the first free `chosenCard`; with more, does nothing.
   Unmark: with one or two marked, clears `slotMark[slot]` (with two, sets bit 1 on the slots still
   marked) and frees the `chosenCard` entry holding the slot's card; otherwise does nothing. */

void UiGauntletSetupToggleSlotMark(UiGauntletSetup *self, s8 mark, u8 slot)
{
  int count;
  int i;
  int j;
  u8 card;

  count = 0;
  for (i = 0; i < 4; i++) {
    if ((self->slotMark[i] & 1) != 0) {
      count++;
    }
  }

  if (mark != 0) {
    if (count <= 0) {
      if (count < 0) {
        return;
      }
      self->slotMark[slot] |= 1;
      self->chosenCard[0] = self->slotCard[slot];
      self->slotMark[slot] |= 2;
      return;
    }
    if (count < 2) {
      self->slotMark[slot] |= 1;
      for (i = 0; i < 4; i++) {
        if (i != slot && (self->slotMark[i] & 1) != 0) {
          self->slotMark[i] |= 2;
        }
      }
      for (j = 0; j < 2; j++) {
        if (self->chosenCard[j] == 0xff) {
          self->chosenCard[j] = self->slotCard[slot];
          return;
        }
      }
      return;
    }
    if (count >= 3) {
      return;
    }
    for (i = 0; i < 4; i++) {
      if (i != slot && (self->slotMark[i] & 1) != 0) {
        if ((self->slotMark[i] & 2) != 0) {
          self->slotMark[i] = 0;
          for (j = 0; j < 2; j++) {
            if (self->chosenCard[j] == self->slotCard[i]) {
              self->chosenCard[j] = 0xff;
              break;
            }
          }
        } else {
          self->slotMark[i] |= 2;
        }
      }
    }
    self->slotMark[slot] |= 1;
    for (j = 0; j < 2; j++) {
      if (self->chosenCard[j] == 0xff) {
        self->chosenCard[j] = self->slotCard[slot];
        return;
      }
    }
    return;
  }

  if (count <= 0) {
    return;
  }
  if (count < 2) {
    self->slotMark[slot] = 0;
    card = self->slotCard[slot];
    for (j = 0; j < 2; j++) {
      if (self->chosenCard[j] == card) {
        self->chosenCard[j] = 0xff;
        return;
      }
    }
    return;
  }
  if (count >= 3) {
    return;
  }
  self->slotMark[slot] = 0;
  card = self->slotCard[slot];
  for (i = 0; i < 4; i++) {
    if ((self->slotMark[i] & 1) != 0) {
      self->slotMark[i] |= 2;
    }
  }
  for (j = 0; j < 2; j++) {
    if (self->chosenCard[j] == card) {
      self->chosenCard[j] = 0xff;
      return;
    }
  }
}
