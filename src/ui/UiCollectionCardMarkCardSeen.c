// bdc 0x08984cb8 UiCollectionCardMarkCardSeen
#include "bdc.h"

/* Marks the selected card of `UiCollectionCard` as seen: sets its bit in the
   profile array `+0x5f3`, clears its new flag `+0xc28` and hides its "new" marker sprite (0x09 +
   cursor). */

void UiCollectionCardMarkCardSeen(UiCollectionCard *self)

{
  SaveProfile *profile = SaveGetProfile();
  GfxSprite **sprites;
  int card = self->slots[self->cursor + self->page * 4];

  profile->data->newItemGroups[0x20 + card / 8] |= (u8)(1 << (card % 8));
  self->isNew[self->cursor + self->page * 4] = 0;
  sprites = (GfxSprite **)self->base.data;
  sprites[9 + self->cursor]->flags &= ~1u;
}
