// bdc 0x0881b614 NetPlayIsSynced
#include "bdc.h"

/* Returns the `NetPlay` 'synced' byte `+0xb9`, derived each frame by `NetPlayUpdate`
   (forced to 1 outside a session). Battle and menu code wait on it before advancing. */

bool NetPlayIsSynced(NetPlay *self)

{
  return self->synced != '\0';
}

