// bdc 0x089a85b4 UiMainMenuApplySelection
#include "bdc.h"

/* Clears the menu flags (`UiMenuFlagsModify` op 2), then turns the chosen item into the menu
   result (`UiSetMenuResult`): item 0 → 1 (story; also `SaveSnapshotBegin` and
   `GameFieldRestoreProgress`), 1 → 2 (or 6 when `leaveAlt` is 1), 2 → 4, 3 → 5, 4 → 3; any
   other cursor sets no result. Unless cancelled, stores the cursor in profile word 0x17. A
   cancelled menu (`cancelled`) yields result 0 and leaves the profile word alone. */

void UiMainMenuApplySelection(UiMainMenu *self)
{
  UiMenuFlagsModify(2, 0);
  if (self->cancelled != 0) {
    UiSetMenuResult(&self->base, 0);
    return;
  }
  if ((u32)(s32)self->cursor < 5) {
    switch (self->cursor) {
    case 1:
      if (self->leaveAlt == 1) {
        UiSetMenuResult(&self->base, 6);
      } else {
        UiSetMenuResult(&self->base, 2);
      }
      break;
    case 2:
      UiSetMenuResult(&self->base, 4);
      break;
    case 3:
      UiSetMenuResult(&self->base, 5);
      break;
    case 4:
      UiSetMenuResult(&self->base, 3);
      break;
    default:
      UiSetMenuResult(&self->base, 1);
      SaveSnapshotBegin();
      GameFieldRestoreProgress();
      break;
    }
  }
  SaveProfileSetWord(SaveGetProfile(), 0x17, self->cursor);
}
