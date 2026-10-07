// bdc 0x089f87ac GfxFabFindLabel
#include "bdc.h"

/* Looks up a `FLAB` label of a `.fab` file by name: walks the `GfxFabLabel` records from `labels` (count
   `labelCount`, stride `size`) and returns the `value` of the match, or 0. */

u16 GfxFabFindLabel(GfxFab *fab, char *name)
{
    GfxFabLabel *label = fab->labels;
    s32 i = 0;

    if (fab->labelCount != 0) {
        do {
            if (strcmp(label->name, name) == 0) {
                return label->value;
            }
            i++;
            label = (GfxFabLabel *)((u8 *)label + label->size);
        } while (i < (s32)fab->labelCount);
    }
    return 0;
}
