// bdc 0x0884efd0 BtlNetSetBattleFlags
#include "bdc.h"

/* Network battles only (profile flag 0 set and a NetPlay manager exists): when `ended`, clears
   NetPlay flag 0x1000000 and then replaces the whole flag word with 0x8000000; otherwise clears
   flag 0x8000000. `main` is unused. */

void BtlNetSetBattleFlags(void *main, bool ended)
{
    (void)main;
    if (SaveGetProfileFlag0() == 0 || NetPlayHasManager() == 0) {
        return;
    }
    if (ended) {
        NetPlayClearFlags(NetPlayGetManager(), 0x1000000);
        NetPlaySetFlags(NetPlayGetManager(), 0x8000000);
    } else {
        NetPlayClearFlags(NetPlayGetManager(), 0x8000000);
    }
}
