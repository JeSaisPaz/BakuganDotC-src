// bdc 0x089de6e0 GfxModelLoadByName
#include "bdc.h"

/* Looks up the GMO file `name` in the resource table `0x08ac520c` (`CorePackChainFindEntry`) and, if found,
   sets the model up from it (`GfxModelSetup` with the entry's data `+4` and size `+8`,
   `workSize`, and `name`). Returns 1 on success, 0 when the file is not loaded. */

/* View of the 0x40-byte directory entry: data pointer `+4`, size `+8`. */
typedef struct GfxLoadEntryView {
  u32 unk00;
  void *data;
  u32 size;
} GfxLoadEntryView;

bool GfxModelLoadByName(GfxModel *self, const char *name, u32 workSize)

{
  GfxLoadEntryView *entry;

  entry = (GfxLoadEntryView *)CorePackChainFindEntry(g_ioLzsPackages, (char *)name);
  if (entry != (GfxLoadEntryView *)0x0) {
    GfxModelSetup(self, entry->data, entry->size, workSize, name);
    return true;
  }
  return false;
}

