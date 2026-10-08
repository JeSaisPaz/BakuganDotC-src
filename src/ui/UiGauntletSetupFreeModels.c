// bdc 0x08931b78 UiGauntletSetupFreeModels
#include "bdc.h"

/* Deletes the three model objects of `UiGauntletSetup` (`+0x1a80`, `+0x1a84`,
   `+0x1af0`) through their virtual destructor (vtable at `+0x14`, slot at `+0xc`, flag 3) and
   clears the pointers. */

static void FreeModel(void *obj)
{
  const VtblEntry *e = (const VtblEntry *)((CoreObject *)obj)->vtable + 1;
  ((void (*)(void *, int))e->fn)((char *)obj + e->delta, 3);
}

void UiGauntletSetupFreeModels(UiGauntletSetup *self)
{
  if (self->bakuganModel != NULL) {
    FreeModel(self->bakuganModel);
    self->bakuganModel = NULL;
  }
  if (self->pedestalModel != NULL) {
    FreeModel(self->pedestalModel);
    self->pedestalModel = NULL;
  }
  if (self->playerModel != NULL) {
    FreeModel(self->playerModel);
    self->playerModel = NULL;
  }
}
