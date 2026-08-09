#ifndef __SGUAN_GIC_H
#define __SGUAN_GIC_H

/* SguanESC配置文件声明 */
#include "Sguan_Config.h"

typedef struct{
    float Ref;
    float Fbk;
    float Output;

    float __Error;
}__GIC_GO_STRUCT;

typedef struct{
    __GIC_GO_STRUCT run;

    float K;
    float boundary;

    float OutMax;
    float OutMin;
}GIC_STRUCT;

void GIC_Init(GIC_STRUCT *gic);
void GIC_Loop(GIC_STRUCT *gic);


#endif // SGUAN_GIC_H
