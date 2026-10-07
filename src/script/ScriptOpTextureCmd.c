// bdc 0x08a007d8 ScriptOpTextureCmd
#include "bdc.h"

/* Script opcode 0x47 (group 0x40): looks up a texture by the inline name string (`GfxFindTexture`)
   and applies a setting: u16 `cmd`, inline string, u16 `value`. cmd 0 calls
   `GfxSetTextureSlotA(tex, value)`, cmd 1 `GfxSetTextureSlotB(tex, value)`. Always returns 0. */

int ScriptOpTextureCmd(Script *script)

{
  u32 cmd;
  char *name;
  u32 slot;
  void *tex;
  
  cmd = ScriptReadU16(script);
  name = (char *)script->operand;
  ScriptSkipString(script);
  slot = ScriptReadU16(script);
  tex = GfxFindTexture(name);
  if (tex != (void *)0x0) {
    if ((int)cmd < 1) {
      if (-1 < (int)cmd) {
        GfxSetTextureSlotA(tex,slot);
      }
    }
    else if ((int)cmd < 2) {
      GfxSetTextureSlotB(tex,slot);
    }
  }
  return 0;
}

