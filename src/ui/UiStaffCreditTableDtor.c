// bdc 0x08946264 UiStaffCreditTableDtor
#include "bdc.h"

/* Empty destructor of the staff-credit timeline global built by
   `UiStaffCreditStaticInit`: the `destructor` slot of its `CxxGlobalRecord` at `0x08ac33e4`,
   registered with `CxxRegisterGlobalObject`. */
void UiStaffCreditTableDtor(void *obj)
{
    (void)obj;
}
