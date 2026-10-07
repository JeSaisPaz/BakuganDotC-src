// bdc 0x08939f8c UiUnlockResultGetModelBrightness
#include "bdc.h"

/* Returns the final colour brightness of reward model `index` of
   `UiUnlockResult` from the 20-float table `0x08ac1a20` (1.0 for indices ≥
   20). */

float UiUnlockResultGetModelBrightness(UiUnlockResult *self, u8 index)
{
    static const float kBrightness[20] = {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.7f, 0.55f};
    float table[20];

    memcpy(table, kBrightness, sizeof(kBrightness));
    if (index > 0x13) {
        return 1.0f;
    }
    return table[index];
}
