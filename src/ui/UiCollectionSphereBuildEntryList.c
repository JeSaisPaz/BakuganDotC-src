// bdc 0x08978958 UiCollectionSphereBuildEntryList
#include "bdc.h"

/* Builds the entry lists of the sphere (Bakugan figure) collection screen for the current
   `category`: clears `entryIds`/`entryNew` and the `kind2Ids`/`kind2New`/`kind1Ids`/`kind1New`
   pairs, then collects the owned entries from the save profile.
   - Category 0 (19 slots) / 1 (11 slots): slot `i` holds id `UiCollectionSphereGetCat0EntryId` /
     `UiCollectionSphereGetCat1EntryId(self, 1, i)` if bit `id` of `bakuganBitsB` is set; it is
     flagged new unless bit `id` of `newItemGroups[0x10..]` (seen marks) is set.
   - Category 2: two rows of 6 slots; slot `row * 6 + j` holds `UiCollectionSphereGetCat2EntryId`
     if bit `j + 1` of `specialBits[row..]` is set, new unless the same bit of `specialBits[2 + row..]`
     is set. Then `kind2Ids` gets Bakugan ids 2/10 and `kind1Ids` ids 1/9 if owned
     (`bakuganBitsB`), new unless seen (`newItemGroups[0x10..]`).
   - Any other category: only the clears. */

void UiCollectionSphereBuildEntryList(UiCollectionSphere *self)
{
    int i;
    int id;
    int row;
    int base;
    int j;
    int bit;
    int slot;
    s8 category;

    id = 0;
    memset(self->entryIds, 0, 0x18);
    memset(self->entryNew, 0, 0x18);
    memset(self->kind2Ids, 0, 2);
    memset(self->kind2New, 0, 2);
    memset(self->kind1Ids, 0, 2);
    memset(self->kind1New, 0, 2);
    category = self->category;
    if (category <= 0) {
        if (category < 0) {
            return;
        }
        for (i = 0; i < 19; i++) {
            id = UiCollectionSphereGetCat0EntryId(self, 1, (u8)i);
            if ((u8)(SaveGetProfile()->data->bakuganBitsB[id / 8] & (1 << (id % 8))) != 0) {
                self->entryIds[i] = (u8)id;
                if ((u8)(SaveGetProfile()->data->newItemGroups[0x10 + id / 8] & (1 << (id % 8))) == 0) {
                    self->entryNew[i] = 1;
                }
            }
        }
    } else if (category < 2) {
        for (i = 0; i < 11; i++) {
            id = UiCollectionSphereGetCat1EntryId(self, 1, (u8)i);
            if ((u8)(SaveGetProfile()->data->bakuganBitsB[id / 8] & (1 << (id % 8))) != 0) {
                self->entryIds[i] = (u8)id;
                if ((u8)(SaveGetProfile()->data->newItemGroups[0x10 + id / 8] & (1 << (id % 8))) == 0) {
                    self->entryNew[i] = 1;
                }
            }
        }
    } else if (category < 3) {
        base = 0;
        for (row = 0; row < 2; row++) {
            for (j = 0; j < 6; j++) {
                slot = base + j;
                id = UiCollectionSphereGetCat2EntryId(self, 1, (u8)slot);
                bit = j + 1;
                if ((u8)(SaveGetProfile()->data->specialBits[row + bit / 8] & (1 << (bit % 8))) != 0) {
                    self->entryIds[slot] = (u8)id;
                    if ((u8)(SaveGetProfile()->data->specialBits[2 + row + bit / 8] & (1 << (bit % 8))) == 0) {
                        self->entryNew[slot] = 1;
                    }
                }
            }
            base += 6;
        }
        /* id keeps its value from the code above for an out-of-range index (never happens). */
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                id = 2;
            } else if (i == 1) {
                id = 10;
            }
            if ((u8)(SaveGetProfile()->data->bakuganBitsB[id / 8] & (1 << (id % 8))) != 0) {
                self->kind2Ids[i] = (u8)id;
                if ((u8)(SaveGetProfile()->data->newItemGroups[0x10 + id / 8] & (1 << (id % 8))) == 0) {
                    self->kind2New[i] = 1;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                id = 1;
            } else if (i == 1) {
                id = 9;
            }
            if ((u8)(SaveGetProfile()->data->bakuganBitsB[id / 8] & (1 << (id % 8))) != 0) {
                self->kind1Ids[i] = (u8)id;
                if ((u8)(SaveGetProfile()->data->newItemGroups[0x10 + id / 8] & (1 << (id % 8))) == 0) {
                    self->kind1New[i] = 1;
                }
            }
        }
    }
}
