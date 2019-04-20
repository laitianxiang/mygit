#ifndef __CRC_H
#define __CRC_H

#include "sys.h"

typedef u16 width_t;
typedef u16 width_u16;

width_t  Crc_ComputeProcess16(u8 *,u32);
void     Crc_InitBuildTable16(void);
void     Crc_InitBuildTableInversion16(void);
width_t  Crc_ComputeProcessInversion16(u8 *,u32);
void     Crc_Init32(void);

#endif
