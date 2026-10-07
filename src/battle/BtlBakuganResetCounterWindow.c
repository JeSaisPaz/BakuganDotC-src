// bdc 0x088712c8 BtlBakuganResetCounterWindow
#include "bdc.h"

/* Clears the counter context: window timer, recorded attacker and press timer. */
void BtlBakuganResetCounterWindow(BtlBakugan *bakugan)
{
    bakugan->counterWindow = 0;
    bakugan->attacker = NULL;
    bakugan->counterPress = 0;
}
