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

#define ACCE_N2G                       (-32768)
#define ACCE_N1G                       (-16384)
#define ACCE_0G                        (0)
#define ACCE_P1G                       (16384)
#define ACCE_P2G                       (32768)
#define ACCE_THRESHOLD                 (3000)

int g_region_time_cnt[TOOTH_REGION_NUM];

int get_region(int gx, int gy, int gz)
{
	#if 0
	/* up-right */
	if ((gx >= ACCE_1G-ACCE_THRESHOLD) && (gx <= ACCE_1G+ACCE_THRESHOLD) &&
		(gy >= ACCE_0G-ACCE_THRESHOLD) && (gy <= ACCE_0G+ACCE_THRESHOLD) && 
		(gz >= ACCE_0G-ACCE_THRESHOLD) && (gz >= ACCE_0G+ACCE_THRESHOLD))
	{
		return INIT_UPRIGHT;
	}

	/* left upside */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= ACCE_0G-ACCE_THRESHOLD) && (gy <= ACCE_0G+ACCE_THRESHOLD) && 
		(gz >= ACCE_1G-ACCE_THRESHOLD) && (gz >= ACCE_1G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/* left external */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= -ACCE_1G-ACCE_THRESHOLD) && (gy <= -ACCE_1G+ACCE_THRESHOLD) && 
		(gz >= ACCE_0G-ACCE_THRESHOLD) && (gz >= ACCE_0G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/* left downside */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= ACCE_0G-ACCE_THRESHOLD) && (gy <= ACCE_0G+ACCE_THRESHOLD) && 
		(gz >= -ACCE_1G-ACCE_THRESHOLD) && (gz >= -ACCE_1G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/* left internal */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= ACCE_1G-ACCE_THRESHOLD) && (gy <= ACCE_1G+ACCE_THRESHOLD) && 
		(gz >= ACCE_0G-ACCE_THRESHOLD) && (gz >= ACCE_0G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/* right upside */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= ACCE_0G-ACCE_THRESHOLD) && (gy <= ACCE_0G+ACCE_THRESHOLD) && 
		(gz >= ACCE_1G-ACCE_THRESHOLD) && (gz >= ACCE_1G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/*
	 right external */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= ACCE_1G-ACCE_THRESHOLD) && (gy <= ACCE_1G+ACCE_THRESHOLD) && 
		(gz >= ACCE_0G-ACCE_THRESHOLD) && (gz >= ACCE_0G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/* right downside */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= ACCE_0G-ACCE_THRESHOLD) && (gy <= ACCE_0G+ACCE_THRESHOLD) && 
		(gz >= -ACCE_1G-ACCE_THRESHOLD) && (gz >= -ACCE_1G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}

	/* right internal */
	if ((gx >= ACCE_0G-ACCE_THRESHOLD) && (gx <= ACCE_0G+ACCE_THRESHOLD) &&
		(gy >= -ACCE_1G-ACCE_THRESHOLD) && (gy <= -ACCE_1G+ACCE_THRESHOLD) && 
		(gz >= ACCE_0G-ACCE_THRESHOLD) && (gz >= ACCE_0G+ACCE_THRESHOLD))
	{
		return LEFT_UPSIDE;
	}
	#endif
	return 0;
}

#define H_ORIENTATION_NUM              (4)
static float h_orientation[H_ORIENTATION_NUM];
static int h_orientation_count = 0;

int get_h_orientation(float ay)
{
	int ret;
#if 0	
	int i, pn_count = 0;
	h_orientation[h_orientation_count] = ay;
	h_orientation_count = (h_orientation_count+1)%H_ORIENTATION_NUM;

	for (i = 0; i < H_ORIENTATION_NUM; i++)
		(h_orientation[i] > 0) ? (pn_count++) : (pn_count=pn_count);
	if (pn_count >= H_ORIENTATION_NUM/2)
		return BRUSH_RIGHT;
	else
		return BRUSH_LEFT;
#endif
	(ay > 0) ? (ret = BRUSH_RIGHT)	: (ret = BRUSH_LEFT);
	return ret;
}

#define ACCRLERATE_RANGE            (0.25)

void print_region(TOOTH_BRUSH_REGION_E brush_region)
{
	switch(brush_region)
	{
		case INIT_UPRIGHT:
			printf("up right\n");
			break;

		case LEFT_OUTSIDE:
			printf("left outside\n");
			break;			

		case LEFT_INSIDE:
			printf("left inside\n");
			break;

		case LEFT_UPSIDE:
			printf("left upside\n");
			break;

		case LEFT_DOWNSIDE:
			printf("left downside\n");
			break;

		case RIGHT_OUTSIDE:
			printf("right outside\n");
			break;

		case RIGHT_INSIDE:
			printf("right inside\n");
			break;

		case RIGHT_UPSIDE:
			printf("right upside\n");
			break;	

		case RIGHT_DOWNSIDE:
			printf("right downside\n");
			break;		
	}
}

#define CHECK_RANGE             (90)
static int check_region_flag   = 0;
static float check_region_low  = 0;
static float check_region_high = 0;
static float check_region_yaw  = 0;

int h_orien_is_change(float yaw)
{
	if (0 == check_region_flag)
	{
		check_region_yaw  = yaw;
		check_region_flag = 1;

		check_region_low  = check_region_yaw-CHECK_RANGE;
		check_region_high = check_region_yaw+CHECK_RANGE;

		if (check_region_yaw-CHECK_RANGE < 0)
		{
			check_region_low  = check_region_high;
			check_region_high = check_region_yaw+360-CHECK_RANGE;
		}

		if (check_region_yaw+CHECK_RANGE > 360)
		{
			check_region_high = check_region_low;
			check_region_low  = check_region_yaw+CHECK_RANGE-360;
		}

		//printf("cyaw = %f, low = %f, high = %f\n", check_region_yaw, check_region_low, check_region_high);
		return T_FALSE;
	}
	else
	{
		printf("low = %f, high = %f, yaw = %f\n", check_region_low, check_region_high, yaw);
		if ((yaw < check_region_low) || (yaw > check_region_high))
		{
			printf("change region\n");
			check_region_flag = 0;
			return T_TRUE;
		}
	}
}

int judge_region(float ax, float ay, float az, float roll, float pitch, float yaw)
{
	int ret;
	TOOTH_BRUSH_REGION_E brush_region;
	static int in_flag = 0;
	static int h_orien = 0;
	
	static TOOTH_BRUSH_REGION_E curr_region, last_region = TOOTH_REGION_NUM;

	if ((ax > 1-ACCRLERATE_RANGE-0.5) && (ax < 1+ACCRLERATE_RANGE))
	{		
		curr_region = INIT_UPRIGHT;
		h_orien = get_h_orientation(ay);
	}

	if ((ay > -1-ACCRLERATE_RANGE) && (ay < -1+ACCRLERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_OUTSIDE;
		else
			curr_region = RIGHT_INSIDE;
	}

	if ((az > 1-ACCRLERATE_RANGE) && (az < 1+ACCRLERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_UPSIDE;
		else
			curr_region = RIGHT_UPSIDE;
	}

	if ((az > -1-ACCRLERATE_RANGE) && (az < -1+ACCRLERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_DOWNSIDE;
		else
			curr_region = RIGHT_DOWNSIDE;
	}

	if ((ay > 1-ACCRLERATE_RANGE) && (ay < 1+ACCRLERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_INSIDE;
		else
			curr_region = RIGHT_OUTSIDE;
	}

	if ((curr_region != INIT_UPRIGHT) && (curr_region == last_region))
	{
		ret = h_orien_is_change(yaw);
		//(ret == T_TRUE) ? (h_orien = (h_orien+1)%2) : (h_orien=h_orien);
	}
	// else
	// {
	// 	check_region_flag = 0;
	// }

	last_region = curr_region;

	print_region(curr_region);
	return 0;
}

#if 0
int calc_region(float ax, float ay, float az, float roll, float pitch, float yaw)
{
	static int m_roll  = 0;
	static int m_pitch = 0;
	static int m_yaw   = 0;
	static int m_chang = 0;
	static int last_region = -1;
	static int l_external_roll  = 0;
	static int l_external_pitch = 0;
	static int l_external_yaw   = 0;
	static int r_external_roll  = 0;
	static int r_external_pitch = 0;
	static int r_external_yaw   = 0;
	TOOTH_BRUSH_REGION_E curr_region;
	int m_dir = 0;

	//printf("gx = %d, gy = %d, gz = %d\n", gx, gy, gz);
	//printf("r = %d, p = %d, y = %d\n", roll, pitch, yaw);
	//printf("curren_region = %d\n", last_region);
	switch (last_region)
	{
		case INIT_UPRIGHT:
			printf("init up-right\n");
			break;

		case LEFT_EXTERNAL:
			printf("left external\n");
			break;			

		case LEFT_INTERNAL:
			printf("left internal\n");
			break;

		case LEFT_UPSIDE:
			printf("left upside\n");
			break;

		case LEFT_DOWNSIDE:
			printf("left downside\n");
			break;

		case RIGHT_EXTERNAL:
			printf("right external\n");
			break;

		case RIGHT_INTERNAL:
			printf("right internal\n");
			break;

		case RIGHT_UPSIDE:
			printf("right upside\n");
			break;	

		case RIGHT_DOWNSIDE:
			printf("right downside\n");
			break;

		default:
			//printf("error\n");
			break;
	}

	if ((ax >= ACCE_P1G-1000) && (ax <= ACCE_P1G+1000) &&
		(gy >= ACCE_0G-1000) && (gy <= ACCE_0G+1000) &&
		(gz >= ACCE_0G-1000) && (gz <= ACCE_0G+1000))
	{		
		curr_region = INIT_UPRIGHT;

		if (last_region != curr_region)
		{
			m_roll  = roll;
			m_pitch = pitch;
			m_yaw   = yaw;

			last_region = curr_region;
		}
	}
	else
	{
		/* 判断左右 */
		if ((yaw >= 180) && (yaw < 360))
		{
			//printf("left region: m_r = %d, m_p = %d, m_y = %d\n", m_roll, m_pitch, m_yaw);			
			
			printf("m_r = %d, m_p = %d, m_y = %d\n\n", l_external_roll, l_external_pitch, l_external_yaw);

			if ((gx >= ACCE_0G-3000) && (gx <= ACCE_0G+3000) &&
				(gy >= ACCE_N1G-3000) && (gy <= ACCE_N1G+3000) &&
				(gz >= ACCE_0G-5000) && (gz <= ACCE_0G+5000))
			{
				//printf("left external: m_r = %d, m_p = %d, m_y = %d\n", m_roll, m_pitch, m_yaw);
				curr_region = LEFT_EXTERNAL;

				if (last_region != curr_region)
				{
					l_external_roll  = roll;
					l_external_pitch = pitch;
					l_external_yaw   = yaw;

					last_region = curr_region;
				}
			}

			if ((roll >= l_external_roll+90-10) && (roll <= l_external_roll+90+10) && 
				(pitch >= l_external_pitch-25) && (pitch <= l_external_pitch+25) && 
				(yaw >= l_external_yaw-40) && (yaw <= l_external_yaw+40))
			{
				//printf("left upside: m_r = %d, m_p = %d, m_y = %d\n", m_roll, m_pitch, m_yaw);
				curr_region = LEFT_UPSIDE;

				if (last_region != curr_region)
				{
					m_roll  = roll;
					m_pitch = pitch;
					m_yaw   = yaw;

					last_region = curr_region;
				}
			}

			if ((roll >= l_external_roll-90-10) && (roll <= l_external_roll-90+10) && 
				(pitch >= l_external_pitch-25) && (pitch <= l_external_pitch+25) && 
				(yaw >= l_external_yaw-40) && (yaw <= l_external_yaw+40))
			{
				//printf("left downside: m_r = %d, m_p = %d, m_y = %d\n", m_roll, m_pitch, m_yaw);
				curr_region = LEFT_DOWNSIDE;

				if (last_region != curr_region)
				{
					m_roll  = roll;
					m_pitch = pitch;
					m_yaw   = yaw;

					last_region = curr_region;
				}
			}

			if ((roll >= l_external_roll+180-10) && (roll <= l_external_roll+180+10) && 
				(pitch >= l_external_pitch-25) && (pitch <= l_external_pitch+25) && 
				(yaw >= l_external_yaw-40) && (yaw <= l_external_yaw+40))
			{
				//printf("left downside: m_r = %d, m_p = %d, m_y = %d\n", m_roll, m_pitch, m_yaw);
				curr_region = LEFT_INTERNAL;

				if (last_region != curr_region)
				{
					m_roll  = roll;
					m_pitch = pitch;
					m_yaw   = yaw;

					last_region = curr_region;
				}
			}			
		}
		else
		{			
			printf("m_r = %d, m_p = %d, m_y = %d\n\n", r_external_roll, r_external_pitch, r_external_yaw);

			if ((gx >= ACCE_0G-3000) && (gx <= ACCE_0G+3000) &&
				(gy >= ACCE_P1G-3000) && (gy <= ACCE_P1G+3000) &&
				(gz >= ACCE_0G-5000) && (gz <= ACCE_0G+5000))
			{
				curr_region = RIGHT_EXTERNAL;

				if (last_region != curr_region)
				{
					r_external_roll  = roll;
					r_external_pitch = pitch;
					r_external_yaw   = yaw;

					last_region = curr_region;
				}
			}

			if ((roll >= r_external_roll+90-10) && (roll <= r_external_roll+90+10) && 
				(pitch >= r_external_pitch-25) && (pitch <= r_external_pitch+25) && 
				(yaw >= r_external_yaw-40) && (yaw <= r_external_yaw+40))
			{
				curr_region = RIGHT_DOWNSIDE;

				if (last_region != curr_region)
				{
					m_roll  = roll;
					m_pitch = pitch;
					m_yaw   = yaw;

					last_region = curr_region;
				}
			}

			if ((roll >= r_external_roll-90-10) && (roll <= r_external_roll-90+10) && 
				(pitch >= r_external_pitch-25) && (pitch <= r_external_pitch+25) && 
				(yaw >= r_external_yaw-40) && (yaw <= r_external_yaw+40))
			{
				curr_region = RIGHT_UPSIDE;

				if (last_region != curr_region)
				{
					m_roll  = roll;
					m_pitch = pitch;
					m_yaw   = yaw;

					last_region = curr_region;
				}
			}

			if ((roll >= r_external_roll-180-10) && (roll <= r_external_roll-180+10) && 
				(pitch >= r_external_pitch-25) && (pitch <= r_external_pitch+25) && 
				(yaw >= r_external_yaw-40) && (yaw <= r_external_yaw+40))
			{
				curr_region = RIGHT_INTERNAL;

				if (last_region != curr_region)
				{
					m_roll  = roll;
					m_pitch = pitch;
					m_yaw   = yaw;

					last_region = curr_region;
				}
			}			
		}		
	}



	return 0;
}
#endif
