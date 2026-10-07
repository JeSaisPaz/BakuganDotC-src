// bdc 0x088f40a4 GameFieldSpawnPlacedActor
#include "bdc.h"

/* Creates the runtime placement record for character-set entry `entryIndex` (body `entry`) in slot
   `slot` and spawns its actor: allocates the 0x48-byte record from the low heap
   (`GameFieldPlacementCtor`, stored in slot `slot` of the placement table `g_gameEventLocationBlock`;
   a failed allocation stores NULL and is not checked further), copies position, heading, character code
   (`modelCode`), route mode/index, event id and the other entry bytes, then spawns the actor
   (`ActorSpawnFromPlacement`) — or, when `reuse` is set and the code is 0xc (the player), rebinds the
   existing actor (`ActorRebindPlacement`) — stores it in `mgr->actors[slot]` and calls its virtual
   slot 13. */

/* Body of a 0x2c-byte character-set entry (the record starts 4 bytes earlier). */
typedef struct CharSetEntryBody {
  s32 pos[3];       /* +0x00 position (20.12) */
  s16 heading;      /* +0x0c binary angle */
  s16 eventId;      /* +0x0e */
  u16 param10;      /* +0x10 low byte -> entryParam45 */
  u8 code;          /* +0x12 character code */
  u8 frozen;        /* +0x13 -> entryParam3e / frozenFlag */
  u8 routeMode;     /* +0x14 */
  u8 routeIndex;    /* +0x15 */
  u8 _unk16;
  u8 param17;       /* +0x17 -> entryParam42 */
} CharSetEntryBody;

void GameFieldSpawnPlacedActor(void *mgr, u8 slot, u8 entryIndex, void *entry, s8 reuse)

{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  const CharSetEntryBody *e = (const CharSetEntryBody *)entry;
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  GameFieldPlacedChar *placed;
  GameFieldPlacedChar *rec;
  GameFieldPlacement *actor;
  const VtblEntry *ve;
  bool fromLow;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rec = (GameFieldPlacedChar *)MemAlloc(0x48, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  placed = NULL;
  if (rec != NULL) {
    GameFieldPlacementCtor(rec, slot);
    placed = rec;
  }
  table[slot] = placed;
  placed->modelCode = e->code;
  table[slot]->pos[0] = e->pos[0];
  table[slot]->pos[1] = e->pos[1];
  table[slot]->pos[2] = e->pos[2];
  table[slot]->rot[0] = 0;
  table[slot]->rot[1] = e->heading;
  table[slot]->rot[2] = 0;
  table[slot]->routeMode = e->routeMode;
  table[slot]->eventId = e->eventId;
  table[slot]->routeIndex = e->routeIndex;
  table[slot]->entry = entryIndex;
  table[slot]->entryParam3e = e->frozen;
  table[slot]->frozenFlag = e->frozen;
  placed = table[slot];
  placed->isGuard = (u8)GameFieldCharSetIsCodeInRange2d(mgr, e->code);
  table[slot]->entryParam42 = e->param17;
  table[slot]->entryParam45 = (u8)e->param10;
  placed = table[slot];
  if (reuse != 0 && e->code == 0xc) {
    actor = (GameFieldPlacement *)ActorRebindPlacement(slot, (s32 *)placed);
  }
  else {
    actor = (GameFieldPlacement *)ActorSpawnFromPlacement(slot, (s32 *)placed);
  }
  set->actors[slot] = actor;
  ve = &actor->vtbl[13];
  ((void (*)(void *))ve->fn)((u8 *)actor + ve->delta);
  return;
}
