// bdc 0x089d98e4 GmoMotionLoadFile
#include "bdc.h"

/* Loads one named motion resource: finds `filename` in the loaded pack chain
   (`CorePackChainFindEntry2`); a name containing `.gmo` is a whole model file, whose motion
   chunks are registered by `GmoMotionLoadFromGmo` (NULL is returned). Otherwise allocates one
   entry from the low heap (a 0x84-byte `GmoMotionEntry`, vtable `g_gmoMotionEntryVtable`, when
   `mgr->entryClass == 0`; a 0x3c-byte `GmoMotionRef`, vtable `g_gmoMotionRefVtable`, otherwise),
   runs its virtual init method (vtable entry 11: `GmoMotionEntryInitFromBin` /
   `GmoMotionRefInitInPlace`) with the pack data and appends it to the registry
   `g_gmoMotionRegistry` with `CoreNodeLink` (end of chain). Returns the entry, or NULL when the
   file was not found or was a `.gmo`. A failed allocation is not checked: the virtual call then
   reads the vtable through NULL. */

CoreNode *GmoMotionLoadFile(void *mgr, const char *filename)

{
  const GmoMotionMgr *m = (const GmoMotionMgr *)mgr;
  CorePackDirEntry *file;
  const GmoFileHeader *gmo;
  CoreNode *node;
  GmoMotionEntry *entry;
  GmoMotionRef *ref;
  const VtblEntry *slot;
  void *data;
  bool fromLow;

  file = CorePackChainFindEntry2(g_ioLzsPackages,(char *)filename);
  if (file == (CorePackDirEntry *)0x0) {
    return (CoreNode *)0x0;
  }
  if (strstr(filename,".gmo") != (char *)0x0) {
    gmo = (const GmoFileHeader *)PspPtr(file->data);
    GmoMotionLoadFromGmo(GmoChunkFind(&gmo->root,3,0));
    return (CoreNode *)0x0;
  }
  /* the asm keeps separate (identical) GmoMotionRef paths for entryClass 1 and for any other
     value (negative or > 1) */
  if ((s32)m->entryClass == 0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    entry = (GmoMotionEntry *)MemAlloc(sizeof(GmoMotionEntry),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (entry != (GmoMotionEntry *)0x0) {
      CoreNodeCtor((CoreNode *)entry,(CoreNode *)0x0);
      ((CoreNode *)entry)->vtable = g_gmoMotionBaseVtbl;
      entry->arrayOwner = (void *)0x0;
      entry->pinned = 1;
      ((CoreNode *)entry)->vtable = g_gmoMotionEntryVtable;
      entry->arena = (u8 *)0x0;
    }
    node = (CoreNode *)entry;
  }
  else {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    ref = (GmoMotionRef *)MemAlloc(sizeof(GmoMotionRef),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (ref != (GmoMotionRef *)0x0) {
      CoreNodeCtor((CoreNode *)ref,(CoreNode *)0x0);
      ((CoreNode *)ref)->vtable = g_gmoMotionBaseVtbl;
      ref->arrayOwner = 0;
      ref->pinned = 1;
      ((CoreNode *)ref)->vtable = g_gmoMotionRefVtable;
    }
    node = (CoreNode *)ref;
  }
  slot = &((const VtblEntry *)node->vtable)[11];
  data = PspPtr(file->data);
  ((void (*)(void *, void *))slot->fn)((u8 *)node + slot->delta,data);
  CoreNodeLink(node,g_gmoMotionRegistry,0);
  return node;
}
