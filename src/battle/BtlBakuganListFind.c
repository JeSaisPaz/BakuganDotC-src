// bdc 0x08860148 BtlBakuganListFind
#include "bdc.h"

/* Returns `self` if that object is currently in the battle Bakugan chain (`g_btlBakuganList`),
   otherwise NULL; a validity check that script opcodes use on object pointers they got from
   variables. */

void *BtlBakuganListFind(BtlBakugan *self)
{
  BtlBakugan *it;

  if (g_btlBakuganList != NULL) {
    for (it = *(BtlBakugan **)g_btlBakuganList; it != NULL; it = (BtlBakugan *)it->base.base.next) {
      if (it == self) {
        return it;
      }
    }
  }
  return NULL;
}
