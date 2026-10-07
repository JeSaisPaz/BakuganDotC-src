// bdc 0x0880f1d8 ScriptOpJumpIfAllOpponentsDead
#include "bdc.h"

/* Conditional jump when every opponent is dead: walks the battle unit list (`BtlGetBakuganList`)
   except the player's unit (`BtlGetPlayerBakugan`), skipping target points (virtual `+0x74`, see
   `BtlHudGetEnemyUnit`) and units whose virtual `+0x5c` is true (0 in the Bakugan base classes);
   when every remaining unit has its `BtlCombatState` `dead` byte (`+0x4c1`) set (or none remain)
   the track pc is set to the u16 `target` and 3 is returned; otherwise 0. */

int ScriptOpJumpIfAllOpponentsDead(Script *script)

{
  u32 target;
  CoreObjectList *list;
  BtlBakugan *player;
  BtlBakugan *unit;
  const VtblEntry *vtbl;
  int deadCount;
  int count;

  deadCount = 0;
  target = ScriptReadU16(script);
  list = (CoreObjectList *)BtlGetBakuganList();
  unit = NULL;
  player = (BtlBakugan *)BtlGetPlayerBakugan();
  if (list != NULL) {
    unit = (BtlBakugan *)list->head;
  }
  count = 0;
  while (unit != NULL) {
    if (unit != player) {
      vtbl = (const VtblEntry *)unit->base.base.vtable;
      if (((s32 (*)(void *))vtbl[14].fn)((u8 *)unit + vtbl[14].delta) == 0) {
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        if (((s32 (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) == 0) {
          if (unit->combat.dead != 0) {
            deadCount++;
          }
          count++;
        }
      }
    }
    unit = (BtlBakugan *)unit->base.base.next;
  }
  if (count != deadCount) {
    return 0;
  }
  script->curTrack->pc = (u16)target;
  return 3;
}
