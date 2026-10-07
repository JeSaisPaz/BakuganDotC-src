// bdc 0x0883fc9c BtlResultGetBattleScoreItem
#include "bdc.h"

/* Returns BtlResultGetScoreItem(hud, 0x1d), the battle score; BtlMainPhaseFinish
   passes it to SaveProfileRecordStageRank. */
int BtlResultGetBattleScoreItem(void *hud)
{
    return (int)BtlResultGetScoreItem(hud, 0x1d);
}
