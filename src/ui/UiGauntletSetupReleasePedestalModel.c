// bdc 0x089333ac UiGauntletSetupReleasePedestalModel
#include "bdc.h"

/* Releases the pedestal model `+0x1a84` of `UiGauntletSetup`
   (`CoreObjectDeferDelete(model, 0)`) and clears the pointer. */

void UiGauntletSetupReleasePedestalModel(UiGauntletSetup *self)

{
  if (self->pedestalModel != (CoreObject *)0x0) {
    CoreObjectDeferDelete(self->pedestalModel,0);
    self->pedestalModel = (void *)0x0;
  }
  return;
}

