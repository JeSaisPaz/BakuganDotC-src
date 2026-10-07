// bdc 0x08a02718 CxxGetMessage
#include "bdc.h"

/* Maps a C++ runtime error code to its message text: 1 "\nC++ runtime abort", 2 "terminate() called
   by the exception handling mechanism", 3 "returned from a user-defined terminate() routine", 4
   "internal error: static object marked for destruction more than once", 5 "main() called more than
   once", 6 "a pure virtual function was called", 7 "invalid dynamic cast", 8 "invalid typeid
   operation", 9 "freeing array not allocated by an array new operation"; any other code gives NULL.
    */

const char *CxxGetMessage(int code)

{
  const char *msg;

  msg = (const char *)0x0;
  switch(code) {
  case 1:
    msg = "\nC++ runtime abort";
    break;
  case 2:
    msg = "terminate() called by the exception handling mechanism";
    break;
  case 3:
    msg = "returned from a user-defined terminate() routine";
    break;
  case 4:
    msg = "internal error: static object marked for destruction more than once";
    break;
  case 5:
    msg = "main() called more than once";
    break;
  case 6:
    msg = "a pure virtual function was called";
    break;
  case 7:
    msg = "invalid dynamic cast";
    break;
  case 8:
    msg = "invalid typeid operation";
    break;
  case 9:
    msg = "freeing array not allocated by an array new operation";
  }
  return msg;
}
