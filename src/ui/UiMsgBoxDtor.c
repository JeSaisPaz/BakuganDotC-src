// bdc 0x089ebee0 UiMsgBoxDtor
#include "bdc.h"

/* Destructor of the 0x80-byte `UiMsgBox`: deletes the choice highlight rect (`highlight`,
   deleting destructor = vtable entry 1, flag 3), the text box helper (`textBox`,
   `UiTextBoxDelete` flag 3) and the owner window frame (`owner`, entry 1 of its sprite-layer
   vtable, flag 3), frees the text buffer `text` under the memory lock, clearing each pointer,
   and frees the box itself when `flags & 1`. A NULL `box` does nothing. */
void UiMsgBoxDtor(UiMsgBox *box, u32 flags)
{
    const VtblEntry *dtor;

    if (box == NULL) {
        return;
    }
    if (box->highlight != NULL) {
        dtor = &box->highlight->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)box->highlight + dtor->delta, 3);
        box->highlight = NULL;
    }
    if (box->textBox != NULL) {
        UiTextBoxDelete(box->textBox, 3);
        box->textBox = NULL;
    }
    if (box->owner != NULL) {
        /* The window frame starts with its sprite layer, which holds the vtable. */
        dtor = &((GfxSpriteLayer *)box->owner)->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)box->owner + dtor->delta, 3);
        box->owner = NULL;
    }
    if (box->text != NULL) {
        MemLock();
        MemFree(box->text, 0, 0);
        MemUnlock();
        box->text = NULL;
    }
    if (flags & 1) {
        MemLock();
        MemFree(box, 0, 0);
        MemUnlock();
    }
}
