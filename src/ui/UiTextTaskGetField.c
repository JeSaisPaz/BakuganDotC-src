// bdc 0x089eb864 UiTextTaskGetField
#include "bdc.h"

/* `GetField` method of the text task (vtable `0x08af56e4`): fields 0..2 come from
   `CoreTaskGetField`; field 3 reports whether the shared text box has its printer (font ready).
    */

u32 UiTextTaskGetField(CoreTask *task, u32 field)
{
    u32 result = 0;

    if (field < 3) {
        result = CoreTaskGetField(task, field);
    } else if (field == 3 && UiTextRenderExists()) {
        if (UiTextBoxHasPrinter(UiTextRenderGetBox())) {
            result = 1;
        }
    }
    return result;
}
