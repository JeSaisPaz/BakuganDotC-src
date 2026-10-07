// bdc 0x089881e0 UiCollectionTheaterMarkSceneSeen
#include "bdc.h"

/* Marks the selected scene of `UiCollectionTheater` as viewed: sets its
   bit in the profile array `+0x601`, clears its new flag `sceneNew` and hides its "new" marker
   sprite (data `+0x4c + cursor * 4`). */

void UiCollectionTheaterMarkSceneSeen(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  SaveProfile *profile;
  u8 *bits;
  int id;
  GfxSprite *sprite;

  profile = SaveGetProfile();
  id = self->sceneId[self->cursor + self->page * 6];
  /* profile data +0x601 = newItemGroups[0x2e] */
  bits = &profile->data->newItemGroups[0x2e + id / 8];
  *bits = *bits | (u8)(1 << (id % 8));
  self->sceneNew[self->cursor + self->page * 6] = 0;
  sprite = ((GfxSprite **)self->base.data)[19 + self->cursor];
  sprite->flags &= ~1u;
  return;
}
