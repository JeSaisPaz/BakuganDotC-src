// bdc 0x089b5208 strtok
#include "bdc.h"

/* Standard strtok: strtok_r with the save pointer kept in the global reent's _strtok_last. */
char *strtok(char *s, char *delim)
{
  return strtok_r(s, delim, &g_impurePtr->_strtok_last);
}
