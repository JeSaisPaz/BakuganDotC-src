// bdc 0x08943a94 NetStatusSetMessage
#include "bdc.h"

/* Requests the netplay status overlay: stores the show byte `0x08ac1bd0` and the message index
   `0x08ac1bd4` (row of the table `0x08a9cff0`, which maps to language-table strings) read by the
   `NetStatusTaskCtor` task's states. */

void NetStatusSetMessage(u8 show, s32 message)

{
  g_netStatusShow = show;
  g_netStatusMessage = message;
  return;
}

