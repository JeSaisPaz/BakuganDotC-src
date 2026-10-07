// bdc 0x0883f63c BtlResultGetPlayerRecord
#include "bdc.h"

/* Score items 0/4/8/12: for result row `index`, finds that team slot's Bakugan
   (`BtlFindTeamBakugan`) and returns its crystal break count when profile word 7 is 1, else its
   knock-out tally; 0 when the slot has no Bakugan. */

u32 BtlResultGetPlayerRecord(void *hud, int index)
{
    BtlBakugan *bakugan;
    u32 record;

    (void)hud;
    record = 0;
    bakugan = (BtlBakugan *)BtlFindTeamBakugan(index);
    if (bakugan != NULL) {
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 1) {
            record = bakugan->crystalBreakCount;
        } else {
            record = bakugan->knockoutTally;
        }
    }
    return record;
}
