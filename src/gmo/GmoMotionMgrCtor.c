// bdc 0x089d9660 GmoMotionMgrCtor
#include "bdc.h"

/* Constructor of the motion registry object (0x14 bytes): creates the list root
   `g_gmoMotionRegistry` (a 0x24-byte `CoreNode` from the low heap, no payload) and sets `+0 =
   0`, `+4 = 0`, `+8 = 1` (entry class selector, see `GmoMotionLoadFile`), `+0xc = 0`
   (replace-existing flag, see `GmoMotionContains`), `+0x10 = NULL` (optional lookup callback used
   by `GmoMotionGetDataByIndex`). Returns `mgr`. */

void *GmoMotionMgrCtor(void *mgr)

{
  GmoMotionMgr *m = (GmoMotionMgr *)mgr;
  bool fromLow;
  CoreNode *node;
  CoreNode *root;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  node = MemAlloc(0x24, (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  root = (CoreNode *)0x0;
  if (node != (CoreNode *)0x0) {
    CoreNodeCtor(node, (CoreNode *)0x0);
    root = node;
  }
  g_gmoMotionRegistry = root;
  m->unk00 = 0;
  m->unk04 = 0;
  m->entryClass = 1;
  m->lookupCb = (void *)0x0;
  m->replaceExisting = 0;
  return mgr;
}
