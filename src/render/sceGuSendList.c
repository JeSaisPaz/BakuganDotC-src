// bdc 0x08a1e880 sceGuSendList
#include "bdc.h"

/* libgu `sceGuSendList`: enqueues `list` with `SceGeListArgs{size 0x10, context, numStacks,
   stacks}` at the head (`mode == 1`, `sceGeListEnQueueHead`) or tail (`sceGeListEnQueue`) of the GE
   queue using the library's callback id (`g_guGeCallbackId`); stores the list id in `g_guSubListId`. Returns
   0, or the negative GE error. */

int sceGuSendList(int mode, const void *list, SceGeContext *context, u32 numStacks, SceGeStack *stacks)
{
  SceGeListArgs args;
  int id;

  args.size = 0x10;
  g_guState4c = 0;
  args.ctx = context;
  args.numStacks = numStacks;
  args.stacks = stacks;
  if (mode == 1) {
    id = sceGeListEnQueueHead((void *)list, (void *)0, g_guGeCallbackId, &args);
  } else {
    id = sceGeListEnQueue((void *)list, (void *)0, g_guGeCallbackId, &args);
  }
  if (id >= 0) {
    g_guSubListId = id;
    id = 0;
  }
  return id;
}
