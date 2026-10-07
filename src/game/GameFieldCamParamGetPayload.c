// bdc 0x08a2c0a0 GameFieldCamParamGetPayload
#include "bdc.h"

/* Vtable `0x08af6de0` entry 2 (`+0x14`) of the field camera quest parameter object
   (`GameFieldCamParamDtor`): returns the payload `obj + 0x10`. */

void *GameFieldCamParamGetPayload(GameFieldCamParam *obj)
{
  return obj->payload;
}
