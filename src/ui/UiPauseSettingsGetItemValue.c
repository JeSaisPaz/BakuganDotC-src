// bdc 0x089acf20 UiPauseSettingsGetItemValue
#include "bdc.h"

/* Returns the value of settings item `item`: sliders 0..2 from `sliders[]`, item 3 the advisor
   toggle (profile adviceOff), anything else 0. */

u8 UiPauseSettingsGetItemValue(UiPauseSettings *self, u8 item)
{
    if (item >= 2) {
        if (item < 3) {
            return self->sliders[2];
        }
        if (item < 4) {
            return SaveGetProfile()->data->adviceOff;
        }
        return 0;
    }
    if (item == 0) {
        return self->sliders[0];
    }
    return self->sliders[1];
}
