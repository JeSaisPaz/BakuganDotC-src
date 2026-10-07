// bdc 0x089c185c SndGroupLoaderIsIdle
#include "bdc.h"

/* Tells whether the `SndGroupLoader` no longer holds anything for `soundId` (under the loader
   lock). With `soundId == -1` it reports whether nothing at all is pending: 1 only when both the
   hold list and the request list are empty (`holds->count == 0 && requests->count == 0`). With a
   real id it scans `requests` and returns 0 when a record with that `soundId` still exists, else 1.
   Returns 1 when the hold list does not exist. */

typedef struct SndRequestRec {
  s32 soundId;
  s32 arg;
  u8 released;
  u8 touched;
} SndRequestRec;

bool SndGroupLoaderIsIdle(SndGroupLoader *loader, s32 soundId)
{
  CoreListNode *node;
  SndRequestRec *rec;
  bool idle;

  idle = true;
  CoreLockAcquire(loader->lock);
  if (loader->holds != NULL) {
    if (soundId == -1) {
      if (loader->holds->count != 0 || loader->requests->count != 0) {
        idle = false;
      }
    } else {
      node = SndRequestListFirst(loader->requests);
      while (node != NULL) {
        rec = node->data;
        node = node->next;
        if (rec != NULL && rec->soundId == soundId) {
          idle = false;
          break;
        }
      }
    }
  }
  CoreLockRelease(loader->lock);
  return idle;
}
