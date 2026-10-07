// bdc 0x088ea38c GameEventPropCtor
#include "bdc.h"

/* Constructor of the 0x14-byte event prop: no model (`+0`), scale `+4..+0xc` = 0x1000 (1.0 in
   1/4096), flag `+0x10` = 0. */

void *GameEventPropCtor(void *prop)

{
  GameEventProp *p = prop;
  p->model = 0;
  p->scaleX = 0x1000;
  p->scaleY = 0x1000;
  p->scaleZ = 0x1000;
  p->flag = 0;
  return prop;
}
