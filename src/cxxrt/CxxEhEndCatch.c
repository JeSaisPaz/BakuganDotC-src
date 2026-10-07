// bdc 0x08a03e40 CxxEhEndCatch
#include "bdc.h"

/* End of a catch handler: releases the current exception (`CxxEhReleaseException`) and pops all
   finished exception records with their storage (`CxxEhFreeTop`). */

void CxxEhEndCatch(void)

{
  u8 isRethrow;
  CxxEhRecord *rec;

  CxxEhReleaseException(g_cxxEhCurrentException);
  while (((rec = (CxxEhRecord *)g_cxxEhCurrentException) != NULL) && (rec->released != 0)) {
    g_cxxEhCurrentException = rec->next;
    isRethrow = rec->isRethrow;
    CxxEhFreeTop();
    if (isRethrow == 0) {
      CxxEhFreeTop();
    }
  }
}
