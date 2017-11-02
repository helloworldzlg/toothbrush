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

typedef enum
{
	BRUSH_LEFT  = 0,
	BRUSH_RIGHT = 1,
}TOOTHBRUSH_DIR_E;

typedef enum
{
	LEFT_EXTERNAL    = 0,
	LEFT_INTERNAL    = 1,
	LEFT_UPSIDE      = 2,
	LEFT_DOWNSIDE    = 3,
	RIGHT_EXTERNAL   = 4,
	RIGHT_INTERNAL   = 5,
	RIGHT_UPSIDE     = 6,
	LRIGHT_DOWNSIDE  = 7,
	TOOSH_REGION_NUM = 8,
}TOOTH_BRUSH_REGION_E;

typedef enum
{
	INIT_UPRIGHT     = 9,
}TOOTH_BRUSH_STATUS_E;


#endif
