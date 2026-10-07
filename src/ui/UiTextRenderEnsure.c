// bdc 0x089eaa40 UiTextRenderEnsure
#include "bdc.h"

/* Lazily creates the shared text-render holder `g_uiTextRenderHolder` (4-byte holder in the low
   heap pointing to a 0x10-byte text box built by `UiTextBoxCtor`, NULL if that allocation fails)
   and the text task (core id `0x2742`, `UiTextTaskCtor`, priority 0x32) if `UiHasTextTask` says
   it is missing. Returns the text box stored in the holder. */

void *UiTextRenderEnsure(void)
{
    bool fromLow;
    void **holder;
    UiTextBox *box;
    UiTextBox *stored;

    if (g_uiTextRenderHolder == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        holder = MemAlloc(sizeof(void *), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        g_uiTextRenderHolder = holder;
        memset(holder, 0, sizeof(void *));

        stored = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        box = MemAlloc(sizeof(UiTextBox), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (box != NULL) {
            UiTextBoxCtor(box);
            stored = box;
        }
        *g_uiTextRenderHolder = stored;
    }
    if (!UiHasTextTask()) {
        CoreTaskCreate(0x2742, 0x32);
    }
    return *g_uiTextRenderHolder;
}
