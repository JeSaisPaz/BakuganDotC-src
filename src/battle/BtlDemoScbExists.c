// bdc 0x08905434 BtlDemoScbExists
#include "bdc.h"

/* Returns whether the `.scb` scene file `g_btlDemoScbNames``[index]` is present in the package
   chain `g_ioLzsPackages` (`CorePackChainFind`). */
bool BtlDemoScbExists(int index)
{
    return CorePackChainFind(g_ioLzsPackages, g_btlDemoScbNames[index]) != NULL;
}
