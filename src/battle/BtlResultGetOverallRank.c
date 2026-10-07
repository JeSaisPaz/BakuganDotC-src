// bdc 0x0883fc80 BtlResultGetOverallRank
#include "bdc.h"

/* Returns `BtlResultGetRank``(hud, 3)`, the overall rank (0 = S … 3 = C);
   `BtlMainPhaseFinish` passes it to `SaveProfileRecordStageRank`. */
int BtlResultGetOverallRank(void *hud)
{
    return BtlResultGetRank(hud, 3);
}
