// bdc 0x0886f8c0 BtlBakuganCalcVoiceBank
#include "bdc.h"

/* `BtlCalcVoiceBank` for a unit (kind `+8`, player flag `+0x158`); returns 0x71 for NULL. Called
   by `BtlCreateBakugan` to fill the voice bank `+0x5a0`. */
int BtlBakuganCalcVoiceBank(BtlBakugan *bakugan)
{
    if (bakugan != NULL) {
        return BtlCalcVoiceBank(bakugan->base.base.unk08, bakugan->isPlayer);
    }
    return 0x71;
}
