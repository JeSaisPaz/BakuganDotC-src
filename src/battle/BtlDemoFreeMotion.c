// bdc 0x088fee8c BtlDemoFreeMotion
#include "bdc.h"

/* Frees motion `index` of the active team Bakugan's motion-name list
   (`g_charMotionNameLists` indexed by `SaveGetTeamBakugan`(-1)) from the GMO motion manager
   (`GmoMotionFreeByName`, name copied to a 64-byte stack buffer first), when that list exists and
   the demo has a Bakugan. */

void BtlDemoFreeMotion(BtlDemo *demo, int index)
{
    char name[64];
    s32 kind;

    kind = SaveGetTeamBakugan(-1);
    if (g_charMotionNameLists[kind] != NULL && demo->bakugan != NULL) {
        strcpy(name, g_charMotionNameLists[kind][index]);
        GmoMotionFreeByName(GmoMotionMgrGet(), name);
    }
}
