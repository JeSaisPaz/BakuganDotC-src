// bdc 0x0897579c UiCollectionMenuMoveCursor
#include "bdc.h"

/* Moves the selection of the current page of `UiCollectionMenu` with Up/Down
   repeat, wrapping over 5 main entries (4 when `+0x760`) or the sub-page count
   (`UiCollectionMenuGetSubEntryCount`); records the direction (1 up, 2 down) in `+0x752`. Returns
   1 when moved. */

int UiCollectionMenuMoveCursor(UiCollectionMenu *self)

{
  int page;
  int count;
  int cur;
  s8 *sel;
  u8 up;
  PadState *pad;

  page = self->page;
  self->moveDir = 0;
  pad = self->base.pad;
  if (page == 0) {
    count = 5;
    if (self->hideFifth != 0) {
      count = 4;
    }
    up = (u8)pad->repeat;
  } else {
    count = UiCollectionMenuGetSubEntryCount(self, self->selMain);
    up = (u8)pad->repeat;
  }
  sel = &self->selMain;
  if ((up & 0x10) != 0) {
    cur = sel[page];
    if (cur == 0) {
      cur = count;
    }
    sel[page] = cur - 1;
    self->moveDir = 1;
    return 1;
  }
  if ((pad->repeat & 0x40) == 0) {
    return 0;
  }
  cur = 0;
  if (sel[page] != count - 1) {
    cur = sel[page] + 1;
  }
  sel[page] = cur;
  self->moveDir = 2;
  return 1;
}
