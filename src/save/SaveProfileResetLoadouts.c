// bdc 0x0880c9c4 SaveProfileResetLoadouts
#include "bdc.h"

/* Part of `SaveProfileReset`: resets the Bakugan loadouts of the profile data (`self->data`) and
   grants the starter set: current Bakugan `curBakugan = 1`; the 21 per-Bakugan entry pairs
   `loadoutPairs` (two bytes each, -1) and equip slots `equipSlots` are emptied; Bakugan 1 is marked
   owned (bit 1 of `ownedBakugan`); items 0 and 2 are marked owned in `ownedItems` and, when newly
   granted, equipped into Bakugan 1's first free slot (`equipSlots[1][0]`, else `[1][1]`); then the
   default bits are set in `bakuganBitsA`, `bakuganBitsB`, `newItems` and `newItemGroups`. */

void SaveProfileResetLoadouts(SaveProfile *self)

{
  s32 bakugan;
  s32 slot;

  self->data->curBakugan = 1;
  for (bakugan = 0; bakugan < 21; bakugan++) {
    for (slot = 0; slot < 2; slot++) {
      self->data->loadoutPairs[bakugan][slot][0] = -1;
      self->data->loadoutPairs[bakugan][slot][1] = -1;
      self->data->equipSlots[bakugan][slot] = 0xff;
    }
  }
  self->data->ownedBakugan[0] |= 2;
  if ((self->data->ownedItems[0] & 1) == 0) {
    self->data->ownedItems[0] |= 1;
    if (self->data->equipSlots[1][0] == 0xff) {
      self->data->equipSlots[1][0] = 0;
    }
    else if (self->data->equipSlots[1][1] == 0xff) {
      self->data->equipSlots[1][1] = 0;
    }
  }
  if ((self->data->ownedItems[0] & 4) == 0) {
    self->data->ownedItems[0] |= 4;
    if (self->data->equipSlots[1][0] == 0xff) {
      self->data->equipSlots[1][0] = 2;
    }
    else if (self->data->equipSlots[1][1] == 0xff) {
      self->data->equipSlots[1][1] = 2;
    }
  }
  self->data->bakuganBitsA[0] |= 2;
  self->data->bakuganBitsA[0] |= 8;
  self->data->bakuganBitsA[0] |= 0x20;
  self->data->bakuganBitsA[0] |= 0x40;
  self->data->bakuganBitsA[0] |= 0x80;
  self->data->bakuganBitsA[1] |= 1;
  self->data->newItems[0] |= 1;
  self->data->newItems[0] |= 4;
  self->data->newItems[1] |= 2;
  self->data->newItems[2] |= 4;
  self->data->newItems[2] |= 0x20;
  self->data->newItems[3] |= 2;
  self->data->newItems[3] |= 0x10;
  self->data->bakuganBitsB[0] |= 2;
  self->data->newItemGroups[0] |= 1;
  self->data->newItemGroups[0] |= 4;
}
