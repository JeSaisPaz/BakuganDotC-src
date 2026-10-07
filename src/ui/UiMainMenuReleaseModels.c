// bdc 0x089a3c4c UiMainMenuReleaseModels
#include "bdc.h"

/* Deletes the main menu's 3D models through their virtual destructors (flag 3): the
   `menu_daiza.gmo` base `baseModel` and the five item models `models[]`, clearing the pointers. */

static void MenuModelDelete(GfxModel *obj)
{
  const VtblEntry *dtor = &((const VtblEntry *)obj->base.vtable)[1];

  ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
}

void UiMainMenuReleaseModels(UiMainMenu *self)
{
  int i;

  if (self->baseModel != (void *)0x0) {
    MenuModelDelete((GfxModel *)self->baseModel);
    self->baseModel = (void *)0x0;
  }
  for (i = 0; i < 5; i++) {
    if (self->models[i] != (void *)0x0) {
      MenuModelDelete((GfxModel *)self->models[i]);
      self->models[i] = (void *)0x0;
    }
  }
}
