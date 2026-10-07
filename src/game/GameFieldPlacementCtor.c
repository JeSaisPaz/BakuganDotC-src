// bdc 0x088f4d54 GameFieldPlacementCtor
#include "bdc.h"

/* Constructor of the runtime placement record of a placed character (0x48 bytes,
   `GameFieldPlacedChar`, vtable `g_gameFieldPlacementVtbl`, stored in the table at
   `g_gameEventLocationBlock`): runs `GameFieldPlacementBaseCtor`, sets flag bit 0 of `flags46`
   and clears its bits 1..7, clears bit 0 and sets bits 1/2 of `flags47`, `eventId`/`id34` = -1,
   `blend` = 2, `mode44` = 6, every other field 0. Returns `rec`. */

void *GameFieldPlacementCtor(void *rec, u8 slot)
{
  GameFieldPlacedChar *p = (GameFieldPlacedChar *)rec;

  GameFieldPlacementBaseCtor(rec, slot);
  p->flags46 = p->flags46 | 1;
  p->flags46 = p->flags46 & ~2;
  p->vtbl = g_gameFieldPlacementVtbl;
  p->work24 = 0;
  p->work28 = 0;
  p->work2c = 0;
  p->work2e = 0;
  p->eventId = -1;
  p->flags46 = p->flags46 & ~4;
  p->work32 = 0;
  p->id34 = -1;
  p->modelCode = 0;
  p->isGuard = 0;
  p->byte38 = 0;
  p->routeMode = 0;
  p->byte3a = 0;
  p->entry = 0;
  p->blend = 2;
  p->flags46 = p->flags46 & ~8;
  p->entryParam3e = 0;
  p->byte3f = 0;
  p->frozenFlag = 0;
  p->byte41 = 0;
  p->entryParam42 = 0;
  p->byte43 = 0;
  p->mode44 = 6;
  p->entryParam45 = 0;
  p->flags46 = p->flags46 & ~0x10;
  p->flags46 = p->flags46 & ~0x20;
  p->flags46 = p->flags46 & ~0x40;
  p->flags47 = p->flags47 & ~1;
  p->flags46 = p->flags46 & ~0x80;
  p->flags47 = p->flags47 | 2;
  p->flags47 = p->flags47 | 4;
  memset(p->work18, 0, 0xc);
  return rec;
}
