// bdc 0x0880c7a0 SaveProfileAttachBlock
#include "bdc.h"

/* Stores `block` in `g_saveDataBlock` and points the profile's save-block pointer
   (`g_playerProfile->data`) at `block + ScriptVarsGetProfileOffset()` (= `block + 0x100`), or NULL when `block` is
   NULL. Called by `SaveProfileInit` and `ScriptVarsInit` with the 0xfd0-byte script variable
   block. */

void SaveProfileAttachBlock(void *block)
{
  g_saveDataBlock = block;
  if (g_playerProfile != (SaveProfile *)0x0) {
    if (block == (void *)0x0) {
      g_playerProfile->data = (SaveProfileData *)0x0;
    }
    else {
      g_playerProfile->data = (SaveProfileData *)((u8 *)block + ScriptVarsGetProfileOffset());
    }
  }
}
