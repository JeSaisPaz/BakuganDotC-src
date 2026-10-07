// bdc 0x0892f490 UiBakuganSelectMarkSeen
#include "bdc.h"

/* Clears the new-mark of the entry under the cursor of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4`
   with 0xc-byte entries): hides its `NEW` sprite (`+0x178 + 4*cursor`), clears the entry flag
   `+0x1ba5` and sets its bit in a save-profile bitset. */

void UiBakuganSelectMarkSeen(UiBakuganSelect *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  SaveProfile *profile;
  u32 id;

  sprites[94 + self->cursor]->flags &= ~1u;
  self->entries[self->cursor].notEvolved = 0;
  profile = SaveGetProfile();
  id = self->entries[self->cursor].bakugan;
  profile->data->ownedItems[11 + (id >> 3)] |= (u8)(1 << (id & 7));
  self->currentPos = self->entries[self->cursor].bakugan;
  profile = SaveGetProfile();
  profile->data->curBakugan = self->currentPos;
}
