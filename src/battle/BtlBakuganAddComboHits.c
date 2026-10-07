// bdc 0x08860130 BtlBakuganAddComboHits
#include "bdc.h"

/* Adds `hits` to the unit's pending combo hits (`+0x1a8`) and restarts the combo timer `+0x1a4` at
   40 frames. The hits are folded into the combo counter by `BtlBakuganCommitPendingComboHits`
   from `BtlBakuganUpdateCombo`. */

void BtlBakuganAddComboHits(BtlBakugan *bakugan, int hits)
{
    bakugan->pendingComboHits += hits;
    bakugan->comboTimer = 40;
}
