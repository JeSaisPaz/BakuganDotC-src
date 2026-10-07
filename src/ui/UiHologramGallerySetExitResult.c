// bdc 0x0891ee88 UiHologramGallerySetExitResult
#include "bdc.h"

/* Sets the menu result of the hologram gallery screen (`UiHologramGalleryCtor`, task 391).
   With `exitMode` 0 the result comes from the menu cursor: 0 → result 0, 1 → result 1,
   2 → commits the Bakugan loadout (`UiBakuganCommitLoadout`), marks the current Bakugan seen
   (`ownedItems[11 + id / 8]` bit), clears `viewSeenMask` bit 6, commits the points
   (`UiHologramGallerySaveOrRevert`), clears bit 0 of profile word 0x30 and sets result 2,
   or 3 when profile word 0x2e is nonzero; other cursor values do nothing. With `exitMode` 1
   it sets result 4 and reverts the points; other modes do nothing. */

void UiHologramGallerySetExitResult(UiHologramGallery *self)
{
  if (self->exitMode == 0) {
    s8 cursor = self->menuCursor;
    if (cursor < 1) {
      if (cursor >= 0) {
        UiSetMenuResult(&self->base, 0);
      }
    } else if (cursor < 2) {
      UiSetMenuResult(&self->base, 1);
    } else if (cursor < 3) {
      SaveProfile *profile;
      SaveProfile *cur;
      s32 id;

      UiBakuganCommitLoadout();
      profile = SaveGetProfile();
      cur = SaveGetProfile();
      id = cur->data->curBakugan;
      profile->data->ownedItems[11 + id / 8] |= (u8)(1 << (id % 8));
      SaveGetProfile()->data->viewSeenMask &= 0xffbf;
      UiHologramGallerySaveOrRevert(self, true);
      SaveProfileModifyWord30Bits(0, 1);
      if (SaveProfileGetWord(SaveGetProfile(), 0x2e) == 0) {
        UiSetMenuResult(&self->base, 2);
      } else {
        UiSetMenuResult(&self->base, 3);
      }
    }
  } else if (self->exitMode < 2) {
    UiSetMenuResult(&self->base, 4);
    UiHologramGallerySaveOrRevert(self, false);
  }
}
