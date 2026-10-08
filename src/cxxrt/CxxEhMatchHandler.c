// bdc 0x08a038f0 CxxEhMatchHandler
#include "bdc.h"

/* Searches a handler/specification list (CxxEhHandlerEntry array, last entry flag 0x20) for one
   that accepts the thrown type `type` with qualifiers `quals` (1 pointer, 2/4 pointee qualifiers):
   catch-all (0x10), same type (identical descriptor or equal name token) with compatible pointer
   qualifiers, `void *` (`g_cxxVoidTypeName`) for any pointer, or a base class found by
   `CxxFindBaseClass` (access string `a4`, flag `a5`), which also stores the adjusted object pointer
   in `*adjObj` when `adjObj` is non-null. Returns the 1-based index of the match and the entry in
   `*outEntry`, or 0 with `*outEntry` = 0 when no entry matches. `a3` is unused. */

int CxxEhMatchHandler(int *list, void *type, u8 quals, u32 a3, char *a4, u32 a5, void **adjObj, u32 *outEntry)

{
  CxxEhHandlerEntry *entry;
  CxxTypeInfo *thrown;
  CxxTypeInfo *caught;
  char *access;
  void *adjusted;
  int result;
  int index;
  int matched;
  u32 isPtr;
  u32 ptrQuals;
  u8 flags;

  result = 0;
  *outEntry = 0;
  isPtr = quals & 1;
  ptrQuals = quals & 6;
  entry = (CxxEhHandlerEntry *)list;
  thrown = (CxxTypeInfo *)type;
  index = 0;
  for (;;) {
    access = a4;
    matched = 0;
    flags = entry->flags;
    index++;
    if ((flags & 0x10) != 0) {
      matched = 1;
    } else if ((u32)(flags & 1) == isPtr &&
               (entry->type == thrown ||
                (entry->type->name == thrown->name && entry->type->name != NULL))) {
      if (isPtr == 0 || (~(flags & 6) & ptrQuals) == 0) {
        matched = 1;
      }
    }
    if (!matched && (u32)(flags & 1) == isPtr &&
        (isPtr == 0 || ((flags & quals) & 6) == ptrQuals)) {
      caught = entry->type;
      if (caught->name != NULL && caught->name == &g_cxxVoidTypeName && isPtr != 0) {
        matched = 1;
      } else if (thrown->bases != NULL &&
                 CxxFindBaseClass(adjObj, &adjusted, thrown,
                                  caught, &access, (int)a5) != 0) {
        matched = 1;
        if (adjObj != NULL) {
          *adjObj = adjusted;
        }
      }
    }
    if (matched) {
      result = index;
      *outEntry = (u32)(uintptr_t)entry; /* bdc: ptr-narrow ok: no caller reads *outEntry */
      break;
    }
    if ((entry->flags & 0x20) != 0) {
      break;
    }
    entry++;
  }
  return result;
}
