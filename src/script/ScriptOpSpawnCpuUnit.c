// bdc 0x0880edf4 ScriptOpSpawnCpuUnit
#include "bdc.h"

/* Script opcode that creates a CPU battle unit: operands u16 `type`, output ref, u16 `allyIndex`
   (stored in `BtlCpuUnit``.allyIndex`), u32 `kind` (1..0x20, else nothing happens), 4 floats
   (x, y, z, yaw in degrees, converted to radians), u16 `aiLevel`. Creates the unit with
   `BtlCreateBakugan` (mode 4 when `type` is 1, else 2) and stores it in the output ref, adds its
   HP gauge, sets `playerSlot = 4` and collider layer 5, applies
   `BtlBakuganSetAiLevel``(unit, aiLevel)`, arms the ball entry (`BtlBakuganBeginBallEntry`)
   unless `type` is 2, and for `type` 1 clears the input's `allowedActions` and sets level 0 (kind
   0x15 also gets fixed HP 60 via `BtlCombatFillHp`, `spawnerId = 0` and `allowedActions =
   0xffffbff3`). Ends with the unit's virtual entry 7 (vtable `+0x38`, init). Returns 0. */

int ScriptOpSpawnCpuUnit(Script *script)

{
  s16 type;
  u32 *outRef;
  s16 allyIndex;
  int kind;
  s16 aiLevel;
  int mode;
  BtlCpuUnit *unit;
  const VtblEntry *init;
  float spawn[4] __attribute__((aligned(16)));

  type = (s16)ScriptReadU16(script);
  outRef = ScriptReadRef(script, 2);
  allyIndex = (s16)ScriptReadU16(script);
  kind = (int)ScriptReadU32(script);
  spawn[2] = 0.0f;
  spawn[1] = 0.0f;
  spawn[0] = 0.0f;
  mode = 2;
  spawn[3] = 0.0f;
  spawn[0] = ScriptReadFloat(script);
  spawn[1] = ScriptReadFloat(script);
  spawn[2] = ScriptReadFloat(script);
  spawn[3] = ScriptReadFloat(script) * 0.0174532924f;
  aiLevel = (s16)ScriptReadU16(script);
  if (kind < 1 || kind > 0x20) {
    return 0;
  }
  if (type == 1) {
    mode = 4;
  }
  unit = (BtlCpuUnit *)BtlCreateBakugan(kind, mode, spawn);
  /* the script variable receives the unit as a 32-bit handle (PSP pointer width) */
  *outRef = PspAddr(unit);
  unit->allyIndex = allyIndex;
  BtlBakuganCreateHpGauge(&unit->base);
  unit->base.playerSlot = 4;
  BtlBakuganSetColliderLayer(&unit->base, 5);
  BtlBakuganSetAiLevel(&unit->base, aiLevel);
  if (type != 2) {
    BtlBakuganBeginBallEntry(&unit->base, 1);
  }
  if (type == 1) {
    unit->base.input->allowedActions = 0;
    BtlCombatSetLevel(&unit->base.combat, 0);
    if (kind == 0x15) {
      BtlCombatFillHp(&unit->base.combat, 60.0f);
      unit->spawnerId = 0;
      unit->base.input->allowedActions = 0xffffbff3;
    }
  }
  init = &((const VtblEntry *)unit->base.base.base.vtable)[7];
  ((void (*)(void *))init->fn)((u8 *)unit + init->delta);
  return 0;
}
