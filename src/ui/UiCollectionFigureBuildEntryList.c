// bdc 0x0898ad40 UiCollectionFigureBuildEntryList
#include "bdc.h"

/* Builds the entry lists of the figure collection screen (`UiCollectionFigure`,
   task 314): clears `entryIds` and `entryNew` (24 bytes each), then for `category` 0 (18 slots,
   `UiCollectionFigureGetCat0EntryId`) or 1 (2 slots, `UiCollectionFigureGetCat1EntryId`) walks
   the special figure ids: an id whose bit is set in the profile's figure-owned bitfield (profile
   data `+0x540`) is stored at `entryIds[slot]`, and `entryNew[slot]` is set to 1 when its bit in
   the figure-seen bitfield (`+0x543`) is clear. Unowned slots stay 0. Any other category leaves the
   lists empty. */

void UiCollectionFigureBuildEntryList(UiCollectionFigure *self)

{
  int slot;
  int id;

  memset(self->entryIds, 0, 0x18);
  memset(self->entryNew, 0, 0x18);
  switch (self->category) {
  case 0:
    for (slot = 0; slot < 18; slot++) {
      id = UiCollectionFigureGetCat0EntryId(self, 1, (u8)slot);
      /* profile data +0x540 / +0x543 overlap upgradeOwned (see SaveProfileData definition) */
      if ((SaveGetProfile()->data->upgradeOwned[0][id / 8] & (u8)(1 << (id % 8))) != 0) {
        self->entryIds[slot] = (u8)id;
        if ((SaveGetProfile()->data->upgradeOwned[0][3 + id / 8] & (u8)(1 << (id % 8))) == 0) {
          self->entryNew[slot] = 1;
        }
      }
    }
    break;
  case 1:
    for (slot = 0; slot < 2; slot++) {
      id = UiCollectionFigureGetCat1EntryId(self, 1, (u8)slot);
      if ((SaveGetProfile()->data->upgradeOwned[0][id / 8] & (u8)(1 << (id % 8))) != 0) {
        self->entryIds[slot] = (u8)id;
        if ((SaveGetProfile()->data->upgradeOwned[0][3 + id / 8] & (u8)(1 << (id % 8))) == 0) {
          self->entryNew[slot] = 1;
        }
      }
    }
    break;
  }
}
