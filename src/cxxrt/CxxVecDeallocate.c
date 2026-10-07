// bdc 0x08a02a1c CxxVecDeallocate
#include "bdc.h"

/* Frees an array block: moves `ptr` back by the cookie size and calls the deallocator: the default
   `operator delete[]` (`CxxOperatorDeleteArray`) when none is given, `dealloc(block)` for
   one-argument deallocators or `dealloc(block, size + cookie)` for sized ones (`twoArgs`). */

void CxxVecDeallocate(void *ptr, u32 size, void *dtor, void *dealloc, int twoArgs, int cookie)

{
  void *block;

  block = (u8 *)ptr - cookie;
  if (dealloc == (void *)0x0) {
    CxxOperatorDeleteArray(block);
  }
  else if (twoArgs == 0) {
    ((void (*)(void *))dealloc)(block);
  }
  else {
    ((void (*)(void *, u32))dealloc)(block, cookie + size);
  }
  return;
}

