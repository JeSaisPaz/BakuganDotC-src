// bdc 0x08a2fd8c SndBgmCmdListNodeAt
#include "bdc.h"

/* Returns the `index`-th real node (0 = first) of the BGM command list, or NULL when `index` is
   negative or not below `count` or the chain ends early. */

CoreListNode *SndBgmCmdListNodeAt(CoreList *list, s32 index)

{
  CoreListNode *node;

  node = (CoreListNode *)0x0;
  if ((-1 < index) && (index < list->count)) {
    node = SndBgmCmdListFirst(list);
    for (; index > 0; index--) {
      if (node == (CoreListNode *)0x0) {
        return (CoreListNode *)0x0;
      }
      node = node->next;
    }
  }
  return node;
}
