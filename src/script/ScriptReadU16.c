// bdc 0x089c9cdc ScriptReadU16
#include "bdc.h"

/* Operand reader of the script VM: fetches the next 16-bit operand at the script's operand pointer
   (`script+0x30`), advances the pointer by 2 and bumps the operand index (`script+0x44`). If bit
   `index` of the operand-mask `script+0x42` is set the value is a variable reference instead of an
   immediate: ids `< 0x1000` index the script-local variable table (`script+0x34`, u32 slots), ids
   `>= 0x1000` index the global variable table `*0x08ac58c4` at `id-0x1000`; the variable's low 16
   bits are returned. */

u32 ScriptReadU16(Script *script)

{
  u8 idx;
  const u8 *p;
  u32 val;

  idx = script->operandIdx;
  p = script->operand;
  val = ((u32)p[0] | (u32)p[1] << 8) & 0xffff;
  script->operand = (u8 *)p + 2;
  if ((script->refMask & (1 << (idx & 0x1f))) != 0) {
    if (val >= 0x1000) {
      u32 g = g_scriptGlobalVars[(val - 0x1000) & 0xffff];
      script->operandIdx = idx + 1;
      return g & 0xffff;
    }
    val = script->vars[val] & 0xffff;
  }
  script->operandIdx = idx + 1;
  return val;
}
