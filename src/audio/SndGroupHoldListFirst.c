// bdc 0x08a2f35c SndGroupHoldListFirst
#include "bdc.h"

/* Returns the first real node of the group hold list of the `SndGroupLoader` (`holds`, data =
   8-byte `{groupId, ttl}` records) (the sentinel's `next`), or NULL if the sentinel is missing or
   the list is empty. Nodes flagged `removed` are not skipped; walk with `node->next`. */

CoreListNode *SndGroupHoldListFirst(CoreList *list)
{
  CoreListNode *first = (CoreListNode *)0x0;

  if (list->sentinel != (CoreListNode *)0x0) {
    first = list->sentinel->next;
  }
  return first;
}
