// bdc 0x08a2fdec SndBgmCmdListGetAt
#include "bdc.h"

/* Returns the `SndBgmCmd` stored in the `index`-th node of the BGM command list
   (`SndBgmCmdListNodeAt`), or NULL when there is no such node. `SndBgmCmdStepPlay` and
   `SndBgmCmdStepStop` use it to walk the commands in queue order up to themselves. */

SndBgmCmd *SndBgmCmdListGetAt(CoreList *list, s32 index)

{
  CoreListNode *node;
  SndBgmCmd *cmd;

  cmd = (SndBgmCmd *)0x0;
  node = SndBgmCmdListNodeAt(list,index);
  if (node != (CoreListNode *)0x0) {
    cmd = node->data;
  }
  return cmd;
}
