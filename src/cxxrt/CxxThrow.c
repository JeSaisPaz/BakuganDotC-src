// bdc 0x08a03f8c CxxThrow
#include "bdc.h"

/* Throws the current exception: walks the exception-handling frame stack (`g_cxxEhFrameStack`,
   frames `{next, u8 kind, ...}`: 0 try block, 1 cleanup region, 2 exception specification, 3
   no-throw, 4 array construction) below the top frame for the first try block whose handler list
   matches (`CxxEhMatchHandler`), honouring exception specifications (kind 2, calling
   `CxxCallUnexpected` on violation) and no-throw frames (kind 3, terminate); unwinds the frames
   in between (cleanup regions `CxxEhRunCleanups`, arrays `CxxEhCleanupArray`, abandoned
   exceptions); calls `CxxTerminateInternal` when nothing matches; finally stores the handler
   index/object in `g_cxxEhHandlerIndex`/`g_cxxEhCaughtObject` and `longjmp`s into the try
   block. Returns 0 only when the target was a violated exception specification and
   `CxxCallUnexpected` returned. */

int CxxThrow(void)

{
  CxxEhRecord *rec;
  CxxEhFrame *frame;
  CxxEhFrame *target;
  CxxEhFrame *cleanup;
  CxxEhTryFrame *tryFrame;
  CxxEhSpecFrame *spec;
  void *type;
  void *obj;
  void **slot;
  u32 a3;
  u32 a4;
  u32 isPtr;
  u32 handlerEntry;
  u32 specEntry;
  int handlerIndex;
  int match;
  u8 quals;
  u8 flags2;
  u8 kind;

  target = NULL;
  slot = NULL;
  handlerIndex = 0;
  if (((CxxEhRecord *)g_cxxEhCurrentException)->pushed == 0) {
    CxxEhPushThrowFrame();
  }
  rec = (CxxEhRecord *)g_cxxEhCurrentException;
  rec->constructed = 1;
  type = rec->type;
  quals = rec->flags;
  a3 = rec->extra;
  a4 = rec->extra2;
  flags2 = rec->flags2;
  isPtr = quals & 1;
  if (isPtr != 0) {
    /* thrown pointer: the adjusted pointer goes into the record's copy slot */
    obj = *(void **)rec->object;
    slot = &rec->objectCopy;
  }
  else {
    obj = rec->object;
  }

  /* search for the target frame */
  for (frame = ((CxxEhFrame *)g_cxxEhFrameStack)->next; frame != NULL; frame = frame->next) {
    kind = frame->kind;
    if (kind == 1 || kind == 4) {
      continue;
    }
    if (kind == 0) {
      tryFrame = (CxxEhTryFrame *)frame;
      if (tryFrame->caught != NULL) {
        continue;
      }
      match = 1;
      if (tryFrame->handlers != NULL) {
        match = CxxEhMatchHandler(tryFrame->handlers, (int)(intptr_t)type, quals, a3, a4, flags2,
                                  (void *)&obj, &handlerEntry);
      }
      if (match == 0) {
        continue;
      }
      if (target == NULL) {
        target = frame;
        handlerIndex = match;
      }
      if (tryFrame->handlers != NULL) {
        break;
      }
      continue;
    }
    if (target != NULL) {
      continue;
    }
    if (kind == 2) {
      spec = (CxxEhSpecFrame *)frame;
      match = 0;
      if (spec->list != NULL) {
        match = CxxEhMatchHandler(spec->list, (int)(intptr_t)type, quals, a3, a4, flags2, NULL, &specEntry);
      }
      if (match != 0) {
        continue;
      }
      target = frame;
      break;
    }
    if (kind == 3) {
      g_cxxEhFrameStack = frame;
      ((CxxEhRecord *)g_cxxEhCurrentException)->active = 1;
      CxxEhPopFrame();
      CxxTerminateInternal();
    }
  }

  /* unwind the frames between the top and the target */
  for (frame = ((CxxEhFrame *)g_cxxEhFrameStack)->next; frame != target; frame = frame->next) {
    kind = frame->kind;
    if (kind == 1) {
      CxxEhRunCleanups(frame, g_cxxEhCurrentRegion, 0xffff);
      g_cxxEhCurrentRegion = ((CxxEhCleanupFrame *)frame)->region;
    }
    else if (kind == 4) {
      CxxEhCleanupArray(frame);
    }
    else if (kind == 0) {
      if (((CxxEhTryFrame *)frame)->caught != NULL) {
        CxxEhReleaseException(((CxxEhTryFrame *)frame)->caught);
      }
    }
  }
  if (target == NULL) {
    ((CxxEhRecord *)g_cxxEhCurrentException)->active = 1;
    CxxEhPopFrame();
    CxxTerminateInternal();
  }
  tryFrame = (CxxEhTryFrame *)target;
  if (target->kind == 0 && tryFrame->region != g_cxxEhCurrentRegion) {
    /* run the try block's own function cleanups down to its region */
    for (cleanup = target->next; cleanup->kind != 1; cleanup = cleanup->next) {
    }
    CxxEhRunCleanups(cleanup, g_cxxEhCurrentRegion, tryFrame->region);
    g_cxxEhCurrentRegion = tryFrame->region;
  }
  ((CxxEhFrame *)g_cxxEhFrameStack)->next = target;
  rec = (CxxEhRecord *)g_cxxEhCurrentException;
  rec->active = 1;
  if (target->kind == 0) {
    g_cxxEhHandlerIndex = handlerIndex;
    if (isPtr != 0) {
      *slot = obj;
      g_cxxEhCaughtObject = slot;
    }
    else {
      g_cxxEhCaughtObject = obj;
    }
    tryFrame->caught = rec;
    longjmp(tryFrame->jmpBuf, 1);
  }
  else if (target->kind == 2) {
    g_cxxEhFrameStack = ((CxxEhFrame *)g_cxxEhFrameStack)->next;
    CxxCallUnexpected();
  }
  return 0;
}
