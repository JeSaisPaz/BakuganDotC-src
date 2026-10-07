// bdc 0x089055a4 BtlDemoScbGetName
#include "bdc.h"

/* Returns the `.scb` scene file name `g_btlDemoScbNames``[index]` (`00_dan_braw.scb`, ...). */

const char *BtlDemoScbGetName(int index)
{
    return g_btlDemoScbNames[index];
}
