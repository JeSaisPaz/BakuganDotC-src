// bdc 0x0889b5a4 BtlAiCreateKindParams
#include "bdc.h"

/* Factory of the per-species AI parameter object: picks the derived vtable of `kind` 1..32
   (one class per kind, `BtlAiParams01Dragonoid` .. `BtlAiParams32Wired`), allocates 8 bytes from the
   low end of the heap under `MemLock` (restoring the previous placement policy afterwards), and
   on success stores the base vtable `g_btlAiParamsVtable`, then `kind`, then the derived vtable
   over it. Any other `kind` keeps the base vtable (the object is still allocated and returned).
   Returns NULL only when the allocation fails. */
BtlAiParams *BtlAiCreateKindParams(void *mgr, s32 kind)
{
    VtblEntry *derived;
    BtlAiParams *params;
    bool wasLow;

    (void)mgr;
    switch (kind) {
    case 1:
        derived = (VtblEntry *)g_btlAiParams01DragonoidVtable;
        break;
    case 2:
        derived = (VtblEntry *)g_btlAiParams02DragonoidVtable;
        break;
    case 3:
        derived = (VtblEntry *)g_btlAiParams03IngramVtable;
        break;
    case 4:
        derived = (VtblEntry *)g_btlAiParams04IngramVtable;
        break;
    case 5:
        derived = (VtblEntry *)g_btlAiParams05ElfinVtable;
        break;
    case 6:
        derived = (VtblEntry *)g_btlAiParams06PercivalVtable;
        break;
    case 7:
        derived = (VtblEntry *)g_btlAiParams07WildaVtable;
        break;
    case 8:
        derived = (VtblEntry *)g_btlAiParams08NemusVtable;
        break;
    case 9:
        derived = (VtblEntry *)g_btlAiParams09HeliosVtable;
        break;
    case 10:
        derived = (VtblEntry *)g_btlAiParams10HeliosVtable;
        break;
    case 11:
        derived = (VtblEntry *)g_btlAiParams11ElicoVtable;
        break;
    case 12:
        derived = (VtblEntry *)g_btlAiParams12BrontesVtable;
        break;
    case 13:
        derived = (VtblEntry *)g_btlAiParams13VulcanVtable;
        break;
    case 14:
        derived = (VtblEntry *)g_btlAiParams14HadesVtable;
        break;
    case 15:
        derived = (VtblEntry *)g_btlAiParams15AltairVtable;
        break;
    case 16:
        derived = (VtblEntry *)g_btlAiParams16DragonoidVtable;
        break;
    case 17:
        derived = (VtblEntry *)g_btlAiParams17ElfinVtable;
        break;
    case 18:
        derived = (VtblEntry *)g_btlAiParams18PercivalVtable;
        break;
    case 19:
        derived = (VtblEntry *)g_btlAiParams19WildaVtable;
        break;
    case 20:
        derived = (VtblEntry *)g_btlAiParams20NemusVtable;
        break;
    case 21:
        derived = (VtblEntry *)g_btlAiParams21ScorpionVtable;
        break;
    case 22:
        derived = (VtblEntry *)g_btlAiParams22HylashVtable;
        break;
    case 23:
        derived = (VtblEntry *)g_btlAiParams23EpsilonVtable;
        break;
    case 24:
        derived = (VtblEntry *)g_btlAiParams24FalconflyVtable;
        break;
    case 25:
        derived = (VtblEntry *)g_btlAiParams25BalitonVtable;
        break;
    case 26:
        derived = (VtblEntry *)g_btlAiParams26PiercianVtable;
        break;
    case 27:
        derived = (VtblEntry *)g_btlAiParams27MetalfencerVtable;
        break;
    case 28:
        derived = (VtblEntry *)g_btlAiParams28ThetaVtable;
        break;
    case 29:
        derived = (VtblEntry *)g_btlAiParams29DynamoVtable;
        break;
    case 30:
        derived = (VtblEntry *)g_btlAiParams30HexadosVtable;
        break;
    case 31:
        derived = (VtblEntry *)g_btlAiParams31FortressVtable;
        break;
    case 32:
        derived = (VtblEntry *)g_btlAiParams32WiredVtable;
        break;
    default:
        derived = NULL;
        break;
    }

    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    params = MemAlloc(8, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (params == NULL) {
        return NULL;
    }
    params->vtbl = (VtblEntry *)g_btlAiParamsVtable;
    params->kind = kind;
    if (derived != NULL) {
        params->vtbl = derived;
    }
    return params;
}
