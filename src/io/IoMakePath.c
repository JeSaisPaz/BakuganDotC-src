// bdc 0x089fb580 IoMakePath
#include "bdc.h"

/* Builds a full file path: copies the storage-root prefix selected by the current mode
   (`g_ioStorageRoot` via `IoGetStorageRoot`) from the table at `0x08ac61b0` into `out` and
   appends `name`. The table has 7 entries: `""`, `"disc0:/PSP_GAME/USRDIR/"`,
   `"ms0:/PSP/SAVEDATA/ULES01466/"`, `"host0:../../disc/USRDIR/"`, `"ms0:/PSP/SAVEDATA/ULES01466"`,
   `"host0:../../disc/INSDIR/"`, `"disc0:/PSP_GAME/INSDIR/"`; the index (`g_ioStorageRoot`) is 1
   in the image and never written, so only the `disc0:` USRDIR root is used. */

void IoMakePath(const char *name, char *out)

{
  s32 root;
  
  root = IoGetStorageRoot();
  strcpy(out,g_ioStorageRootNames[root]);
  strcat(out,name);
  return;
}

