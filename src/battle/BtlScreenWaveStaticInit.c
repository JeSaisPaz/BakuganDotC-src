// bdc 0x08819eb4 BtlScreenWaveStaticInit
#include "bdc.h"

/* Static constructor of the screen-wave translation unit (`BtlScreenWaveDraw` ends right before
   it): fills the four GE command words of `g_btlScreenWaveTexState`: texture scale U/V = 1.0
   and texture offset U/V = 0. */
void BtlScreenWaveStaticInit(void)
{
    g_btlScreenWaveTexState[0] = 0x483f8000;
    g_btlScreenWaveTexState[1] = 0x493f8000;
    g_btlScreenWaveTexState[2] = 0x4a000000;
    g_btlScreenWaveTexState[3] = 0x4b000000;
}
