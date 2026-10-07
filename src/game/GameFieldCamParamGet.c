// bdc 0x08a2c160 GameFieldCamParamGet
#include "bdc.h"

/* Returns the payload of a field camera parameter object by calling its virtual entry 2 (vtable at
   `+4`, entry `+0x10` with its this-delta); for the derived class (`GameFieldCamParamDtor`)
   that is `obj + 0x10`. */

void *GameFieldCamParamGet(GameFieldCamParam *obj)
{
  const GameFieldCamParamVEntry *e = &obj->vtbl->getPayload;

  return e->fn((u8 *)obj + e->thisDelta);
}
