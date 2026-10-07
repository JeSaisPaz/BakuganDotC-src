// bdc 0x088fa394 GameQuestCamCtrlDtor
#include "bdc.h"

/* Destructor of the quest-field camera controller (field camera `+0x5c4`, see
   `GameFieldCameraReset`; tables from `GameQuestParsePathTable`/`GameQuestParseCamTable`)
   (`g_questCamCtrlVtbl` slot 1): deletes both mode objects `look`/`mode` through their virtual
   destructors, runs `GameQuestCamCtrlBaseDtor` and frees the object when bit 0 of `flags`
   is set. */

void GameQuestCamCtrlDtor(GameQuestCamCtrl *self, u32 flags)

{
  GameQuestCamSpring *obj;

  if (self != (GameQuestCamCtrl *)0x0) {
    obj = self->look;
    self->vtbl = g_questCamCtrlVtbl;
    if (obj != (GameQuestCamSpring *)0x0) {
      ((void (*)(void *, int))obj->vtbl[1].fn)((u8 *)obj + obj->vtbl[1].delta, 3);
      self->look = (GameQuestCamSpring *)0x0;
    }
    obj = self->mode;
    if (obj != (GameQuestCamSpring *)0x0) {
      ((void (*)(void *, int))obj->vtbl[1].fn)((u8 *)obj + obj->vtbl[1].delta, 3);
      self->mode = (GameQuestCamSpring *)0x0;
    }
    GameQuestCamCtrlBaseDtor(self, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (const char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
