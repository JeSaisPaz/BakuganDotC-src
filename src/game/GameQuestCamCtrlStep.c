// bdc 0x088fa198 GameQuestCamCtrlStep
#include "bdc.h"

/* Per-frame step of the quest-field camera controller (field camera `+0x5c4`, see
   `GameFieldCameraReset`; tables from `GameQuestParsePathTable`/`GameQuestParseCamTable`):
   refreshes the nearest path node (`GameQuestPathFindNearestNode`); when both modes exist, copies
   mode `+0x38`'s vector (slot `+0x38`) into the look spring's `lookParams` (`+0x70`), runs slot `+0x28`
   (update, with `dt`) then slot `+0x30` (post-update) on look and mode, copies their positions
   (slot `+0x10`) into `eye` (from mode) and `lookAt` (from look), then clears the snap flag `+0x4a`. */

void GameQuestCamCtrlStep(float dt, GameQuestCamCtrl *self)

{
  GameQuestCamLookSpring *look;
  GameQuestCamSpring *obj;
  const ScePspFVector4 *src;
  ScePspFVector4 point __attribute__((aligned(16)));

  GameQuestPathFindNearestNode(self->nodeCursor);
  if ((self->look != NULL) && (self->mode != NULL)) {
    look = (GameQuestCamLookSpring *)self->look;
    obj = self->mode;
    ((void (*)(void *, ScePspFVector4 *))obj->vtbl[7].fn)((u8 *)obj + obj->vtbl[7].delta, &point);
    look->lookParams = point;
    obj = self->look;
    ((void (*)(float, void *))obj->vtbl[5].fn)(dt, (u8 *)obj + obj->vtbl[5].delta);
    obj = self->mode;
    ((void (*)(float, void *))obj->vtbl[5].fn)(dt, (u8 *)obj + obj->vtbl[5].delta);
    obj = self->look;
    ((void (*)(float, void *))obj->vtbl[6].fn)(dt, (u8 *)obj + obj->vtbl[6].delta);
    obj = self->mode;
    ((void (*)(float, void *))obj->vtbl[6].fn)(dt, (u8 *)obj + obj->vtbl[6].delta);
    obj = self->mode;
    src = ((const ScePspFVector4 *(*)(void *))obj->vtbl[2].fn)((u8 *)obj + obj->vtbl[2].delta);
    self->eye = *src;
    obj = self->look;
    src = ((const ScePspFVector4 *(*)(void *))obj->vtbl[2].fn)((u8 *)obj + obj->vtbl[2].delta);
    self->lookAt = *src;
    self->snap = 0;
  }
  return;
}
