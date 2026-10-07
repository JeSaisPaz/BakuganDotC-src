// bdc 0x08a02df8 CxxVecDeleteEx
#include "bdc.h"

/* General `delete[]` helper (`__vec_delete`): for `count == -1` reads the element count from the
   array cookie (aborting with code 9 through `CxxAbortBadArrayDelete` when the cookie's `~n`
   check fails), runs the destructor on each element from last to first under an array frame, then
   frees the block when `doFree` (`CxxVecDeallocate`). Does nothing for a NULL `ptr`. */

void CxxVecDeleteEx(void *ptr, u32 count, u32 size, void *dtor, int doFree, void *dealloc, int twoArgs, int cookie)

{
  u8 frame[0x70]; /* generic EH frame area; CxxEhArrayFrame at its start */
  CxxEhArrayRec rec;
  CxxVecBlock *blk;
  u32 byteSize;
  s32 i;
  u8 *elem;

  byteSize = 0;
  if (ptr == NULL) {
    return;
  }
  CxxEhPushArrayFrame(frame, &rec, 0);
  rec.f14 = doFree;
  rec.f00 = ptr;
  rec.f04 = count;
  rec.f08 = size;
  rec.f18 = dtor;
  rec.f1c = dealloc;
  rec.f20 = twoArgs;
  if (count == 0xffffffff) {
    count = 0;
    if (cookie != 0) {
      blk = (CxxVecBlock *)((u8 *)ptr - cookie);
      count = blk->byteSize;
      if (count != size * ~blk->notCount) {
        CxxAbortBadArrayDelete();
      }
    }
    byteSize = count;
    count = byteSize / size;
  }
  rec.f04 = count;
  if (dtor != NULL) {
    elem = (u8 *)ptr + (count - 1) * size;
    for (i = 0; i < (s32)count; i++) {
      rec.f0c++;
      ((void (*)(void *, int))dtor)(elem, 2);
      elem -= size;
    }
  }
  g_cxxEhFrameStack = ((CxxEhArrayFrame *)g_cxxEhFrameStack)->next;
  if (doFree != 0) {
    CxxVecDeallocate(ptr, byteSize, dtor, dealloc, twoArgs, cookie);
  }
}
