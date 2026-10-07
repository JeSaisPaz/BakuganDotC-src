// bdc 0x0892b37c UiBakuganSelectInitState
#include "bdc.h"

/* Class init of the Bakugan select screen (`UiBakuganSelectCtor`, task 371; cursor `+0x74`,
   current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries): puts the cursor
   `+0x74`/`+0x75` on the grid position of the profile's current Bakugan (`+0x48c`,
   `UiBakuganListOrder`), clears the camera/model pointers `+0x1cf4..+0x1cfc`, remembers the id in
   `+0x1b7e`, builds the owned list (`UiBakuganSelectBuildOwnedList`) and clears the animation
   records `+0x1c94..`. */

void UiBakuganSelectInitState(UiBakuganSelect *self)

{
  SaveProfile *profile = SaveGetProfile();
  self->cursor = UiBakuganListOrder(self, false, (u32)profile->data->curBakugan & 0xff);
  self->camera = NULL;
  self->current = self->cursor;
  self->model = NULL;
  self->pedestal = NULL;
  profile = SaveGetProfile();
  self->currentPos = (u8)profile->data->curBakugan;
  UiBakuganSelectBuildOwnedList(self);
  memset(&self->gaugeOn, 0, 0x24);
  memset(&self->namePanelOn, 0, 4);
  memset(&self->arrowsOn, 0, 4);
}
