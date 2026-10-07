// bdc 0x08854e20 BtlStatsSetOwner
#include "bdc.h"

/* Stores `owner` in word 0 of a per-unit statistics record; `BtlBakuganCtor` binds the record it
   gets from `BtlStatsCreate` (stored at `unit+0x170`) to its unit. */
void BtlStatsSetOwner(BtlStats *stats, void *owner)
{
    stats->owner = owner;
}
