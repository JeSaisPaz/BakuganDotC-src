// bdc 0x089c87a4 SndBgmCancelChannel
#include "bdc.h"

/* Cancels every queued BGM command of BGM channel `channel`: walks `g_sndBgmCmdList` and sets
   `done = 1` on each `SndBgmCmd` whose `channel` matches, so `SndBgmCmdUpdate` kills them on
   its next run without executing them. Does nothing when the list does not exist. Used before
   queuing a new command on a channel (about thirty game-side callers, e.g. the scene exit
   `GameFieldStopAllSound` for channel 1). */

void SndBgmCancelChannel(s32 channel)

{
  CoreListNode *node;
  SndBgmCmd *cmd;

  if ((g_sndBgmCmdList != (CoreList *)0x0) &&
     (node = SndBgmCmdListFirst(g_sndBgmCmdList), node != (CoreListNode *)0x0)) {
    cmd = (SndBgmCmd *)node->data;
    while (1) {
      node = node->next;
      if (cmd->channel == channel) {
        cmd->done = 1;
      }
      if (node == (CoreListNode *)0x0) break;
      cmd = (SndBgmCmd *)node->data;
    }
  }
  return;
}
