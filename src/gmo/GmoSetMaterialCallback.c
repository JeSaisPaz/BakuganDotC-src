// bdc 0x089db0f4 GmoSetMaterialCallback
#include "bdc.h"

/* Stores its argument in `g_gmoMaterialCallback`. */
void GmoSetMaterialCallback(void *value)
{
    g_gmoMaterialCallback = value;
}
