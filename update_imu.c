/*********************************************************************
 * update_imu.c
 ********************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "update_imu.h"

#define START_SMAPLE               (1)
#define STOP_SMAPLE                (0)
#define INIT_SCORE                 (80)
#define SHORT_MAX                  (32768)
#define ACCELERATE_MAX             (16)                //加速度计量程,单位g,即16g,需要根据实际配置修改
#define ACCELERATE_1G              (SHORT_MAX/ACCELERATE_MAX)
#define ACCELERATE_RANGE           (ACCELERATE_1G/4)   //测定的加速度计容忍的误差限制

unsigned int g_sample_flag = STOP_SMAPLE;
unsigned int g_brush_score = 0;
unsigned int g_region_time_count[TOOTH_REGION_NUM];

/* 启动姿态解算 */
void StartSampleImu(void)
{
	memset(g_region_time_count, 0, sizeof(g_region_time_count));
	g_sample_flag = START_SMAPLE;
	g_brush_score = INIT_SCORE;
}

/* 停止姿态解算 */
void StopSmapleImu(void)
{
	g_sample_flag = STOP_SMAPLE;
}

int get_h_orientation(short ay)
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

void judge_region(short gx, short gy, short gz, short ax, short ay, short az)
{
	int ret;
	TOOTH_BRUSH_REGION_E brush_region;
	static int in_flag = 0;
	static int h_orien = 0;

	static TOOTH_BRUSH_REGION_E curr_region, last_region = TOOTH_REGION_NUM;

	if ((ax > ACCELERATE_1G-ACCELERATE_RANGE) && (ax < ACCELERATE_1G+ACCELERATE_RANGE))
	{
		curr_region = INIT_UPRIGHT;
		h_orien = get_h_orientation(ay);
	}

	if ((ay > -ACCELERATE_1G-ACCELERATE_RANGE) && (ay < -ACCELERATE_1G+ACCELERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_OUTSIDE;
		else
			curr_region = RIGHT_INSIDE;
	}

	if ((az > ACCELERATE_1G-ACCELERATE_RANGE) && (az < ACCELERATE_1G+ACCELERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_UPSIDE;
		else
			curr_region = RIGHT_UPSIDE;
	}

	if ((az > -ACCELERATE_1G-ACCELERATE_RANGE) && (az < -ACCELERATE_1G+ACCELERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_DOWNSIDE;
		else
			curr_region = RIGHT_DOWNSIDE;
	}

	if ((ay > ACCELERATE_1G-ACCELERATE_RANGE) && (ay < ACCELERATE_1G+ACCELERATE_RANGE))
	{
		if (h_orien == BRUSH_LEFT)
			curr_region = LEFT_INSIDE;
		else
			curr_region = RIGHT_OUTSIDE;
	}

#if 0 //判断水平方向的旋转,参数待调整,暂时未加入
	if ((curr_region != INIT_UPRIGHT) && (curr_region == last_region))
	{
		ret = h_orien_is_change(yaw);
		(ret == T_TRUE) ? (h_orien = (h_orien+1)%2) : (h_orien=h_orien);
	}
	else
	{
	 	check_region_flag = 0;
	}
	last_region = curr_region;
#endif
	print_region(curr_region);
	g_region_time_count[curr_region]++;
	return;
}

/* 姿态解算 */
void CalcEulerAngle(unsigned char *bmi160_data_buf, unsigned int buf_len)
{
	int i;
	short gx, gy, gz;
	short ax, ay, az;

	if ((NULL == bmi160_data_buf) || (buf_len < 12) || (buf_len%12 != 0) || (g_sample_flag != START_SMAPLE))
		return;

	for (i = 0; i < buf_len/12; i++)
	{
		gx = *(short*)&bmi160_data_buf[i*12+0];
		gy = *(short*)&bmi160_data_buf[i*12+2];
		gz = *(short*)&bmi160_data_buf[i*12+4];

		ax = *(short*)&bmi160_data_buf[i*12+6];
		ay = *(short*)&bmi160_data_buf[i*12+8];
		az = *(short*)&bmi160_data_buf[i*12+10];

        //printf("gx = %4d, gy = %4d, gz = %4d, ax = %4d, ay = %4d, az = %4d\n",\
        gx, gy, gz, ax, ay, az);
		judge_region(gx, gy, gz, ax, ay, az);
	}
	return;
}

/* 获取本次刷牙得分 */
int GetBrushScore(void)
{
    int i;
    int sum;
    int count = 0;

    printf("socre = %d\n", g_brush_score);
    if ((g_region_time_count[LEFT_OUTSIDE] >= LEFT_OUTSIDE_SCORE)
    	|| (g_region_time_count[RIGHT_OUTSIDE] >= RIGHT_OUTSIDE_SCORE))
    {
    	sum = g_region_time_count[LEFT_OUTSIDE] + g_region_time_count[RIGHT_OUTSIDE];
    	g_brush_score += (int)CALC_REGION_SCORE(sum, LEFT_OUTSIDE_SCORE);
    }
    else
    {
    	sum = g_region_time_count[LEFT_OUTSIDE] + g_region_time_count[RIGHT_OUTSIDE];
        g_brush_score -= (int)CALC_REGION_SCORE(sum, LEFT_OUTSIDE_SCORE);
    }

    printf("socre = %d\n", g_brush_score);
    if ((g_region_time_count[LEFT_DOWNSIDE] >= LEFT_DOWNSIDE_SCORE) ||
    	(g_region_time_count[RIGHT_DOWNSIDE] >= RIGHT_DOWNSIDE_SCORE))
    {
    	sum = g_region_time_count[LEFT_DOWNSIDE] + g_region_time_count[RIGHT_DOWNSIDE];
    	g_brush_score += (int)CALC_REGION_SCORE(sum, LEFT_DOWNSIDE_SCORE);
    }
    else
    {
    	sum = g_region_time_count[LEFT_DOWNSIDE] + g_region_time_count[RIGHT_DOWNSIDE];
        g_brush_score -=  (int)CALC_REGION_SCORE(sum, LEFT_DOWNSIDE_SCORE);
    }

    printf("socre = %d\n", g_brush_score);
    if ((g_region_time_count[LEFT_INSIDE] >= LEFT_INSIDE_SCORE) ||
    	(g_region_time_count[RIGHT_INSIDE] >= RIGHT_INSIDE_SCORE))
    {
    	sum = g_region_time_count[LEFT_INSIDE] + g_region_time_count[RIGHT_INSIDE];
        g_brush_score += (int)CALC_REGION_SCORE(sum, LEFT_INSIDE_SCORE);
    }
    else
    {
    	sum = g_region_time_count[LEFT_INSIDE] + g_region_time_count[RIGHT_INSIDE];
        g_brush_score -= (int)CALC_REGION_SCORE(sum, LEFT_INSIDE_SCORE);
    }

    printf("socre = %d\n", g_brush_score);
    if ((g_region_time_count[LEFT_UPSIDE] >= LEFT_UPSIDE_SCORE) ||
    	(g_region_time_count[RIGHT_UPSIDE] >= RIGHT_UPSIDE_SCORE))
    {
    	sum = g_region_time_count[LEFT_UPSIDE] + g_region_time_count[RIGHT_UPSIDE];
    	g_brush_score += (int)CALC_REGION_SCORE(sum, LEFT_UPSIDE_SCORE);
    }
    else
    {
    	sum = g_region_time_count[LEFT_UPSIDE] + g_region_time_count[RIGHT_UPSIDE];
        g_brush_score -= (int)CALC_REGION_SCORE(sum, LEFT_UPSIDE_SCORE);
    }

	(g_brush_score > 100) ? (g_brush_score = 100) : (g_brush_score = g_brush_score);

	//8个区域基本未覆盖,说明牙刷未运动
	for (i = 0; i < 9; i++)
	{
	    printf("%d: %d\n", i, g_region_time_count[i]);
	    if (g_region_time_count[i] == 0)
	        count++;
	}
    (count > 4) ? (g_brush_score = (8-count)*10) : (g_brush_score = g_brush_score);

    printf("socre = %d\n", g_brush_score);
	return g_brush_score;
}
