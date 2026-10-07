// bdc 0x08812894 CxxStaticTable08AACBC8Dtor
#include "bdc.h"

/* Empty destructor (`jr ra`) of the plain-data global built by
   `CxxStaffCreditTableCopyStaticInit`: it is the `destructor` slot of that global's
   `CxxGlobalRecord` at `0x08aae39c`, registered with `CxxRegisterGlobalObject`. */

void CxxStaticTable08AACBC8Dtor(void *obj)
{
    (void)obj;
}
