// bdc 0x0898e728 UiCollectionFigureMarkEntrySeen
#include "bdc.h"

/* Marks the selected entry of `UiCollectionFigure` as seen: sets bit `id`
   in the profile's figure-seen bitfield (profile data `+0x543`), clears its "new" flag
   `entryNew[page * 6 + cursor]` and hides the cell's "new" badge sprite (data `+0x34 + cursor * 4`).
    */

void UiCollectionFigureMarkEntrySeen(UiCollectionFigure *self)

{
  SaveProfile *profile;
  u8 *bits;
  int id;
  GfxSprite *sprite;

  profile = SaveGetProfile();
  id = self->entryIds[self->cursor + self->page * 6];
  /* profile data +0x543 = upgradeOwned + 3 (bitfield overlaps; see SaveProfileData definition) */
  bits = &profile->data->upgradeOwned[0][3 + id / 8];
  *bits = *bits | (u8)(1 << (id % 8));
  self->entryNew[self->cursor + self->page * 6] = 0;
  sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
  sprite->flags &= ~1u;
  return;
}
