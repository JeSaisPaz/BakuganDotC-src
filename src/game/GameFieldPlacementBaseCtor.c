// bdc 0x088f4c9c GameFieldPlacementBaseCtor
#include "bdc.h"

/* Base constructor of the 0x48-byte runtime placement record (vtable `0x08af43b4`): clears the
   position `+0..+0xb` and angles `+0xc..+0x11`, `+0x12 = 0`, slot `+0x13 = slot`. */

void *GameFieldPlacementBaseCtor(void *rec, u8 slot)
{
  GameFieldPlacementBase *p = (GameFieldPlacementBase *)rec;

  p->vtbl = g_gameFieldPlacementBaseVtbl;
  p->pad12 = 0;
  p->slot = slot;
  memset(p->pos, 0, 0xc);
  memset(p->angles, 0, 6);
  return rec;
}
