// bdc 0x08897d00 BtlAiRangeConstsStaticInit
#include "bdc.h"

/* Static initialiser of the CPU AI translation unit: fills `g_btlAiRangeConsts` with the
   ranges 1500, 2000, 30, 60, 1000 and 350 and the three detour waypoints (20, 0, 723, 0),
   (20, 0, 2028, 0) and (20, 0, 3333, 0); bytes `+0x18..+0x1f` are left untouched. */
void BtlAiRangeConstsStaticInit(void)
{
    BtlAiRangeConsts *c = &g_btlAiRangeConsts;

    c->detourWaypoints[0][0] = 20.0f;
    c->detourWaypoints[0][1] = 0.0f;
    c->pickRange = 1500.0f;
    c->followScatter = 2000.0f;
    c->rayLowHeight = 30.0f;
    c->rayHighHeight = 60.0f;
    c->threatRange = 1000.0f;
    c->rayHeadClearance = 350.0f;
    c->detourWaypoints[0][2] = 723.0f;
    c->detourWaypoints[0][3] = 0.0f;
    c->detourWaypoints[1][0] = 20.0f;
    c->detourWaypoints[1][1] = 0.0f;
    c->detourWaypoints[1][2] = 2028.0f;
    c->detourWaypoints[1][3] = 0.0f;
    c->detourWaypoints[2][0] = 20.0f;
    c->detourWaypoints[2][1] = 0.0f;
    c->detourWaypoints[2][2] = 3333.0f;
    c->detourWaypoints[2][3] = 0.0f;
}
