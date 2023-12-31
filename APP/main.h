/**
 * @file main.h
 * @author lik
 * @brief 
 * @version 0.1
 * @date 2022-01-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "SWM341.h"
#include "dev_gt9x.h"
#include "dev_rgb.h"
#include "dev_sdio.h"
#include "dev_sdram.h"
#include "dev_sfc.h"
#include "dev_systick.h"
#include "dev_uart.h"

//352K+128K=480K
#define EEPROM_ADDR	   0x75000

//#define LV_HOR_RES_MAX (400)
//#define LV_VER_RES_MAX (960)
#define LV_HOR_RES_MAX (480)
#define LV_VER_RES_MAX (800)
#define TEXT_RED "\x1B[1;31m"
#define TEXT_GREEN "\x1B[1;32m"
#define TEXT_YELLOW "\x1B[1;33m"

/* 调试信息 */
#define _DEBUG
#ifdef _DEBUG
#define debug(...) printf(TEXT_GREEN __VA_ARGS__)
#define debugwarn(...) printf(TEXT_YELLOW __VA_ARGS__)
#define debugerror(...) printf(TEXT_RED __VA_ARGS__)

// #define debug(...) printf(__VA_ARGS__)
// #define debugwarn(...) printf(__VA_ARGS__)
// #define debugerror(...) printf(__VA_ARGS__)

#else
#define debug(...) ;
#define debugwarn(...) ;
#define debugerror(...) ;
#endif //_DEBUG

#endif //__MAIN_H__
