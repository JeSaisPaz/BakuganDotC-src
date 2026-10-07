// bdc 0x089c178c SndGroupLoaderRelease
#include "bdc.h"

/* Marks every request record of the `SndGroupLoader` whose `soundId` equals the argument as
   released (`released = 1`), under the loader lock. `SndGroupLoaderUpdate` later removes released
   requests once none of their group's sounds are playing. */

typedef struct SndRequestRec {
  s32 soundId;
  s32 arg;
  u8 released;
  u8 touched;
} SndRequestRec;

void SndGroupLoaderRelease(SndGroupLoader *loader, s32 soundId)
{
  CoreListNode *node;
  SndRequestRec *rec;

  CoreLockAcquire(loader->lock);
  node = SndRequestListFirst(loader->requests);
  while (node != NULL) {
    rec = node->data;
    node = node->next;
    if (rec != NULL && rec->soundId == soundId) {
      rec->released = 1;
    }
  }
  CoreLockRelease(loader->lock);
}
