// bdc 0x08973964 UiCollectionMenuUpdateItemBox
#include "bdc.h"

/* Per-frame update of the item-box model of `UiCollectionMenu` while it is
   active (`+0x520`): advances its motion by 0.5 frame and updates it (model vtable slots at
   +0x34/+0x3c). */

typedef struct GfxAdvEntry {
  short adjust;
  short pad;
  void (*fn)(void *self, float frames);
} GfxAdvEntry;

typedef struct GfxUpdEntry {
  short adjust;
  short pad;
  void (*fn)(void *self);
} GfxUpdEntry;

typedef struct GfxModelVt2 {
  char slots[0x30];
  GfxAdvEntry advance;
  GfxUpdEntry update;
} GfxModelVt2;

void UiCollectionMenuUpdateItemBox(UiCollectionMenu *self)

{
  if ((self->itemBoxClosing != 0) && (self->itemBox != (GfxModel *)0x0)) {
    const GfxAdvEntry *a = &((const GfxModelVt2 *)self->itemBox->base.vtable)->advance;
    a->fn((char *)self->itemBox + a->adjust, 0.5f);
    const GfxUpdEntry *u = &((const GfxModelVt2 *)self->itemBox->base.vtable)->update;
    u->fn((char *)self->itemBox + u->adjust);
  }
}
