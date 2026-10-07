// bdc 0x0892a178 UiHologramViewNextPage
#include "bdc.h"

/* Advances to the second page of the hologram detail view (`UiHologramViewCtor`, task 392; view
   kind `+0x485`) (`+0x709`) when it exists (page id `+0x704[page] != 0xff`); returns 1 when it
   advanced. */

int UiHologramViewNextPage(UiHologramView *self)
{
  if (self->tipPage != 1) {
    self->tipPage = self->tipPage + 1;
    if (self->pageIds[self->tipPage] != 0xff) {
      return 1;
    }
  }
  return 0;
}
