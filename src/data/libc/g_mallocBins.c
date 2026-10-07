// bdc 0x08ac4718 g_mallocBins
#include "bdc.h"

__typeof__(MallocState) g_mallocBins = {
    .bins = {
        { .fd = (struct MallocChunk *)&g_mallocBins, .bk = (struct MallocChunk *)&g_mallocBins },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins,
            .bk = (struct MallocChunk *)&g_mallocBins.bins,
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[1],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[1],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[2],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[2],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[3],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[3],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[4],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[4],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[5],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[5],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[6],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[6],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[7],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[7],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[8],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[8],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[9],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[9],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[10],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[10],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[11],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[11],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[12],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[12],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[13],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[13],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[14],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[14],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[15],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[15],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[16],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[16],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[17],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[17],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[18],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[18],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[19],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[19],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[20],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[20],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[21],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[21],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[22],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[22],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[23],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[23],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[24],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[24],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[25],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[25],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[26],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[26],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[27],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[27],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[28],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[28],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[29],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[29],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[30],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[30],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[31],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[31],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[32],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[32],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[33],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[33],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[34],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[34],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[35],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[35],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[36],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[36],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[37],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[37],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[38],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[38],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[39],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[39],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[40],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[40],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[41],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[41],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[42],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[42],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[43],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[43],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[44],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[44],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[45],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[45],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[46],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[46],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[47],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[47],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[48],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[48],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[49],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[49],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[50],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[50],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[51],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[51],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[52],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[52],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[53],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[53],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[54],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[54],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[55],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[55],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[56],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[56],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[57],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[57],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[58],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[58],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[59],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[59],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[60],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[60],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[61],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[61],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[62],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[62],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[63],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[63],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[64],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[64],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[65],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[65],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[66],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[66],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[67],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[67],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[68],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[68],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[69],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[69],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[70],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[70],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[71],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[71],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[72],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[72],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[73],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[73],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[74],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[74],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[75],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[75],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[76],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[76],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[77],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[77],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[78],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[78],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[79],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[79],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[80],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[80],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[81],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[81],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[82],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[82],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[83],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[83],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[84],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[84],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[85],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[85],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[86],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[86],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[87],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[87],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[88],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[88],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[89],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[89],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[90],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[90],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[91],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[91],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[92],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[92],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[93],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[93],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[94],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[94],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[95],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[95],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[96],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[96],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[97],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[97],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[98],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[98],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[99],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[99],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[100],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[100],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[101],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[101],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[102],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[102],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[103],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[103],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[104],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[104],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[105],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[105],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[106],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[106],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[107],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[107],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[108],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[108],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[109],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[109],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[110],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[110],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[111],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[111],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[112],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[112],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[113],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[113],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[114],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[114],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[115],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[115],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[116],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[116],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[117],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[117],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[118],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[118],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[119],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[119],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[120],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[120],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[121],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[121],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[122],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[122],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[123],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[123],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[124],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[124],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[125],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[125],
        },
        {
            .fd = (struct MallocChunk *)&g_mallocBins.bins[126],
            .bk = (struct MallocChunk *)&g_mallocBins.bins[126],
        },
    },
};
