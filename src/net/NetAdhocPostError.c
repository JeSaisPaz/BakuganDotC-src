// bdc 0x089d4764 NetAdhocPostError
#include "bdc.h"

/* Posts the sce error `code` to the net error manager (`NetErrorPush`) unless it is already
   listed; returns 1 when it was added. */

bool NetAdhocPostError(NetAdhocConn *self, s32 code)
{
  if (NetErrorHasManager()) {
    if (!NetErrorContains(NetErrorGetManager(), code)) {
      NetErrorPush(NetErrorGetManager(), code);
      return true;
    }
  }
  return false;
}
