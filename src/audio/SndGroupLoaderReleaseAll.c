// bdc 0x089c1800 SndGroupLoaderReleaseAll
#include "bdc.h"

/* Marks every outstanding request of the `SndGroupLoader` as released: under the loader lock it
   walks `requests` (`SndRequestListFirst` and then each node's `next`) and sets the `released`
   byte (+8) of every non-NULL record to 1. `SndGroupLoaderUpdate` then deletes each request once
   its group has no live voice and unloads the groups, so everything drains over the following
   frames. */

typedef struct SndRequestRec {
  s32 soundId;
  s32 arg;
  u8 released;
  u8 touched;
} SndRequestRec;

void SndGroupLoaderReleaseAll(SndGroupLoader *loader)
{
  CoreListNode *node;
  SndRequestRec *rec;

  CoreLockAcquire(loader->lock);
  node = SndRequestListFirst(loader->requests);
  while (node != NULL) {
    rec = node->data;
    node = node->next;
    if (rec != NULL) {
      rec->released = 1;
    }
  }
  CoreLockRelease(loader->lock);
}
