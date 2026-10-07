// bdc 0x0889aac8 BtlModelExists
#include "bdc.h"

/* Returns 1 if the model file of `modelId` is loaded, i.e. its name in `g_btlModelNames` is
   found in the loaded resource-pack chain (`CorePackChainFind`), else 0. The first parameter is
   ignored. */
int BtlModelExists(void *unused, int modelId)
{
    (void)unused;
    return CorePackChainFind(g_ioLzsPackages, g_btlModelNames[modelId]) != NULL;
}
