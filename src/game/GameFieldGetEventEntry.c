// bdc 0x088bf48c GameFieldGetEventEntry
#include "bdc.h"

/* Returns the 8-byte event entry `id` of the field task (id 500, `GameFieldCtor`): from the
   loaded table `+0x6c4` (`{u32 ?, u32 count, entries…}`, a dummy entry `0x08a92c3c` when `id` is
   out of range) or, without one, from the per-area default table `0x08abe908[area]`. */

void *GameFieldGetEventEntry(CoreTask *task, u16 id)
{
  u32 *table = ((GameFieldTask *)task)->eventTable;

  if (table == NULL) {
    return g_gameEventDefaultTables[g_gameEventFlags[0]] + id * 8;
  }
  if (id < table[1]) {
    return &table[2 + id * 2];
  }
  return g_gameEventDummyEntry;
}
