// bdc 0x0881b1ec NetPlayHasManager
#include "bdc.h"

/* Returns 1 when the NetPlay manager exists, i.e. `g_netPlay` is non-NULL and its word is
   non-NULL, else 0. The guard that 33 callers (task and menu code) use before
   `NetPlayGetManager`. */

bool NetPlayHasManager(void)

{
  bool has;

  has = false;
  if ((g_netPlay != (void **)0x0) && (*g_netPlay != (void *)0x0)) {
    has = true;
  }
  return has;
}
