// bdc 0x0897d288 UiCollectionSphereMarkEntrySeen
#include "bdc.h"

/* Marks the selected entry of `UiCollectionSphere` as seen.
   Categories 0/1: sets bit `id` of the profile's `newItemGroups[0x10..]` (data `+0x5e3`) for
   id = `entryIds[page * 6 + cursor]` and clears `entryNew` of that slot. Category 2 asks
   `UiCollectionSphereGetPageKind`: kind 0 or 0xff sets bit `cursor + 1` of the bitmap at data
   `+0x53e + page / 3` and clears `entryNew[(page / 3) * 6 + cursor]`; kind 1/2 sets the bit of
   `kind1Ids`/`kind2Ids[page / 3]` in `newItemGroups[0x10..]` and clears `kind1New`/`kind2New`.
   Other categories/kinds mark nothing. Always clears bit 0 of the flags of cursor sprite
   `14 + cursor`. */

void UiCollectionSphereMarkEntrySeen(UiCollectionSphere *self)
{
    SaveProfile *profile;
    int id;
    int group;
    u8 kind;
    GfxSprite *sprite;

    if (self->category < 2) {
        if (self->category >= 0) {
            profile = SaveGetProfile();
            id = self->entryIds[self->cursor + self->page * 6];
            profile->data->newItemGroups[0x10 + id / 8] |= (u8)(1 << (id % 8));
            self->entryNew[self->cursor + self->page * 6] = 0;
        }
    } else if (self->category < 3) {
        kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
        if (kind == 0xff || kind == 0) {
            profile = SaveGetProfile();
            group = self->page / 3;
            id = self->cursor + 1;
            profile->data->specialBits[2 + group + id / 8] |= (u8)(1 << (id % 8));
            self->entryNew[self->cursor + (self->page / 3) * 6] = 0;
        } else if (kind < 2) {
            if (kind > 0) {
                profile = SaveGetProfile();
                id = self->kind1Ids[self->page / 3];
                profile->data->newItemGroups[0x10 + id / 8] |= (u8)(1 << (id % 8));
                self->kind1New[self->page / 3] = 0;
            }
        } else if (kind < 3) {
            profile = SaveGetProfile();
            id = self->kind2Ids[self->page / 3];
            profile->data->newItemGroups[0x10 + id / 8] |= (u8)(1 << (id % 8));
            self->kind2New[self->page / 3] = 0;
        }
    }
    sprite = ((GfxSprite **)self->base.data)[14 + self->cursor];
    sprite->flags &= ~1u;
}
