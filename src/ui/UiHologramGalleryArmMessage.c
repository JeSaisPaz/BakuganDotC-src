// bdc 0x08920c28 UiHologramGalleryArmMessage
#include "bdc.h"

/* Arms the message record of the hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu
   cursor `+0x77`, panel `+0x74`): `+0x22a0 = kind`, `+0x22a1 = 0` (step), `+0x22a3 = a`, `+0x22a4 =
   b`. */

void UiHologramGalleryArmMessage(UiHologramGallery *self, u8 kind, u8 a, u8 b)

{
  self->msgKind = kind;
  self->msgStep = '\0';
  self->msgArgA = a;
  self->msgArgB = b;
  return;
}

