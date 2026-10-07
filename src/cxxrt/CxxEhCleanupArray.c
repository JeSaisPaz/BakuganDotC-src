// bdc 0x08a02d10 CxxEhCleanupArray
#include "bdc.h"

/* Unwind cleanup of an array-construction frame: destroys the elements already constructed
   (`dtor(elem, 2)` from the last one down) and, when the array was allocated by the runtime, frees
   it (`CxxVecDeallocate`). */

void CxxEhCleanupArray(void *frame)

{
  CxxEhArrayRec *rec = ((CxxEhArrayFrame *)frame)->rec;
  void (*dtor)(void *, int) = (void (*)(void *, int))(uintptr_t)rec->f18;
  u8 *base = (u8 *)(uintptr_t)rec->f00;
  u32 elemSize = rec->f08;
  u32 count = rec->f0c;
  u32 i;
  u8 *elem;

  if (rec->flag == 0) {
    count = rec->f04 - count;
  }
  if (dtor != NULL) {
    i = 0;
    elem = base + elemSize * (count - 1);
    if (count != 0) {
      do {
        dtor(elem, 2);
        i++;
        elem -= elemSize;
      } while (i < count);
    }
  }
  if (rec->f14 != 0) {
    CxxVecDeallocate(base, elemSize * count, (void *)(uintptr_t)rec->f18, (void *)(uintptr_t)rec->f1c,
                     rec->f20, 0);
  }
}
