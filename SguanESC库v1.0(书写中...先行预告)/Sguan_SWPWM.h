#ifndef __SGUAN_SWPWM_H
#define __SGUAN_SWPWM_H

/* SguanESC配置文件声明 */
#include "Sguan_Config.h"

void SWPWM_Tick(uint8_t *sector, 
                float Umid, 
                float Ua, 
                float Ub, 
                float Uc, 
                uint8_t flag);


#endif // SGUAN_SWPWM_H
