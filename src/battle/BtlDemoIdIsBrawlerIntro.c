// bdc 0x08905510 BtlDemoIdIsBrawlerIntro
#include "bdc.h"

/* Returns 1 when `id` is in 1..21 (the per-brawler intro ids), else 0. */
bool BtlDemoIdIsBrawlerIntro(int id)
{
    return 0 < id && id < 22;
}
