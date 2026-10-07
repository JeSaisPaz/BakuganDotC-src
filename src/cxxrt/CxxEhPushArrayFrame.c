// bdc 0x08a029d8 CxxEhPushArrayFrame
#include "bdc.h"

/* Pushes an array-construction frame (kind 4) on the exception-handling frame stack
   (`g_cxxEhFrameStack`, frames `{next, u8 kind, ...}`: 0 try block, 1 cleanup region, 2 exception
   specification, 3 no-throw, 4 array construction): links `frame` in, points it at the array record
   `rec` and zeroes the record (`rec->flag` = `flag`). Used by `CxxVecNewEx` and `CxxVecDeleteEx`
   so that a throwing element constructor/destructor destroys the finished elements
   (`CxxEhCleanupArray`). */

void CxxEhPushArrayFrame(void *frame, void *rec, u32 flag)

{
  CxxEhArrayFrame *f = (CxxEhArrayFrame *)frame;
  CxxEhArrayRec *r = (CxxEhArrayRec *)rec;

  f->next = (CxxEhArrayFrame *)g_cxxEhFrameStack;
  g_cxxEhFrameStack = frame;
  f->kind = 4;
  f->rec = r;
  r->f00 = 0;
  r->f04 = 0;
  r->f08 = 0;
  r->f0c = 0;
  r->flag = flag;
  r->f14 = 0;
  r->f18 = 0;
  r->f1c = 0;
  r->f20 = 0;
}
