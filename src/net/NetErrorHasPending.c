// bdc 0x089d257c NetErrorHasPending
#include "bdc.h"

/* Returns whether any network error is queued (`NetErrorListHasEntries` under the manager lock).
    */

bool NetErrorHasPending(NetErrorMgr *mgr)

{
  bool pending;

  CoreLockAcquire(mgr->lock);
  pending = NetErrorListHasEntries(mgr->list);
  CoreLockRelease(mgr->lock);
  return pending;
}
