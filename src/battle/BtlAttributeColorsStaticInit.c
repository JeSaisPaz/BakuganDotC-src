// bdc 0x0886a968 BtlAttributeColorsStaticInit
#include "bdc.h"

/* Static initialiser of the battle-unit translation unit: fills the six RGBA float colours of
   `g_btlAttributeColors` (one per attribute index, alpha 1) read by `BtlGetAttributeColor`. */
void BtlAttributeColorsStaticInit(void)
{
    g_btlAttributeColors[0][0] = 1.0f;
    g_btlAttributeColors[0][1] = 0.125490203f;
    g_btlAttributeColors[0][2] = 0.0f;
    g_btlAttributeColors[0][3] = 1.0f;
    g_btlAttributeColors[1][0] = 0.376470596f;
    g_btlAttributeColors[1][1] = 0.733333349f;
    g_btlAttributeColors[1][2] = 0.925490201f;
    g_btlAttributeColors[1][3] = 1.0f;
    g_btlAttributeColors[2][0] = 0.678431392f;
    g_btlAttributeColors[2][1] = 0.568627477f;
    g_btlAttributeColors[2][2] = 0.423529416f;
    g_btlAttributeColors[2][3] = 1.0f;
    g_btlAttributeColors[3][0] = 0.964705884f;
    g_btlAttributeColors[3][1] = 1.0f;
    g_btlAttributeColors[3][2] = 0.0f;
    g_btlAttributeColors[3][3] = 1.0f;
    g_btlAttributeColors[4][0] = 0.690196097f;
    g_btlAttributeColors[4][1] = 0.254901975f;
    g_btlAttributeColors[4][2] = 0.905882359f;
    g_btlAttributeColors[4][3] = 1.0f;
    g_btlAttributeColors[5][0] = 0.129411772f;
    g_btlAttributeColors[5][1] = 1.04313731f;
    g_btlAttributeColors[5][2] = 0.168627456f;
    g_btlAttributeColors[5][3] = 1.0f;
}
