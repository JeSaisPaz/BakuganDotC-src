// bdc 0x088d654c GameGimmickIrSensorDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the IR sensor gimmick (`GameGimmickIrSensorCtor`, vtables
   `0x08af303c`/`0x08af30e4`): frees the beam timer `+0x1f4`, deletes the shape `+0x1b4` and
   collider `+0x174`, runs `GameGimmickDtor` and frees when `flags & 1`. */

void GameGimmickIrSensorDtor(GameGimmickIrSensor *obj, u32 flags)
{
  if (obj != NULL) {
    obj->base.base.base.vtable = g_gameGimmickIrSensorVtbl;
    obj->base.vtbl2 = g_gameGimmickIrSensorVtbl2;
    if (obj->beamTimer != NULL) {
      u8 *ptr = obj->beamTimer;

      MemLock();
      MemFree(ptr, NULL, 0);
      MemUnlock();
      obj->beamTimer = NULL;
    }
    if (obj->shapeOwner != NULL) {
      GameGimmickIrSensorOwned *shape = obj->shapeOwner;
      const VtblEntry *dtor = &shape->vtbl[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)shape + dtor->delta, 3);
      obj->shapeOwner = NULL;
    }
    if (obj->base.attached != NULL) {
      CoreNode *node = (CoreNode *)obj->base.attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      obj->base.attached = NULL;
    }
    GameGimmickDtor(&obj->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, NULL, 0);
      MemUnlock();
    }
  }
}
