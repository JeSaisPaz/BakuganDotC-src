// bdc 0x08931e24 UiGauntletSetupUpdateModels
#include "bdc.h"

/* Per-frame update of the model objects `+0x1a80` and `+0x1af0` of
   `UiGauntletSetup`: calls the virtual update (vtable `+0x14`, slot at
   `+0x3c`) of each one that exists. */

void UiGauntletSetupUpdateModels(UiGauntletSetup *self)

{
  GfxModel *model = (GfxModel *)self->bakuganModel;

  if (model != NULL) {
    const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];
    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
  }
  model = (GfxModel *)self->playerModel;
  if (model != NULL) {
    const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];
    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
  }
  return;
}
