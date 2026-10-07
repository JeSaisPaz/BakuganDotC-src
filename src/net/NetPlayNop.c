// bdc 0x0881bfe8 NetPlayNop
#include "bdc.h"

/* Empty function (`jr ra`), called from `BtlMainPhaseBattle`; presumably a NetPlay hook compiled
   out of the release build. */
void NetPlayNop(void)
{
}
