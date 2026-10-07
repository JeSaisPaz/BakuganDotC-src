// bdc 0x08a026c4 CxxRegisterGlobalObject
#include "bdc.h"

/* Records a freshly constructed global object for later destruction: aborts through
   `CxxAbortDoubleRegister` if its `CxxGlobalRecord` `rec` is already linked (`rec->next != 0`)
   or is the current head, otherwise pushes it on `g_cxxGlobalObjects` (`rec->next = head; head =
   rec`). Called at the end of the constructors of globals that have destructors. */

void CxxRegisterGlobalObject(CxxGlobalRecord *rec)

{
  if ((rec->next != (CxxGlobalRecord *)0x0) || (rec == g_cxxGlobalObjects)) {
    CxxAbortDoubleRegister();
  }
  rec->next = g_cxxGlobalObjects;
  g_cxxGlobalObjects = rec;
  return;
}

