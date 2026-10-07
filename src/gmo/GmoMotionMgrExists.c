// bdc 0x089d9650 GmoMotionMgrExists
#include "bdc.h"

/* Returns true when the motion manager singleton `g_gmoMotionMgr` exists. */

bool GmoMotionMgrExists(void)
{
  return g_gmoMotionMgr != 0;
}
