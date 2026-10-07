// bdc 0x08a2f790 SndGroupIdListIndexOf
#include "bdc.h"

/* Returns the position (0-based, in list order) of the first node whose `data` equals `groupId` in
   the needed-group-id list of the `SndGroupLoader`, or -1 if it is not present or the list has no
   sentinel. `removed` flags are ignored. */

s32 SndGroupIdListIndexOf(CoreList *list, s32 groupId)
{
  CoreListNode *node;
  s32 index = 0;

  if (list->sentinel != (CoreListNode *)0x0) {
    for (node = list->sentinel->next; node != (CoreListNode *)0x0; node = node->next) {
      if ((s32)(intptr_t)node->data == groupId) {
        return index;
      }
      index = index + 1;
    }
  }
  return -1;
}
