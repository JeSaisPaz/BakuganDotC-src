// bdc 0x08a2fd74 SndBgmCmdListFirst
#include "bdc.h"

/* Returns the first real node of the BGM command list (`sentinel->next`), or NULL when the list has
   no sentinel or is empty. The walkers (`SndBgmCancelChannel`, `SndBgmCountChannelCmds`,
   `SndBgmCmdListNodeAt`) start here and follow `node->next`. */

CoreListNode *SndBgmCmdListFirst(CoreList *list)

{
  CoreListNode *first;
  
  first = (CoreListNode *)0x0;
  if (list->sentinel != (CoreListNode *)0x0) {
    first = list->sentinel->next;
  }
  return first;
}

