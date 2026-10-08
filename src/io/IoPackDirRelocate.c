// bdc 0x089bd544 IoPackDirRelocate
#include "bdc.h"

/* Turns the data offsets of a loaded resource-pack directory into pointers (adds the directory base
   to each entry's `data`), recursing into sub-directories (type `0x78`), and returns how many TIM2
   texture entries (type 3) the tree contains. */

int IoPackDirRelocate(u16 *dir)
{
  CorePackDirEntry *base;
  CorePackDirEntry *entry;
  int i;
  int textures;

  base = (CorePackDirEntry *)dir;
  textures = 0;
  entry = base;
  for (i = 0; i < (int)base->count; i++) {
    u16 type = entry->type;
    entry->data = entry->data + PspAddr(base);
    if (type >= 4) {
      if (type == 0x78) {
        textures += IoPackDirRelocate((u16 *)PspPtr(entry->data));
      }
    } else if (type >= 3) {
      textures++;
    }
    entry++;
  }
  return textures;
}
