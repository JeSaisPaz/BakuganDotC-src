// bdc 0x0899bdf8 UiWorldMapResetDialog
#include "bdc.h"

/* Resets the 16-byte confirm-dialog state `dialogState` of `UiWorldMap` (`[0]`
   answer, `[1]` step) and, when `answer` is non-zero, pre-sets the answer byte;
   `UiWorldMapShowConfirm` then runs the dialog. */

void UiWorldMapResetDialog(UiScreen *screen, u8 answer)
{
  UiWorldMap *map = (UiWorldMap *)screen;

  memset(map->dialogState, 0, 0x10);
  if (answer != 0) {
    map->dialogState[0] = answer;
  }
}
