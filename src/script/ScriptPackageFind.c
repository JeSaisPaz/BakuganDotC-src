// bdc 0x089c9bb0 ScriptPackageFind
#include "bdc.h"

/* Finds a script in the loaded package by exact name and returns its decoded payload. Walks the
   `count` 0x40-byte entries of `g_scriptPackage` comparing the name at entry `+8` with `strcmp`;
   on a match it allocates (from the top of the heap) a buffer of the payload's decoded size
   (`CoreLzssGetSize`), decodes the payload into it (`CoreLzssDecompress`), stores the entry's
   name pointer through `outName` and returns the buffer. Returns NULL when there is no package or
   no match. */

void *ScriptPackageFind(const char *name, const char **outName)
{
  ScriptPackageDirEntry *entry;
  s32 count;
  s32 i;
  s32 offset;
  u8 *blob;
  u32 size;
  bool fromLow;
  u8 *dst;

  dst = NULL;
  offset = 0;
  entry = (ScriptPackageDirEntry *)g_scriptPackage;
  if (entry == NULL) {
    return NULL;
  }
  /* entry 0's first word is the entry count */
  count = entry->offset;
  for (i = 0; i < count; i++, entry++) {
    if (strcmp(entry->name, name) != 0) {
      continue;
    }
    if (i != 0) {
      offset = entry->offset;
    }
    blob = (u8 *)g_scriptPackage + count * (s32)sizeof(ScriptPackageDirEntry) + offset;
    size = CoreLzssGetSize(blob);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(false);
    dst = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    CoreLzssDecompress(blob, dst);
    *outName = entry->name;
    break;
  }
  return dst;
}
