/**
 * @file dev_sfc.c
 * @author lik
 * @brief sfc相关配置函数
 * @version 0.1
 * @date 2022-01-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "SWM341.h"

/**
 * @brief sfc初始化
 *
 */
void sfc_init(void)
{
    SFC_InitStructure SFC_initStruct;

    /** SFC使用专用的FSPI（Flash SPI）接口连接SPI Flash */
    PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK, 0);
    PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL, 0);
    PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI, 1);
    PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO, 1);
    PORT_Init(PORTD, PIN3, PORTD_PIN3_FSPI_DATA2, 1);
    PORT_Init(PORTD, PIN4, PORTD_PIN4_FSPI_DATA3, 1);

    SFC_initStruct.ClkDiv = SFC_CLKDIV_2;
    SFC_initStruct.Cmd_Read = 0xEB;
    SFC_initStruct.Width_Read = SFC_RDWIDTH_4;
    SFC_initStruct.Cmd_PageProgram = 0x32;
    SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_4;

    SFC_Init(&SFC_initStruct);
    if (!SFC_QuadState())
        SFC_QuadSwitch(1);
}
