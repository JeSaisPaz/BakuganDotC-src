// bdc 0x08a0f8a0 abort
#include "bdc.h"

/* Abnormal termination: calls _exit(1), looping forever in case it returns. */
void abort(void)
{
  for (;;) {
    _exit(1);
  }
}
