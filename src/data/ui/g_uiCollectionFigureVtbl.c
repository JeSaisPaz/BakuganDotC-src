// bdc 0x08af501c g_uiCollectionFigureVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCollectionFigureVtbl = {
    {0}, { .fn = (void *)UiCollectionFigureDtor }, { .fn = (void *)UiCollectionFigureUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCollectionFigureDraw },
    { .fn = (void *)UiCollectionFigureSetField }, { .fn = (void *)UiCollectionFigureGetField },
};
