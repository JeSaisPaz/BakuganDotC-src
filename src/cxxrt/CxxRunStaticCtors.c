// bdc 0x08a02890 CxxRunStaticCtors
#include "bdc.h"

/* Runs the static constructors listed in `g_cxxCtorTable`. Pass 1 walks the table up to the first
   NULL function: an entry with priority 0 gets the default priority 0x10000 written back, any other
   priority sets a "needs sorting" flag. If that flag is set, the entries are bubble-sorted by
   ascending priority (swapping adjacent entries when the next one is smaller); then every function
   is called in table order with no arguments. In the retail image all priorities are 0, so no sort
   happens and the constructors run in link order. */

void CxxRunStaticCtors(void)
{
  CxxCtorEntry *table = g_cxxCtorTable;
  CxxCtorEntry *e;
  CxxCtorEntry tmp;
  int count = 0;
  int needSort = 0;
  int last;
  int j;

  for (e = table; e->fn != NULL; e++) {
    if (e->priority == 0) {
      e->priority = 0x10000;
    } else {
      needSort = 1;
    }
    count++;
  }

  if (needSort == 1) {
    for (last = count - 1; last >= 0; last--) {
      for (j = 0; j < last; j++) {
        if (table[j + 1].priority < table[j].priority) {
          tmp = table[j];
          table[j] = table[j + 1];
          table[j + 1] = tmp;
        }
      }
    }
  }

  for (e = table; e->fn != NULL; e++) {
    e->fn();
  }
}
