// bdc 0x089036e0 BtlDemoStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5ce8`) of the battle intro demo unit
   (`BtlDemoCtor`): zeroes the vector `g_btlDemoCamVector` (`0x08b00fe0..0x08b00fef`) used by
   `BtlDemoCamStateMain`. */
void BtlDemoStaticInit(void)
{
    g_btlDemoCamVector.z = 0.0f;
    g_btlDemoCamVector.y = 0.0f;
    g_btlDemoCamVector.x = 0.0f;
    g_btlDemoCamVector.w = 0.0f;
}
