// bdc 0x08a00504 ScriptOpSprite
#include "bdc.h"

/* Script opcode 0x46 (group 0x40): drives the 2D sprite manager task (core id `0x276a`,
   `UiSpriteMngGet` / `UiSpriteMngExists`). Operands: u32 `cmd`, output ref (via
   `ScriptReadRef`, size 2), an inline string (sprite name, skipped with `ScriptSkipString`),
   then u32 `slot` and 7 u32 operands `x, y, z, rx, ry, rw, rh`. cmd 0 `UiSpriteMngRemove`
   (slot), 1 `UiSpriteMngAdd` (name, x..rh; the new slot index is the result), 2
   `UiSpriteMngSetVisible` (slot, x != 0; result 1), 3 `UiSpriteMngSetRectPos`, 4
   `UiSpriteMngSetPos`, 5 `UiSpriteMngSetRect` (slot, rx..rh), 6 `UiSpriteMngMoveAndOffsetUv`,
   7 `UiSpriteMngMove`, 8 `UiSpriteMngOffsetUv` (slot, rx..rh); any other cmd gives result -1.
   The result is stored through the output ref (when non-NULL) and the opcode returns 0. If the
   manager task is missing, `UiSpriteMngEnsureTask` is called, the output is -1 and the opcode
   returns 2 (retry). */

int ScriptOpSprite(Script *script)

{
  u32 cmd;
  u32 *out;
  const char *name;
  u32 slot;
  u32 x;
  u32 y;
  u32 z;
  u32 rx;
  u32 ry;
  u32 rw;
  u32 rh;
  u32 result;
  int ret;

  cmd = ScriptReadU32(script);
  out = ScriptReadRef(script, 2);
  /* ScriptSkipString leaves the string start in v0; read it before the skip. */
  name = (const char *)script->operand;
  ScriptSkipString(script);
  slot = ScriptReadU32(script);
  x = ScriptReadU32(script);
  y = ScriptReadU32(script);
  z = ScriptReadU32(script);
  rx = ScriptReadU32(script);
  ry = ScriptReadU32(script);
  rw = ScriptReadU32(script);
  rh = ScriptReadU32(script);
  result = 0xffffffff;
  if (!UiSpriteMngExists()) {
    UiSpriteMngEnsureTask();
    ret = 2;
  }
  else {
    switch (cmd) {
    case 0:
      result = UiSpriteMngRemove(UiSpriteMngGet(), slot);
      break;
    case 1:
      result = UiSpriteMngAdd(UiSpriteMngGet(), name, x, y, z, rx, ry, rw, rh);
      break;
    case 2:
      UiSpriteMngSetVisible(UiSpriteMngGet(), slot, x != 0);
      result = 1;
      break;
    case 3:
      result = UiSpriteMngSetRectPos(UiSpriteMngGet(), slot, x, y, z, rx, ry, rw, rh);
      break;
    case 4:
      result = UiSpriteMngSetPos(UiSpriteMngGet(), slot, x, y, z);
      break;
    case 5:
      result = UiSpriteMngSetRect(UiSpriteMngGet(), slot, rx, ry, rw, rh);
      break;
    case 6:
      result = UiSpriteMngMoveAndOffsetUv(UiSpriteMngGet(), slot, x, y, z, rx, ry, rw, rh);
      break;
    case 7:
      result = UiSpriteMngMove(UiSpriteMngGet(), slot, x, y, z);
      break;
    case 8:
      result = UiSpriteMngOffsetUv(UiSpriteMngGet(), slot, rx, ry, rw, rh);
      break;
    }
    ret = 0;
  }
  if (out != NULL) {
    *out = result;
  }
  return ret;
}
