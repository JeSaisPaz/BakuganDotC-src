// bdc 0x089d4008 NetAdhocHasError
#include "bdc.h"

/* Returns 1 when the net error manager exists and has a pending error (`NetErrorHasPending`). */

bool NetAdhocHasError(void)
{
  bool result;
  void *mgr;

  result = false;
  if (NetErrorHasManager()) {
    mgr = NetErrorGetManager();
    result = false;
    if (NetErrorHasPending(mgr)) {
      result = true;
    }
  }
  return result;
}
