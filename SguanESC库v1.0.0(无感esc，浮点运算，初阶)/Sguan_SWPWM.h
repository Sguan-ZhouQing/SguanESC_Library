#ifndef __SGUAN_SWPWM_H
#define __SGUAN_SWPWM_H

/* SguanESC配置文件声明 */
#include "Sguan_Config.h"

void SWPWM(int8_t *sector, 
        uint32_t Duty,
        float D_ubus_in,
        float Umid, 
        float Ua, 
        float Ub, 
        float Uc);


#endif // SGUAN_SWPWM_H
