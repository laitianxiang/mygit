/*
*********************************************************************************************************
*                                 CRC  FUNCTIONS OF APP
*
*
*                               (c) Copyright 2019-01-03w
*
* File    : crc.C
* By      : tianxiang_lai@KC
* Version : V0.1
*
* LICENSING TERMS:
* ---------------
* crc.C is implement CRC Check function related application functions for system.
*
*********************************************************************************************************
*/

#include "includes.h"

/*
文件内部宏定义
*/
#define E_POLYNUMIAL               0x1021
#define E_INITIAL_REMAINDER        0x0000
#define E_FINAL_XOR_VALUE          0x0000
#define E_WIDTH (8 * sizeof(width_t))
#define E_TOPBIT (1 << (E_WIDTH - 1))
#define E_TOPBITINVERT             0x0001

static width_t crctable[256];
static void Crc_InvertUint16(u16 *, u16 );

/*------------------------------------------------------------------------------------------------------
********************************************************************************************************
*                                        函数检证
* 函数名称     :   Crc_InvertUint16
* 功能说明     :   16位数据格式反向转换函数
* 作者         :   ltx
* 日期         :   2019-01-02
* 参数说明     :   u16 *Desbuf 输出值地址
                   u16 SrcBuf  输入值数据
* 返回值       :   无
* 输出说明     :   u16 *Desbuf 保存转换后的数据值
* 检证结果     :   OK
*********************************************************************************************************
-------------------------------------------------------------------------------------------------------*/
static void Crc_InvertUint16(u16 *Desbuf, u16 SrcBuf)
{
    u8 i;
    u16 temp = 0;

    for ( i = 0; i < 16; i++ ){
        if ( SrcBuf & (1 << i) ){
            temp |= 1 << (15 - i);
        }
    }

    *Desbuf = temp;
}

/*------------------------------------------------------------------------------------------------------
********************************************************************************************************
*                                        函数检证
* 函数名称     :   Crc_InitBuildTable16
* 功能说明     :   Crc正向检测算法表格初始化创建
* 作者         :   ltx
* 日期         :   2019-01-02
* 参数说明     :   无
* 返回值       :   无
* 输出说明     :   保存256数据到crctable表格
* 检证结果     :   OK
*********************************************************************************************************
-------------------------------------------------------------------------------------------------------*/
void  Crc_InitBuildTable16(void)
{
    width_t remainder;
    width_t dividend;
    u8 bit;

    for ( dividend = 0; dividend < 256; dividend++ ){
        remainder = dividend << (E_WIDTH - 8);

        for ( bit = 0; bit < 8; bit++ ){
            if ( remainder & E_TOPBIT ){
                remainder = (remainder << 1) ^ E_POLYNUMIAL;
            }else{
                remainder = remainder << 1;
            }
        }

        crctable[dividend] = remainder;
    }

}

/*------------------------------------------------------------------------------------------------------
********************************************************************************************************
*                                        函数检证
* 函数名称     :   Crc_ComputeProcess16
* 功能说明     :   Crc正向检测查表算法
* 作者         :   ltx
* 日期         :   2019-01-02
* 参数说明     :   u8 *buff  检测数据帧缓存buff
                   u32 size  数据帧长度
* 返回值       :   width_t CRC检测结果
* 输出说明     :   width_t CRC检测结果
* 检证结果     :   OK
*********************************************************************************************************
-------------------------------------------------------------------------------------------------------*/
width_t  Crc_ComputeProcess16(u8 *buff, u32 size)
{
    u8  byte;
    u32 offset;
    width_t remainder = E_INITIAL_REMAINDER;

    for ( offset = 0; offset < size; offset++ ){
        byte = (remainder >> (E_WIDTH - 8)) ^ buff[offset];
        remainder = crctable[byte] ^ (remainder << 8);
    }

    return (remainder ^ E_FINAL_XOR_VALUE);
}

/*------------------------------------------------------------------------------------------------------
********************************************************************************************************
*                                        函数检证
* 函数名称     :   Crc_InitBuildTableInversion16
* 功能说明     :   Crc反向检测算法表格初始化创建
* 作者         :   ltx
* 日期         :   2019-01-02
* 参数说明     :   无
* 返回值       :   无
* 输出说明     :   保存256数据到crctable表格
* 检证结果     :   OK
*********************************************************************************************************
-------------------------------------------------------------------------------------------------------*/
void  Crc_InitBuildTableInversion16(void)
{
    width_u16 i,j;
    width_u16 nData,nAccum;
    width_u16 nPolyInvert;

    Crc_InvertUint16(&nPolyInvert,E_POLYNUMIAL);

    for ( i = 0; i < 256; i++ ){
        nData = i;
        nAccum  = 0;

        for ( j = 0; j < 8; j++ ){
            if ( (nData ^ nAccum) & E_TOPBITINVERT ){
                nAccum = (nAccum >> 1) ^ nPolyInvert;
            }else{
                nAccum >>= 1;
            }
            nData >>= 1;
        }

        crctable[i] = nAccum;
    }
}

/*------------------------------------------------------------------------------------------------------
********************************************************************************************************
*                                        函数检证
* 函数名称     :   Crc_ComputeProcessInversion16
* 功能说明     :   Crc反向检测查表算法
* 作者         :   ltx
* 日期         :   2019-01-02
* 参数说明     :   u8 *buff  检测数据帧缓存buff
                   u32 size  数据帧长度
* 返回值       :   width_t CRC检测结果
* 输出说明     :   width_t CRC检测结果
* 检证结果     :   OK
*********************************************************************************************************
-------------------------------------------------------------------------------------------------------*/
width_t  Crc_ComputeProcessInversion16(u8 *buff, u32 size)
{
    width_u16 i;
    width_u16 nAccum;

    nAccum = E_INITIAL_REMAINDER;
    for ( i = 0; i < size; i++ ){
        nAccum = (nAccum >> 8) ^ crctable[(nAccum & 0x00ff) ^ *buff++];
    }

    return (nAccum ^ E_FINAL_XOR_VALUE);
}

void     Crc_Init32(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
    CRC_ResetDR();
}


