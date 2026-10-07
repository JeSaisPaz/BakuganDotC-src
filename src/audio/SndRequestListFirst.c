// bdc 0x08a2eebc SndRequestListFirst
#include "bdc.h"

/* Returns the first real node of the outstanding sound-request list of the `SndGroupLoader`
   (`requests`, data = 12-byte `{soundId, arg, u8 released, u8 touched}` records) (the sentinel's
   `next`), or NULL if the sentinel is missing or the list is empty. Nodes flagged `removed` are not
   skipped; walk with `node->next`. */

CoreListNode *SndRequestListFirst(CoreList *list)
{
  CoreListNode *first;

  first = (CoreListNode *)0x0;
  if (list->sentinel != (CoreListNode *)0x0) {
    first = list->sentinel->next;
  }
  return first;
}
