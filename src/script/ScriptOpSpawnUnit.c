// bdc 0x0880ec00 ScriptOpSpawnUnit
#include "bdc.h"

/* Script opcode that creates a battle unit with `BtlCreateBakugan``(kind, mode, &pos)` and writes
   it to the output ref. Operands: u16 `type` (1 forces mode 4), output ref, u32 `kind`, u32 `mode`,
   4 floats (x, y, z, yaw in degrees). `kind == 0` spawns the player: kind = profile word 3 (the
   current Bakugan, default 1; 1 in mode 4), `isPlayer = 1`, `targetPointVariant = 0`, slot 0,
   collider layer 1 (`BtlBakuganSetColliderLayer`), with `type` 1 level 4 (`BtlCombatSetLevel`)
   and `keepHpFull = 1` when virtual slot 18 (`+0x90`) returns nonzero, and its input disabled when
   the camera task exists and `g_btlControlLockApplied` is set. Any other kind gets slot 4 and
   layer 5. Always adds the HP gauge (`BtlBakuganCreateHpGauge`) and calls the unit's virtual
   slot 7 (`+0x38`, init). Returns 0. */

int ScriptOpSpawnUnit(Script *script)
{
  short type;
  u32 *out;
  u32 kind;
  u32 mode;
  bool isPlayer;
  BtlBakugan *unit;
  const VtblEntry *entry;
  float pos[4] __attribute__((aligned(16)));

  type = (short)ScriptReadU16(script);
  out = ScriptReadRef(script, 2);
  kind = ScriptReadU32(script);
  mode = ScriptReadU32(script);
  pos[2] = 0.0f;
  pos[1] = 0.0f;
  pos[0] = 0.0f;
  isPlayer = false;
  pos[3] = 0.0f;
  pos[0] = ScriptReadFloat(script);
  pos[1] = ScriptReadFloat(script);
  pos[2] = ScriptReadFloat(script);
  pos[3] = ScriptReadFloat(script) * 0.017453292f;
  if (type == 1) {
    mode = 4;
  }
  if (kind == 0) {
    kind = SaveProfileGetWord(SaveGetProfile(), 3);
    if (kind == 0) {
      kind = 1;
    }
    if (mode == 4) {
      kind = 1;
    }
    isPlayer = true;
  }
  unit = (BtlBakugan *)BtlCreateBakugan((int)kind, (int)mode, pos);
  *out = (u32)(uintptr_t)unit; /* PSP: 32-bit script variable holds a pointer; port handle needed */
  BtlBakuganCreateHpGauge(unit);
  if (isPlayer) {
    unit->isPlayer = 1;
    unit->targetPointVariant = 0;
    unit->playerSlot = 0;
    BtlBakuganSetColliderLayer(unit, 1);
    if (type == 1) {
      BtlCombatSetLevel(&unit->combat, 4);
      entry = &((const VtblEntry *)unit->base.base.vtable)[18];
      if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
        ((BtlUnitMode4 *)unit)->keepHpFull = 1;
      }
    }
  }
  else {
    unit->playerSlot = 4;
    BtlBakuganSetColliderLayer(unit, 5);
  }
  if (isPlayer && BtlCameraTaskExists() != 0 && g_btlControlLockApplied != 0) {
    unit->input->disabled = 1;
  }
  entry = &((const VtblEntry *)unit->base.base.vtable)[7];
  ((void (*)(void *))entry->fn)((u8 *)unit + entry->delta);
  return 0;
}
