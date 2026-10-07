// bdc 0x089eaf7c UiTextBoxDelete
#include "bdc.h"

/* Destructor of the text box helper: deletes the text printer at `+0xc` through its virtual
   destructor (vtable at printer `+0x74`, flag 3) and frees the box when bit 0 of `flags` is set. */

void UiTextBoxDelete(UiTextBox *box, u32 flags)
{
    UiTextPrinter *printer;
    const VtblEntry *dtor;

    if (box != NULL) {
        printer = box->printer;
        if (printer != NULL) {
            dtor = &printer->layer.vtbl[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)printer + dtor->delta, 3);
            box->printer = NULL;
        }
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(box, NULL, 0);
            MemUnlock();
        }
    }
}
