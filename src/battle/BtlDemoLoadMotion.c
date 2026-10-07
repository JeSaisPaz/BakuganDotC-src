// bdc 0x088ff478 BtlDemoLoadMotion
#include "bdc.h"

/* Loads motion `index` of the active team Bakugan's motion-name list (`g_charMotionNameLists` entry
   `SaveGetTeamBakugan``(-1)`), when that list exists and the demo has a Bakugan (`bakugan`,
   `+0x4c4`): loads `<name>.gmo` into the GMO motion manager (`GmoMotionMgrGet`,
   `GmoMotionLoadFile`) and stores the motion's index (`GmoMotionIndexOfName` of `<name>`) in slot
   `index` of the Bakugan's `motionTable` (`+0x164`). Counterpart of `BtlDemoFreeMotion`. */

void BtlDemoLoadMotion(BtlDemo *demo, int index)
{
    char **names;
    char **name;
    BtlBakugan *bakugan;
    s32 motionIndex;
    char fileName[64];

    names = g_charMotionNameLists[SaveGetTeamBakugan(-1)];
    if (names == NULL || demo->bakugan == NULL) {
        return;
    }
    name = &names[index];
    strcpy(fileName, *name);
    strcat(fileName, ".gmo");
    GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
    bakugan = demo->bakugan;
    motionIndex = GmoMotionIndexOfName(GmoMotionMgrGet(), *name);
    bakugan->motionTable[index] = (s16)motionIndex;
}
