// bdc 0x089713c4 UiOptionRefreshAllRowArrows
#include "bdc.h"

/* Runs `UiOptionRefreshRowArrows` for the four value rows of `UiOption`. */

void UiOptionRefreshAllRowArrows(UiOption *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        UiOptionRefreshRowArrows(self, (u8)i);
    }
}
