// bdc 0x08af1654 g_uiTextPrinterVtbl
#include "bdc.h"

__typeof__(VtblEntry[6]) g_uiTextPrinterVtbl = {
    {0}, { .fn = (void *)UiTextPrinterDtor }, { .fn = (void *)UiTextPrinterPrint },
    { .fn = (void *)UiTextPrinterPrintVertical }, { .fn = (void *)UiTextPrinterAddOutline },
    { .fn = (void *)UiTextPrinterFlush },
};
