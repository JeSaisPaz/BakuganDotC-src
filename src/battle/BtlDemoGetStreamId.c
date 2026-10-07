// bdc 0x08905530 BtlDemoGetStreamId
#include "bdc.h"

/* Returns the sound stream id played with demo `id`: 0x15 for 1..21 (`BtlDemoIdIsBrawlerIntro`),
   0x27 for variant 3 (`BtlDemoIdIsVariant3`), 0x26 for variant 1 (`BtlDemoIdIsVariant1`),
   else -1. */
int BtlDemoGetStreamId(int id)
{
    if (BtlDemoIdIsBrawlerIntro(id)) {
        return 0x15;
    }
    if (BtlDemoIdIsVariant3(id)) {
        return 0x27;
    }
    if (BtlDemoIdIsVariant1(id)) {
        return 0x26;
    }
    return -1;
}
