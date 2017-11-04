/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : toothbrush.h
 * Author        : zhangligui
 * Date          : 2017-10-28
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2017-10-28
 *   Author      : zhangligui
 *   Modification: Created file

*************************************************************************************************************/
#ifndef __TOOTHBRUSH_H__
#define __TOOTHBRUSH_H__
#include <stdio.h>

typedef enum
{
	BRUSH_LEFT  = 0,
	BRUSH_RIGHT = 1,
}TOOTHBRUSH_DIR_E;

typedef enum
{
	INIT_UPRIGHT     = 0,
	LEFT_EXTERNAL    = 1,
	LEFT_INTERNAL    = 2,
	LEFT_UPSIDE      = 3,
	LEFT_DOWNSIDE    = 4,
	RIGHT_EXTERNAL   = 5,
	RIGHT_INTERNAL   = 6,
	RIGHT_UPSIDE     = 7,
	RIGHT_DOWNSIDE   = 8,
	
	TOOSH_REGION_NUM = 9,
}TOOTH_BRUSH_REGION_E;



int calc_region(short gx, short gy, short gz, int roll, int pitch, int yaw);



#endif
