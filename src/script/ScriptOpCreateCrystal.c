// bdc 0x08810610 ScriptOpCreateCrystal
#include "bdc.h"

/* Script opcode: creates a battle crystal actor (`ActorCrystalCtor`, model `fz_crystal01.gmo`,
   0xa90 bytes, allocated from the low end of the heap) at a position/yaw taken from the operands,
   sets its player slot and collider layer to 5 and stores a fire type clamped to 0..5. If the
   allocation fails it still writes through a NULL actor (as the binary does). Returns 0.
   The x/y/z/yaw operands are copied into a separate argument vector for the constructor. */

int ScriptOpCreateCrystal(Script *script)
{
  bool fromLow;
  u32 type;
  u32 flag;
  u32 fireTypeRaw;
  u32 style;
  ActorCrystal *mem;
  ActorCrystal *crystal;
  float level;
  s32 fireType;
  float pos[4];
  float argPos[4];

  type = ScriptReadU32(script);
  flag = ScriptReadU32(script);
  pos[2] = 0.0f;
  pos[1] = 0.0f;
  pos[0] = 0.0f;
  pos[3] = 0.0f;
  pos[0] = ScriptReadFloat(script);
  pos[1] = ScriptReadFloat(script);
  pos[2] = ScriptReadFloat(script);
  pos[3] = ScriptReadFloat(script) * 0.0174532924f;
  fireTypeRaw = ScriptReadU16(script);
  style = ScriptReadU16(script);
  crystal = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(ActorCrystal), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    argPos[0] = pos[0];
    argPos[1] = pos[1];
    argPos[2] = pos[2];
    argPos[3] = pos[3];
    ActorCrystalCtor(mem, 0x21, 2, argPos, flag, type, style);
    crystal = mem;
  }
  crystal->base.playerSlot = 5;
  BtlBakuganSetColliderLayer(&crystal->base, 5);
  level = (float)(s32)fireTypeRaw;
  if (level < 0.0f) {
    fireType = 0;
  } else if (!(level <= 5.0f)) {
    fireType = 5;
  } else {
    fireType = (s32)level;
  }
  crystal->fireType = fireType;
  return 0;
}
