// bdc 0x089c8674 SndBgmCmdListDestroyGlobal
#include "bdc.h"

/* Destroys the global BGM command list: if `g_sndBgmCmdList` is set it runs the list destructor
   with flags 3 (destruct and delete, `SndBgmCmdListDestroy`) and clears the global. */

void SndBgmCmdListDestroyGlobal(void)

{
  if (g_sndBgmCmdList != (CoreList *)0x0) {
    SndBgmCmdListDestroy(g_sndBgmCmdList,3);
    g_sndBgmCmdList = (CoreList *)0x0;
  }
  return;
}

