// bdc 0x0892b13c UiBakuganSelectBuildOwnedList
#include "bdc.h"

/* Builds the list of owned Bakugan of the Bakugan select screen (`UiBakuganSelectCtor`, task
   371): clears the 20 entries `entries`, stores the unselectable masks
   (`UiBakuganGetUnselectableMask`) in `unselectableMask`, then for every grid position
   (`UiBakuganListOrder`) whose Bakugan is owned (profile bitset `ownedBakugan`) fills the entry:
   `bakugan` = id, `notEvolved` = 1 when the id is not marked evolved (profile bitset
   `ownedItems + 11`, `+0x525`), the owned evolved form `evolvedForm` (`UiBakuganGetPair`) and the
   four display shorts from `g_uiBakuganSelectDisplayTable` (row = `UiBakuganListOrderB` slot).
   Entries of unowned positions stay zero. */

void UiBakuganSelectBuildOwnedList(UiBakuganSelect *self)
{
  u32 mask[2];
  UiBakuganSelectEntry *entry;
  const s16 *row;
  s32 i;
  s32 id;
  s32 form;

  memset(self->entries, 0, sizeof(self->entries));
  UiBakuganGetUnselectableMask(mask);
  self->unselectableMask[0] = mask[0];
  self->unselectableMask[1] = mask[1];

  entry = self->entries;
  for (i = 0; i < 20; i++, entry++) {
    id = UiBakuganListOrder(self, true, (u8)i);
    if ((u8)(SaveGetProfile()->data->ownedBakugan[id / 8] & (1 << (id % 8))) == 0) {
      continue;
    }
    entry->bakugan = (u8)id;
    if ((u8)(SaveGetProfile()->data->ownedItems[11 + id / 8] & (1 << (id % 8))) == 0) {
      entry->notEvolved = 1;
    }
    row = g_uiBakuganSelectDisplayTable[UiBakuganListOrderB(self, false, id)];
    entry->display[0] = row[0];
    entry->display[1] = row[1];
    entry->display[2] = row[2];
    entry->display[3] = row[3];

    form = UiBakuganGetPair(0, id);
    if (form == 0) {
      continue;
    }
    if ((u8)(SaveGetProfile()->data->ownedBakugan[form / 8] & (1 << (form % 8))) != 0) {
      entry->evolvedForm = (u8)form;
    }
  }
}
