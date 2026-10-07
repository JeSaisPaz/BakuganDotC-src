// bdc 0x0881d590 GfxEffectFindKey
#include "bdc.h"

/* Looks up the command block for key `key` in an effect's definition (`effect+0x218`): the
   definition starts with a count word followed by 4-byte index entries `{u16 wordOffset; u16 key}`;
   returns `def + wordOffset*4` for the first entry whose key matches, or NULL. */

void *GfxEffectFindKey(GfxEffect *effect, u32 key)

{
  u32 *def = (u32 *)effect->def;
  s32 i;

  for (i = 0; i < (s32)def[0]; i++) {
    const u16 *entry = (const u16 *)&def[i];
    if (key == entry[3]) {
      return &def[entry[2]];
    }
  }
  return NULL;
}
