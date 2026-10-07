// bdc 0x08a02a78 CxxVecNewEx
#include "bdc.h"

/* General array-construction helper behind `new T[n]` (the compiler runtime's `__vec_new`):
   optionally allocates the block (`n * size + cookie` bytes, through the supplied allocator or the
   default `CxxOperatorNewArray`), writes the array cookie `{n * size, ~n}` just below the
   returned pointer (for a caller-supplied `ptr` only when `writeCookie`), and calls the
   constructor on each of the `n` elements (stride `size`), with an exception-cleanup frame
   (`CxxEhPushArrayFrame`, chain head `g_cxxEhFrameStack`) registered while it runs when the
   runtime allocated the block or a destructor (`unwindFlag`) is given. Returns the array pointer,
   or NULL if allocation fails. Takes 12 argument words (8 in registers, the rest on the stack);
   callers go through `CxxVecNew`. */

void *CxxVecNewEx(void *ptr, u32 count, s32 size, s32 ctorArg, void *ctor, s32 unwindFlag, void *allocator, void *dealloc, s32 twoArgs, s32 writeCookie, s32 unused11, s32 cookieSize)

{
  u8 frame[0x70]; /* generic EH frame area; CxxEhArrayFrame at its start */
  CxxEhArrayRec rec;
  CxxVecBlock *blk;
  int allocated;
  int pushFrame;
  u32 byteSize;
  s32 i;
  u8 *elem;

  allocated = (ptr == NULL);
  pushFrame = (unwindFlag != 0) | allocated;
  if (ptr == NULL || writeCookie != 0) {
    byteSize = count * size;
    if (ptr == NULL) {
      if (allocator == NULL) {
        ptr = CxxOperatorNewArray(cookieSize + byteSize);
      }
      else {
        ptr = ((void *(*)(u32))allocator)(cookieSize + byteSize);
      }
      if (ptr != NULL) {
        ptr = (u8 *)ptr + cookieSize;
      }
      if (ptr == NULL) {
        return NULL;
      }
    }
    if (cookieSize != 0) {
      blk = (CxxVecBlock *)((u8 *)ptr - cookieSize);
      blk->byteSize = byteSize;
      blk->notCount = ~count;
    }
  }
  if (pushFrame != 0) {
    CxxEhPushArrayFrame(frame, &rec, 1);
    rec.f14 = allocated;
    rec.f04 = count;
    rec.f08 = size;
    rec.f18 = (void *)(intptr_t)unwindFlag;
    rec.f1c = dealloc;
    rec.f20 = twoArgs;
    rec.f00 = ptr;
  }
  if (ctor != NULL) {
    elem = (u8 *)ptr;
    for (i = 0; i < (s32)count; i++) {
      if (ctorArg == 0) {
        ((void (*)(void *, int, int, int, int, int, int, int, int))ctor)(elem, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      else {
        ((void (*)(void *, s32))ctor)(elem, ctorArg);
      }
      if (unwindFlag != 0) {
        rec.f0c++;
      }
      if (ctorArg != 0) {
        ctorArg += size;
      }
      elem += size;
    }
  }
  if (pushFrame != 0) {
    g_cxxEhFrameStack = ((CxxEhArrayFrame *)g_cxxEhFrameStack)->next;
  }
  return ptr;
}
