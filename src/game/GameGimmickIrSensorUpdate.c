// bdc 0x088d6660 GameGimmickIrSensorUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the IR sensor gimmick (`GameGimmickIrSensorCtor`, vtables
   `0x08af303c`/`0x08af30e4`): `GameGimmickUpdate`, then for states 0..2 (`+0x16c`) the state
   handler through virtual slot 20 (`+0xa4`). */

void GameGimmickIrSensorUpdate(GameGimmickIrSensor *obj)
{
  const VtblEntry *vt;
  int state;

  GameGimmickUpdate(&obj->base);
  state = obj->base.state;
  if (state >= 0 && state < 3) {
    vt = &((const VtblEntry *)obj->base.base.base.vtable)[20];
    ((void (*)(void *))vt->fn)((u8 *)obj + vt->delta);
  }
}
