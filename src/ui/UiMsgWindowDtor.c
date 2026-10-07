// bdc 0x088168d4 UiMsgWindowDtor
#include "bdc.h"

/* Destructor of the message window (`UiMsgWindowCtor`): frees the frame sprite array `+0x3c`,
   deletes the sprite layer `+0x38` and the 0x50-byte `GfxRect` `+0x30` through their virtual
   destructors (flag 3), deletes the text box `+0x2c` (`UiTextBoxDelete`), frees the text buffer
   `+0xc` and the window itself when `flags & 1`. Called by `UiMsgWindowDestroy`. */

void UiMsgWindowDtor(UiMsgWindow *self, u32 flags)
{
    GfxSpriteLayer *layer;
    GfxRect *rect;
    const VtblEntry *dtor;

    if (self == NULL) {
        return;
    }
    if (self->sprites != NULL) {
        GfxSprite **sprites = self->sprites;
        MemLock();
        MemFree(sprites, NULL, 0);
        MemUnlock();
        self->sprites = NULL;
    }
    layer = (GfxSpriteLayer *)self->spriteLayer;
    if (layer != NULL) {
        dtor = &layer->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
        self->spriteLayer = NULL;
    }
    rect = (GfxRect *)self->rect;
    if (rect != NULL) {
        dtor = &rect->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)rect + dtor->delta, 3);
        self->rect = NULL;
    }
    if (self->textBox != NULL) {
        UiTextBoxDelete((UiTextBox *)self->textBox, 3);
        self->textBox = NULL;
    }
    if (self->text != NULL) {
        char *text = self->text;
        MemLock();
        MemFree(text, NULL, 0);
        MemUnlock();
        self->text = NULL;
    }
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
