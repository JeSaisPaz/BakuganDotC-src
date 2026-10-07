// bdc 0x088be214 GameFieldTaskExists
#include "bdc.h"

/* Returns whether the field task (id 500) exists (`CoreTaskExists`). */

s16 GameFieldTaskExists(void)
{
  return (s16)CoreTaskExists(500);
}
