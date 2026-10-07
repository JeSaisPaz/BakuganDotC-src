// bdc 0x08985d14 UiCollectionCardSetArtPulse
#include "bdc.h"

/* Resets the card-art pulse record of `UiCollectionCard` (`+0xefc`, 0x0c
   bytes: `u8 enabled`, `u8 step`, `u16 timer`), sets `enabled` and hides the pulse sprite (data
   `+0xcc`, visible bit 0 of `+0xd0` cleared). */

void UiCollectionCardSetArtPulse(UiCollectionCard *self, u8 enable)

{
  GfxSprite *pulseSprite;

  memset(&self->previewOn, 0, 0xc);
  self->previewOn = enable;
  pulseSprite = ((GfxSprite **)self->base.data)[0xcc / sizeof(GfxSprite *)];
  pulseSprite->flags = pulseSprite->flags & 0xfffffffe;
}
