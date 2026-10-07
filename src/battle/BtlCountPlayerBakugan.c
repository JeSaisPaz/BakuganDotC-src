// bdc 0x0883a618 BtlCountPlayerBakugan
#include "bdc.h"

/* Returns how many units in the battle unit list (`BtlGetBakuganList`) are player-controlled
   (`isPlayer`); 0 when the list is missing. */
int BtlCountPlayerBakugan(void)
{
  CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
  CoreObject *obj;
  int count = 0;

  if (list == NULL) {
    return 0;
  }
  for (obj = list->head; obj != NULL; obj = obj->next) {
    if (((BtlBakugan *)obj)->isPlayer != 0) {
      count++;
    }
  }
  return count;
}
