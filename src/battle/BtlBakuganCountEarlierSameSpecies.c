// bdc 0x088663a4 BtlBakuganCountEarlierSameSpecies
#include "bdc.h"

/* Counts the units in the battle list (`BtlGetBakuganList`, `node + 4` = next) that come before
   `unit` and have the same species id (`+8`), i.e. the index of `unit` among units of its species
   (0 for the first). Used right after spawning to pick a per-copy texture variant
   (`BtlBakuganApplyTextureVariant`) and the matching variant of the spawn effect model. */

int BtlBakuganCountEarlierSameSpecies(BtlBakugan *self)

{
  BtlBakugan *node = NULL;
  int count = 0;

  if (g_btlBakuganList != NULL) {
    node = *(BtlBakugan **)g_btlBakuganList;
  }
  for (; node != NULL && node != self; node = (BtlBakugan *)node->base.base.next) {
    if (node->base.base.unk08 == self->base.base.unk08) {
      count++;
    }
  }
  return count;
}
