// bdc 0x0884d070 BtlGetStagePoint
#include "bdc.h"

/* Copies point `index` of the current stage's row in point table `table`
   (`g_btlStagePointTables`, row = script global variable 1, the stage number) into `out`,
   clamping `index` to the row's last point; zeroes `out` when the row address is NULL. */
void BtlGetStagePoint(void *main, float *out, int index, int table)
{
    const BtlStagePointSet *row;
    ScePspFVector4 tmp;

    (void)main;
    row = &g_btlStagePointTables[table][g_scriptGlobalVars[1]];
    if (row == NULL) {
        out[2] = 0.0f;
        out[1] = 0.0f;
        out[0] = 0.0f;
        out[3] = 0.0f;
        return;
    }
    if (row->count - 1 < index) {
        index = row->count - 1;
    }
    /* quad copy through a stack temp */
    tmp = row->points[index];
    out[0] = tmp.x;
    out[1] = tmp.y;
    out[2] = tmp.z;
    out[3] = tmp.w;
}
