// bdc 0x08933374 UiGauntletSetupReleaseBakuganModel
#include "bdc.h"

/* Releases the Bakugan model `+0x1a80` of `UiGauntletSetup`
   (`CoreObjectDeferDelete(model, 0)`) and clears the pointer. */

void UiGauntletSetupReleaseBakuganModel(UiGauntletSetup *self)

{
  if (self->bakuganModel != (CoreObject *)0x0) {
    CoreObjectDeferDelete(self->bakuganModel,0);
    self->bakuganModel = (void *)0x0;
  }
  return;
}

