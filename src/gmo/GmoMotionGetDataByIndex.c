// bdc 0x089d9ea4 GmoMotionGetDataByIndex
#include "bdc.h"

/* Returns the data block (`GmoMotionInfo`) of the registry motion with the given index: the entry
   is found with the callback stored at `mgr+0x10` when one is installed, otherwise with
   `GmoMotionAt`; the data comes from the entry's virtual "get data" method (vtable entry 2). NULL
   when there is no such entry. */

typedef struct GmoMotionVtblView {
  u8 pad00[0x10];
  s16 getDataAdj;
  u8 pad12[2];
  void *(*getData)(void *self);
} GmoMotionVtblView;

typedef struct GmoMotionMgrView {
  u8 pad00[0x10];
  CoreNode *(*lookup)(s32 index, void *mgr);
} GmoMotionMgrView;

void *GmoMotionGetDataByIndex(void *mgr, s32 index)
{
  GmoMotionMgrView *m = (GmoMotionMgrView *)mgr;
  CoreNode *node;
  const GmoMotionVtblView *vt;

  if (m->lookup != NULL) {
    node = m->lookup(index, mgr);
  } else {
    node = GmoMotionAt(mgr, index);
  }
  if (node == NULL) {
    return NULL;
  }
  vt = (const GmoMotionVtblView *)node->vtable;
  return vt->getData((u8 *)node + vt->getDataAdj);
}
