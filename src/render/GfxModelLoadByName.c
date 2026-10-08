// bdc 0x089de6e0 GfxModelLoadByName
#include "bdc.h"

/* Looks up the GMO file `name` in the resource table `0x08ac520c` (`CorePackChainFindEntry`) and, if found,
   sets the model up from it (`GfxModelSetup` with the entry's data `+4` and size `+8`,
   `workSize`, and `name`). Returns 1 on success, 0 when the file is not loaded. */

bool GfxModelLoadByName(GfxModel *self, const char *name, u32 workSize)

{
  CorePackDirEntry *entry;

  entry = (CorePackDirEntry *)CorePackChainFindEntry(g_ioLzsPackages, (char *)name);
  if (entry != (CorePackDirEntry *)0x0) {
    GfxModelSetup(self, PspPtr(entry->data), entry->size, workSize, name);
    return true;
  }
  return false;
}
