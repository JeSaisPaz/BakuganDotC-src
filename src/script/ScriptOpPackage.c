// bdc 0x08a00c74 ScriptOpPackage
#include "bdc.h"

/* Script opcode 0x43 (group 0x40): load/unload/poll one of 14 named `.lzs` script/resource
   packages. Operands u32 `cmd`, u32 `pkg`; when `cmd` is outside 0..2 or `pkg` outside 0..13
   nothing happens and 2 (retry) is returned. cmd 0 destroys the loaded package object in
   `g_scriptPackageSlots[pkg]` (virtual deleting destructor, vtable entry 1, flag 2), clears the
   slot and returns 0. cmd 1 builds the path in `g_scriptPackagePaths[pkg]` from
   `g_scriptPackageFmts``[pkg]` (`sprintf` with the language directory from
   `SaveGetLanguageDirName` for slots 3, 6, 8, 11, 13, else `strcpy`), constructs the package
   object in `g_scriptPackages[pkg]` (`IoLzsPackageCtor`) when the slot is empty, and starts
   loading it with `IoLzsPackageStartLoad(obj, path, 10, g_scriptPackageLoadFlags[pkg], 0)`;
   returns 0 when that returns non-zero, else 2. cmd 2 polls `IoLzsPackagePoll(obj, 1)` and
   returns 0 when it reports the load done, else 2 (also 2 when the slot is empty). */

int ScriptOpPackage(Script *script)

{
  int result;
  int cmd;
  int pkg;
  bool valid;

  result = 2;
  cmd = (int)ScriptReadU32(script);
  pkg = (int)ScriptReadU32(script);
  valid = false;
  if (cmd >= 0 && cmd < 3 && pkg >= 0 && pkg < 14) {
    valid = true;
  }
  if (!valid) {
    return result;
  }
  if (cmd < 1) {
    if (cmd >= 0) {
      IoLzsPackage *pkgObj = g_scriptPackageSlots[pkg];
      if (pkgObj != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)pkgObj->base.vtable)[1];
        ((void (*)(void *, int))dtor->fn)((u8 *)pkgObj + dtor->delta, 2);
        g_scriptPackageSlots[pkg] = NULL;
      }
      result = 0;
    }
  }
  else if (cmd < 2) {
    IoLzsPackage **slot = &g_scriptPackageSlots[pkg];
    char *path = g_scriptPackagePaths[pkg];
    const char *fmt = g_scriptPackageFmts[pkg];
    IoLzsPackage *pkgObj;

    switch (pkg) {
    case 3:
    case 6:
    case 8:
    case 11:
    case 13:
      sprintf(path, fmt, SaveGetLanguageDirName());
      break;
    default:
      strcpy(path, fmt);
      break;
    }
    pkgObj = *slot;
    if (pkgObj == NULL) {
      IoLzsPackage *storage = &g_scriptPackages[pkg];
      pkgObj = NULL;
      if (storage != NULL) {
        IoLzsPackageCtor(storage);
        pkgObj = storage;
      }
      *slot = pkgObj;
    }
    if (IoLzsPackageStartLoad(pkgObj, path, 10, g_scriptPackageLoadFlags[pkg], 0) != 0) {
      result = 0;
    }
  }
  else if (cmd < 3) {
    IoLzsPackage *pkgObj = g_scriptPackageSlots[pkg];
    if (pkgObj != NULL && IoLzsPackagePoll(pkgObj, 1) != 0) {
      result = 0;
    }
  }
  return result;
}
