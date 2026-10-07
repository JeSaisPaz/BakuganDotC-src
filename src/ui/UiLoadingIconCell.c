// bdc 0x0890c0c0 UiLoadingIconCell
#include "bdc.h"

/* Splits icon index `i` into its cell in a 3-column sheet: `*col = i % 3`, `*row = i / 3` (as
   floats). */

void UiLoadingIconCell(UiLoading *self, int i, float *col, float *row)

{
  *col = (float)(i % 3);
  *row = (float)(i / 3);
  return;
}

