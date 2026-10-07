// bdc 0x0892967c UiHologramViewFinish
#include "bdc.h"

/* Ends the hologram detail view (`UiHologramViewCtor`, task 392; view kind `+0x485`): sets the
   menu result 0 (`UiSetMenuResult`) and marks view kind `kind` as seen in profile word `+0x82`
   (bit `kind`). */

void UiHologramViewFinish(UiHologramView *self)

{
  SaveProfile *profile;

  UiSetMenuResult(&self->base,0);
  profile = SaveGetProfile();
  profile->data->viewSeenMask = profile->data->viewSeenMask | (u16)(1 << (self->kind & 0x1f));
}
