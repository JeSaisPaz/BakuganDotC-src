// bdc 0x08a15c40 GmoRecGetNext
#include "bdc.h"

/* Returns the word at `+4` (link/child pointer) of a record, or 0 for NULL. */

void *GmoRecGetNext(void *rec)
{
  if (rec == (void *)0x0) {
    return (void *)0x0;
  }
  return ((GmoInstance *)rec)->next;
}
