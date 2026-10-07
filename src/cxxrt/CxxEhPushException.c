// bdc 0x08a03bd0 CxxEhPushException
#include "bdc.h"

/* Creates an exception record (0xa4 bytes from `CxxEhAlloc`) and pushes it on the
   current-exception stack (`g_cxxEhCurrentException`): thrown type, destructor, qualifier flags,
   object pointer and rethrow link; counts one use on the original record for a rethrow, else on the
   new one. Remembers the innermost try frame not already handling an exception and, when the
   previous record belongs to that same frame, drops one use of the new record
   (`CxxEhReleaseException`). */

void CxxEhPushException(void *type, void *dtor, u8 flags, int a3, int a4, u8 a5, void *object, int isRethrow, void *orig)
{
    CxxEhRecord *exc;
    CxxEhRecord *origRec = (CxxEhRecord *)orig;
    CxxEhRecord *prev;
    CxxEhFrame *f;

    exc = (CxxEhRecord *)CxxEhAlloc(0xa4);
    exc->next = (CxxEhRecord *)g_cxxEhCurrentException;
    g_cxxEhCurrentException = exc;
    exc->type = type;
    exc->dtor = (void (*)(void *, int))dtor;
    exc->flags = flags;
    exc->extra = (u32)a3;
    exc->extra2 = (u32)a4;
    exc->flags2 = a5;
    exc->object = object;
    exc->objectCopy = NULL;
    exc->orig = origRec;
    exc->useCount = 0;
    if (isRethrow != 0) {
        origRec->useCount++;
    } else {
        exc->useCount++;
    }
    prev = exc->next;
    exc->isRethrow = (u8)isRethrow;
    exc->destroyed = 0;
    exc->released = 0;
    exc->active = 0;
    exc->constructed = 0;
    exc->pushed = 0;
    exc->frame.next = NULL;
    exc->frame.kind = 3;

    for (f = (CxxEhFrame *)g_cxxEhFrameStack; f != NULL; f = f->next) {
        if (f->kind == 0 && ((CxxEhTryFrame *)f)->caught == NULL) {
            break;
        }
    }
    exc->tryFrame = f;
    if (prev != NULL && prev->tryFrame == f) {
        CxxEhReleaseException(exc);
    }
}
