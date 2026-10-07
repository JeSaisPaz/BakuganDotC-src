// bdc 0x0898b3f8 UiCollectionFigureDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiCollectionFigure screen (task id 314): reinstalls vtable
   `g_uiCollectionFigureVtbl`, waits for the GE, deletes the six cell cameras through their
   virtual destructors (flag 3), runs `UiCollectionFigureFreeModels`, clears
   `g_gfxActiveCamera`, deletes the help printer; then `UiScreenDtor``(this, 0)` and frees the
   object when `flags & 1`. */

void UiCollectionFigureDtor(UiCollectionFigure *self, u32 flags)
{
    UiTextPrinter *printer;
    const VtblEntry *vt;
    int i;

    if (self != NULL) {
        self->base.base.vtable = g_uiCollectionFigureVtbl;
        GfxWaitGeIdle();
        for (i = 0; i < 6; i++) {
            CoreNode *cam = (CoreNode *)self->cameras[i];

            if (cam != NULL) {
                vt = (const VtblEntry *)cam->vtable;
                ((void (*)(void *, int))vt[1].fn)((char *)cam + vt[1].delta, 3);
                self->cameras[i] = NULL;
            }
        }
        UiCollectionFigureFreeModels(self);
        g_gfxActiveCamera = NULL;
        printer = self->helpPrinter;
        if (printer != NULL) {
            vt = printer->layer.vtbl;
            ((void (*)(void *, int))vt[1].fn)((char *)printer + vt[1].delta, 3);
            self->helpPrinter = NULL;
        }
        UiScreenDtor(&self->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
