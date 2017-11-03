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
#include "toothbrush.h"

#define X_AXIS_THRESHOLD               (0)
#define Y_AXIS_THRESHOLD               (0)
#define Z_AXIS_THRESHOLD               (0)

#define ACCE_N2G                       (0)
#define ACCE_N1G                       (16384)
#define ACCE_0G                        (32768)
#define ACCE_P1G                       (49512)
#define ACCE_P2G                       (65535)
#define ACCE_THRESHOLD                 (3000)

int g_region_time_cnt[TOOSH_REGION_NUM];

int get_region(int gx, int gy, int gz)
{
	/* up-right */
	if ((gx >= ACCE_N1G-ACCE_THRESHOLD) && (gx <= ACCE_N1G+ACCE_THRESHOLD) &&
		(gy >= ACCE_0G-ACCE_THRESHOLD) && (gy <= ACCE_0G-ACCE_THRESHOLD) && 
		(gz >= ACCE_0G-ACCE_THRESHOLD) && (gz >= ACCE_0G-ACCE_THRESHOLD))
	{
		return INIT_UPRIGHT;
	}
	return 0;
}

int calc_region(short gx, short gy, short gz, int roll, int pitch, int yaw)
{
	static int m_roll  = 0;
	static int m_pitch = 0;
	static int m_yaw   = 0;

	
	return 0;
}