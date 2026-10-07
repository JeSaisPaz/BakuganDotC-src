// bdc 0x089d26a0 NetErrorUpdate
#include "bdc.h"

/* Per-frame step of the `CONetError` manager `NetErrorMgr` (built by `NetErrorMgrCtor`),
   called from the adhoc update `NetAdhocCheckErrors`.
   - A kind-0 system dialog is open (`SysUtilGetDialog`): once it reports done
     (`SysUtilMsgDialogIsIdle`) it reads the shown error value, closes the dialog
     (`SysUtilCloseDialog`) and, under the manager lock, removes the first queued record holding
     that code, returning it to the `g_netErrorMgr` pool or the heap; if the queue is then empty
     the latch byte is cleared.
   - No dialog and the queue has entries: opens the error dialog (`SysUtilOpenDialog` kind 0)
     and starts it (vtable entry 2, the handler's Request) with the front record's code.
   - No dialog and an empty queue: clears the latch under the lock if it was set. */

void NetErrorUpdate(void *mgr)
{
  NetErrorMgr *self = (NetErrorMgr *)mgr;
  SysUtilHandler *dlg = NULL;
  u32 *rec;
  u32 code;

  if (SysUtilIsInit()) {
    dlg = SysUtilGetDialog(SysUtilGetCell(), 0);
  }

  if (dlg != NULL) {
    if (!SysUtilMsgDialogIsIdle()) {
      return;
    }
    code = SysUtilMsgDialogGetErrorValue();
    SysUtilCloseDialog(SysUtilGetCell(), 0);
    CoreLockAcquire(self->lock);
    NetErrorListMerge(self->list);
    for (;;) {
      rec = NetErrorListNext(self->list);
      if (rec == NULL) {
        break;
      }
      if (rec[0] == code) {
        MemPool *pool;

        NetErrorListRemove(self->list, rec);
        pool = (MemPool *)g_netErrorMgr[1];
        if (pool == NULL || !MemPoolFree(pool, rec)) {
          MemLock();
          MemFree(rec, NULL, 0);
          MemUnlock();
        }
        break;
      }
    }
    NetErrorListMerge(self->list);
    if (!NetErrorListHasEntries(self->list)) {
      self->latch = 0;
    }
    CoreLockRelease(self->lock);
  } else if (NetErrorListHasEntries(self->list)) {
    SysUtilOpenDialog(SysUtilGetCell(), 0);
    dlg = SysUtilGetDialog(SysUtilGetCell(), 0);
    CoreLockAcquire(self->lock);
    NetErrorListMerge(self->list);
    rec = NetErrorListNext(self->list);
    ((s32 (*)(void *, u32))dlg->vtbl[2].fn)((u8 *)dlg + dlg->vtbl[2].delta, rec[0]);
    NetErrorListMerge(self->list);
    CoreLockRelease(self->lock);
  } else if (self->latch != 0) {
    CoreLockAcquire(self->lock);
    self->latch = 0;
    CoreLockRelease(self->lock);
  }
}
