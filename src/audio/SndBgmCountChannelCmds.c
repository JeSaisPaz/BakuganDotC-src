// bdc 0x089c8804 SndBgmCountChannelCmds
#include "bdc.h"

/* Counts the live `SndBgmCmd`s in `g_sndBgmCmdList` whose `channel` equals `channel` (0 when
   the list does not exist). The play and stop steps (`SndBgmCmdStepPlay`, `SndBgmCmdStepStop`)
   use it to find out whether older commands for their channel are still queued; the count includes
   the calling command itself. */

s32 SndBgmCountChannelCmds(s32 channel)
{
    CoreListNode *node;
    s32 count = 0;

    if (g_sndBgmCmdList != NULL) {
        node = SndBgmCmdListFirst(g_sndBgmCmdList);
        if (node != NULL) {
            do {
                if (((SndBgmCmd *)node->data)->channel == channel) {
                    count++;
                }
                node = node->next;
            } while (node != NULL);
        }
    }
    return count;
}
