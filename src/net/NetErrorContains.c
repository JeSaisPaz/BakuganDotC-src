// bdc 0x089d25d0 NetErrorContains
#include "bdc.h"

/* Returns 1 if an error with `code` is queued (walks the list under the manager lock), else 0. Used
   to avoid posting the same error twice. */

bool NetErrorContains(void *mgr, s32 code)

{
  NetErrorMgr *m = mgr;
  s32 *rec;
  bool found = false;

  CoreLockAcquire(m->lock);
  NetErrorListMerge(m->list);
  if (NetErrorListHasEntries(m->list)) {
    while ((rec = NetErrorListNext(m->list)) != (s32 *)0x0) {
      if (*rec == code) {
        found = true;
        break;
      }
    }
  }
  CoreLockRelease(m->lock);
  return found;
}
