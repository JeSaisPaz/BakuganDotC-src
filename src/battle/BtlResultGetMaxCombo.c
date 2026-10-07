// bdc 0x0883f430 BtlResultGetMaxCombo
#include "bdc.h"

/* Result-screen score item 0x11: the player Bakugan's best combo (BtlBakugan.bestCombo), 0 when
   there is no player Bakugan, clamped to 0..99. `BtlResultGetScoreItem` multiplies it by 50 for
   item 0x13. */
int BtlResultGetMaxCombo(void *hud)
{
    BtlBakugan *bakugan = (BtlBakugan *)BtlHudGetPlayerBakugan(hud);
    int combo = 0;

    if (bakugan != NULL) {
        combo = bakugan->bestCombo;
    }
    if (combo < 0) {
        return 0;
    }
    if (combo > 99) {
        return 99;
    }
    return combo;
}
