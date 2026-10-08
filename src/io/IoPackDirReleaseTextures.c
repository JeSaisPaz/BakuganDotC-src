// bdc 0x089bdefc IoPackDirReleaseTextures
#include "bdc.h"

/* Reverse of `IoLzsPackageRegister`: walks the directory (recursing into sub-directories, type
   `0x78`), destroys the package texture of each TIM2 entry (type 3) through its virtual destructor
   (vtable slot 1, flag 2 = don't free) when `pkg->textures` exists, converts every entry's data
   pointer back into an offset from the directory base and clears `pkg->registered`. */

void IoPackDirReleaseTextures(IoLzsPackage *pkg, u16 *dir, int *index)
{
  CorePackDirEntry *entry;
  int i;

  entry = (CorePackDirEntry *)dir;
  for (i = 0; i < (int)*dir; i++) {
    u16 type = entry->type;
    if (type < 4) {
      if (type >= 3 && pkg->textures != NULL) {
        GfxTexture *tex = &pkg->textures[*index];
        const VtblEntry *slot = &tex->vtbl[1];
        ((void (*)(void *, int))slot->fn)((u8 *)tex + slot->delta, 2);
        *index = *index + 1;
      }
    } else if (type == 0x78) {
      IoPackDirReleaseTextures(pkg, (u16 *)PspPtr(entry->data), index);
    }
    entry->data = entry->data - PspAddr(dir);
    entry++;
  }
  pkg->registered = 0;
}
