// bdc 0x088aa56c ActorStageObjGetKindMaxHp
#include "bdc.h"

/* Returns the HP of a stage-object kind as a float: 1000 for landmarks (category 5), 1 for
   category 3 (one hit), otherwise a per-kind value (e.g. kind 1 300, 2 230, 3 100, 0xb 250,
   0x31 350, 0x4a..0x55 250, 0x68 210), with 1 for kinds not listed (including 0x6c and kinds
   outside 1..0xba). `ActorStageObjBaseInit` stores it as HP/max HP `+0x200/+0x204`. */

float ActorStageObjGetKindMaxHp(ActorStageObjBase *self, int kind)
{
    int category = ActorStageObjGetCategory(kind);

    if (category < 4) {
        if (category >= 3) {
            return 1.0f;
        }
    } else if (category == 5) {
        return 1000.0f;
    }

    switch (kind) {
    case 0x01:
    case 0x04:
        return 300.0f;
    case 0x02:
        return 230.0f;
    case 0x31:
        return 350.0f;
    case 0x0b:
    case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:
        return 250.0f;
    case 0x68:
        return 210.0f;
    case 0x05: case 0x07: case 0x08: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    case 0x2e: case 0x2f: case 0x30: case 0x32: case 0x33: case 0x34: case 0x35:
    case 0x3a: case 0x6e: case 0x72:
    case 0x7d: case 0x7e: case 0x7f: case 0x80: case 0x81: case 0x82:
    case 0x85: case 0x87:
        return 200.0f;
    case 0x66:
        return 120.0f;
    case 0x03: case 0x06: case 0x09: case 0x0a: case 0x10: case 0x11: case 0x12:
    case 0x2d: case 0x6b: case 0x7c: case 0x8b: case 0x8c: case 0x99: case 0x9a:
    case 0xa7: case 0xba:
        return 100.0f;
    case 0x6a:
        return 50.0f;
    case 0x65:
    case 0x67:
        return 10.0f;
    default:
        return 1.0f;
    }
}
