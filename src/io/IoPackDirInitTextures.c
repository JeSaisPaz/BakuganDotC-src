// bdc 0x089bd600 IoPackDirInitTextures
#include "bdc.h"

/* Walks a relocated resource-pack directory (recursing into type-`0x78` sub-directories) and
   initialises one 0x140-byte texture object of `pkg->textures` per TIM2 entry (type 3) with
   `GfxTextureInitFromTim2``(tex, entryName, data, fromLow)`; `*index` counts the textures
   used. */

void IoPackDirInitTextures(u16 *dir, IoLzsPackage *pkg, u8 fromLow, int *index)
{
  CorePackDirEntry *entry;
  int i;

  entry = (CorePackDirEntry *)dir;
  for (i = 0; i < (int)*dir; i++) {
    u16 type = entry->type;
    if (type < 4) {
      if (type >= 3) {
        GfxTextureInitFromTim2(&pkg->textures[*index], entry->name, entry->data, fromLow);
        *index = *index + 1;
      }
    } else if (type == 0x78) {
      IoPackDirInitTextures((u16 *)entry->data, pkg, fromLow, index);
    }
    entry++;
  }
}
