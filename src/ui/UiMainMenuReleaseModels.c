// bdc 0x089a3c4c UiMainMenuReleaseModels
#include "bdc.h"

/* Deletes the main menu's 3D models through their virtual destructors (flag 3): the
   `menu_daiza.gmo` base `baseModel` and the five item models `models[]`, clearing the pointers. */

typedef struct MenuModelVt {
  u8 pad[8];
  s16 adj;
  s16 pad2;
  void (*dtor)(void *obj, int flags);
} MenuModelVt;

typedef struct MenuModelObj {
  u8 pad[0x14];
  MenuModelVt *vt;
} MenuModelObj;

static void MenuModelDelete(MenuModelObj *obj)
{
  MenuModelVt *vt = obj->vt;

  vt->dtor((char *)obj + vt->adj, 3);
}

void UiMainMenuReleaseModels(UiMainMenu *self)
{
  int i;

  if (self->baseModel != (void *)0x0) {
    MenuModelDelete((MenuModelObj *)self->baseModel);
    self->baseModel = (void *)0x0;
  }
  for (i = 0; i < 5; i++) {
    if (self->models[i] != (void *)0x0) {
      MenuModelDelete((MenuModelObj *)self->models[i]);
      self->models[i] = (void *)0x0;
    }
  }
}
