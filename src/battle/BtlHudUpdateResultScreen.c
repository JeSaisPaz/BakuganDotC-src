// bdc 0x08844068 BtlHudUpdateResultScreen
#include "bdc.h"

/* End-of-battle result step: when profile flag 0 is set and net character 0 exists, marks it
   ready; then, while UI window 9 is active, runs the rated result screen when the battle rule
   mode (script global variable 8) is 1 -- or opens window 10 instead when global script flag
   bit 3 is set -- and the arena result screen for any other rule mode. */
void BtlHudUpdateResultScreen(BtlHud *self)
{
    NetChara *chara;

    if (SaveGetProfileFlag0()) {
        chara = NetCharaGetByIndex(0);
        if (chara != NULL) {
            NetCharaSetReady(chara);
        }
    }
    if (!UiGetWindowActive(9)) {
        return;
    }
    if (g_scriptGlobalVars[8] == 1) {
        if (CoreBitsetTest(3, g_scriptGlobalBits)) {
            UiSetWindowActive(10, 1);
        } else {
            BtlHudRatingResultScreen(self);
        }
    } else {
        BtlHudArenaResultScreen(self);
    }
}
