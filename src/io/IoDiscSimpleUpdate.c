// bdc 0x089fa550 IoDiscSimpleUpdate
#include "bdc.h"

#define IO_DISC_ERR_ASYNC_BUSY ((int)0x80020329)

/* Calls state handler `slot` of the reader's vtable (`g_discSimpleVtbl`, GCC 2.x entry:
   `this` adjusted by the entry's delta). */
#define IO_DISC_STATE_CALL(self, slot)                                                    \
  ((int (*)(void *))(self)->vtbl[slot].fn)((u8 *)(self) + (self)->vtbl[slot].delta)

/* Per-frame update of the `CODiscSimple` disc reader (vtable slot `+0x0c`), under its `lock`:
   1. if an async operation is issued (`asyncIssued`, `fd >= 0`) and `sceIoPollAsync` reports it
      finished (`<= 0`), clears `asyncIssued`, sets `asyncPending` and copies the low word of
      `asyncResult` into `result`;
   2. if nothing is in flight: on the memory stick (`medium == 2`) a negative `result` other than
      `0x80020329` (async busy) sets `msError = 1`, records it through `IoDiscSetError` when no
      error is stored yet (skipped on `powerSuspend`) and sets `abortClose`; with `abortClose` set,
      states 2-5/7/8 (and 1 on the memory stick) are forced to the close state 6 (`asyncPending`,
      `asyncIssued`, `result` cleared);
   3. unless the memory-stick service is busy (`CoreMsIsIdle` false), dispatches `state` to the
      vtable handlers: 1 `IoDiscSimpleStateOpen`, 2 `IoDiscSimpleStateGetSize`, 4
      `IoDiscSimpleStateRead`, 5 `IoDiscSimpleStateStreamRead` (its return keeps the thread
      awake), 6 `IoDiscSimpleStateClose`, 7 `IoDiscSimpleStateRewind`, 8
      `IoDiscSimpleStateGetLba`, 10 `IoDiscSimpleStateReopen`; state 3 sends the thread to
      sleep, state 9 returns to state 1 unless `streamed`.
   While `busy`, stamps `g_ioDiscClock.lastActiveTime`. After releasing the lock the thread
   sleeps (`BootSleepCurrentThread`) in state 3 or when idle (state 0, nothing issued, no
   stream-read result); otherwise, when the state did not change, it waits one vblank if
   `result` is async busy, else delays 200 us. */

void IoDiscSimpleUpdate(IoDiscSimple *self)
{
  bool sleep = false;
  int streamResult = 0;
  int prevState;
  int medium;
  bool msReady;

  CoreLockAcquire(self->lock);
  if (self->asyncIssued && self->fd >= 0 &&
      sceIoPollAsync(self->fd, &self->asyncResult) <= 0) {
    self->asyncIssued = 0;
    self->asyncPending = 1;
    self->result = (int)self->asyncResult;
  }
  prevState = self->state;
  if (!self->asyncIssued) {
    medium = self->medium;
    if (self->result < 0 && medium == 2 && self->result != IO_DISC_ERR_ASYNC_BUSY) {
      if (!self->powerSuspend) {
        self->msError = 1;
        if (IoDiscGetError(self) == 0) {
          IoDiscSetError(self, self->result);
        }
      }
      self->abortClose = 1;
      medium = self->medium;
    }
    if (self->abortClose) {
      switch (self->state) {
      case 1:
        if (medium != 2) {
          break;
        }
        /* fall through */
      case 2:
      case 3:
      case 4:
      case 5:
      case 7:
      case 8:
        self->state = 6;
        self->asyncPending = 0;
        self->asyncIssued = 0;
        self->result = 0;
        break;
      default:
        break;
      }
    }

    msReady = true;
    if (medium == 2 && CoreMsHasState() && !CoreMsIsIdle(CoreMsGet())) {
      msReady = false;
    }
    if (msReady) {
      switch (self->state) {
      case 1:
        IO_DISC_STATE_CALL(self, 3);
        break;
      case 2:
        IO_DISC_STATE_CALL(self, 4);
        break;
      case 3:
        sleep = true;
        break;
      case 4:
        IO_DISC_STATE_CALL(self, 5);
        break;
      case 5:
        streamResult = IO_DISC_STATE_CALL(self, 6);
        break;
      case 6:
        IO_DISC_STATE_CALL(self, 7);
        break;
      case 7:
        IO_DISC_STATE_CALL(self, 8);
        break;
      case 8:
        IO_DISC_STATE_CALL(self, 9);
        break;
      case 9:
        if (!self->streamed) {
          self->state = 1;
        }
        break;
      case 10:
        IO_DISC_STATE_CALL(self, 10);
        break;
      default:
        break;
      }
    }
  }

  if (self->busy) {
    sceRtcGetCurrentClockLocalTime(&g_ioDiscClock.lastActiveTime);
  }
  if (streamResult == 0 && !self->asyncIssued && self->state == 0) {
    sleep = true;
  }
  CoreLockRelease(self->lock);
  if (sleep) {
    BootSleepCurrentThread();
  } else if (prevState == self->state) {
    if (self->result == IO_DISC_ERR_ASYNC_BUSY) {
      sceDisplayWaitVblankStartCB();
    } else {
      sceKernelDelayThreadCB(200);
    }
  }
}
