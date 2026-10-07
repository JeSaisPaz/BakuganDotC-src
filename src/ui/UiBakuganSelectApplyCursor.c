// bdc 0x0892f1cc UiBakuganSelectApplyCursor
#include "bdc.h"

/* When the cursor `+0x74` moved onto another owned entry, makes it current (`+0x75`, change flag
   `+0x1ce8`), releases the old model and refreshes gauges, name panel and stats
   (`UiBakuganSelectStartGaugeAnim`, `UiBakuganSelectShowNamePanel`,
   `UiBakuganSelectRefreshStatA`, `UiBakuganSelectRefreshStatB`). */

void UiBakuganSelectApplyCursor(UiBakuganSelect *self)

{
  s8 cursor = self->cursor;
  self->currentChanged = 0;
  if (self->entries[cursor].bakugan != 0 && cursor != self->current) {
    self->currentChanged = 1;
    self->current = cursor;
    UiBakuganSelectReleaseModel(self);
    UiBakuganSelectStartGaugeAnim(self, 1, (u8)self->cursor);
    UiBakuganSelectShowNamePanel(self, 1, 0);
    UiBakuganSelectRefreshStatA(self);
    UiBakuganSelectRefreshStatB(self);
  }
}
