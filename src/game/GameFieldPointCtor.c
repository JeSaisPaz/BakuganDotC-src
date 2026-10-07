// bdc 0x088d8d94 GameFieldPointCtor
#include "bdc.h"

/* Constructor of the 0x40-byte field point object: `CoreObjectInit` without chain, vtable
   `g_gameFieldPointVtbl`, clears `effect`, id `-1`, kind `0`, `disabled` `0`. */

CoreObject *GameFieldPointCtor(CoreObject *obj)
{
  GameFieldPoint *point = (GameFieldPoint *)obj;

  CoreObjectInit(obj, (CoreObject *)0);
  point->vtable = g_gameFieldPointVtbl;
  point->effect = (void *)0;
  point->id = -1;
  point->kind = 0;
  point->disabled = 0;
  return obj;
}
