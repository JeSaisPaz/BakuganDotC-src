// bdc 0x0885b108 BtlLoadRequestOtherBusy
#include "bdc.h"

/* True when another request than `req` in `g_btlLoadRequests` is currently loading. */

bool BtlLoadRequestOtherBusy(void *req)

{
  CoreObject *node;

  if (g_btlLoadRequests != NULL) {
    for (node = g_btlLoadRequests->head; node != NULL; node = node->next) {
      if (node != req && ((BtlLoadRequest *)node)->loading != 0) {
        return true;
      }
    }
  }
  return false;
}
