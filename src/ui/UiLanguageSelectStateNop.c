// bdc 0x0880a384 UiLanguageSelectStateNop
#include "bdc.h"

/* Empty state method (index 3 of the language-selection screen's state table `0x08a34024`):
   the screen waits here after UiLanguageSelectStateBuild until something sets the state
   field to 2 (UiLanguageSelectSetField). */
void UiLanguageSelectStateNop(CoreTask *task)
{
    (void)task;
}
