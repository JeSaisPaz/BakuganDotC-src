// bdc 0x0892eb34 UiBakuganSelectHideCursors
#include "bdc.h"

/* Hides the cursor sprites 0x19 and 0x84 of the Bakugan select screen (`UiBakuganSelectCtor`,
   task 371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries). */

void UiBakuganSelectHideCursors(UiBakuganSelect *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[0x19]->flags &= ~1u;
  sprites[0x84]->flags &= ~1u;
  return;
}

