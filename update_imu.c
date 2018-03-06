/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : toothbrush.h
 * Author        : helloworldzlg
 * Date          : 2017-10-28
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2017-10-28
 *   Author      : helloworldzlg
 *   Modification: Created file

*************************************************************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "update_imu.h"

#define START_SMAPLE               (1)
#define STOP_SMAPLE                (0)

unsigned int g_sample_flag = STOP_SMAPLE;
float g_brush_score = 0.0f;
unsigned int g_region_time_count[TOOTH_REGION_NUM];
unsigned int g_warning_acce_x = 0;
unsigned int g_warning_acce_y = 0;

void judge_region(short gx, short gy, short gz, short ax, short ay, short az);

/*******************************  外部接口  *******************************/
/*******************************  外部接口  *******************************/
/*******************************  外部接口  *******************************/

/*
 * 使能IMU数据解算姿态
 */
void StartSampleImu(void)
{
	memset(g_region_time_count, 0, sizeof(g_region_time_count));
	g_sample_flag = START_SMAPLE;
	g_brush_score = INIT_SCORE;
}

/*
 * 停止IMU数据解算姿态
 */
void StopSmapleImu(void)
{
	g_sample_flag = STOP_SMAPLE;
}

/*
 * 获取BMI160传输的加速度和角速度
 */
void AnalysisBrushPose(unsigned char *bmi160_data_buf, unsigned int buf_len)
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

		judge_region(gx, gy, gz, ax, ay, az);
	}
	return;
}

/*
 * 获取本次刷牙过程得分
 */
int GetBrushScore(void)
{
    int i;
    float standard_line[8] = {
       LEFT_OUTSIDE_SCORE, LEFT_INSIDE_SCORE, LEFT_UPSIDE_SCORE, LEFT_DOWNSIDE_SCORE,
       RIGHT_OUTSIDE_SCORE, RIGHT_INSIDE_SCORE, RIGHT_UPSIDE_SCORE, RIGHT_DOWNSIDE_SCORE,
    };

    float region_score = 0;

    for (i = 0; i < TOOTH_REGION_NUM-1; i++)
    {
        if (g_region_time_count[i+1] >= (int)standard_line[i])
        {
            region_score = 5.0;
        }
        else
        {
            region_score = (g_region_time_count[i+1] * 5.0f)/standard_line[i];
        }
        g_brush_score += region_score;
        //printf("count: %d, standard_line: %f, region_score: %f\n", g_region_time_count[i+1], standard_line[i], region_score);
    }

    //printf("score = %f\n", g_brush_score);
	return (int)g_brush_score;
}

/*
 * 获取x轴方向的加速度过大告警状态
 */
int GetWarningAcceX()
{
    return g_warning_acce_x;
}

/*
 * 获取y轴方向的加速度过大告警状态
 */
int GetWarningAcceY()
{
    return g_warning_acce_y;
}

/*******************************  内部函数  *******************************/
/*******************************  内部函数  *******************************/
/*******************************  内部函数  *******************************/

/*
 * 实时更新x轴和y轴的加速度过大(频率快)告警
 */
void update_acce_warning(short ax, short ay)
{
    static int x_acce_count = 0;
    static int y_acce_count = 0;

    if ((ax > WARNING_ACCE_X) || (ax < -WARNING_ACCE_X))
    {
        x_acce_count += 2;
    }

    if ((ay > WARNGIN_ACCE_Y) || (ay < -WARNGIN_ACCE_Y))
    {
        y_acce_count += 2;
    }

    (x_acce_count > 0) ? (x_acce_count -= 1) : (x_acce_count = x_acce_count);
    (y_acce_count > 0) ? (y_acce_count -= 1) : (y_acce_count = y_acce_count);

    if (x_acce_count > 0)
    {
        g_warning_acce_x = 1;
    }

    if (y_acce_count > 0)
    {
        g_warning_acce_y = 1;
    }
    return;
}

/*
 * 根据实时的加速度和角速度判断当前的牙刷姿态，并分析当前刷牙的区域
 */
void judge_region(short gx, short gy, short gz, short ax, short ay, short az)
{
    int ret;
    TOOTH_BRUSH_REGION_E brush_region;
    static int in_flag = 0;
    static int h_orien = 0;

    static TOOTH_BRUSH_REGION_E curr_region, last_region = TOOTH_REGION_NUM;

    update_acce_warning(ax, ay);

    if ((ax > ACCELERATE_1G-ACCELERATE_RANGE) && (ax < ACCELERATE_1G+ACCELERATE_RANGE))
    {
        curr_region = INIT_UPRIGHT;
        //h_orien = get_h_orientation(ay);
        (ay > 0) ? (h_orien = BRUSH_RIGHT)  : (h_orien = BRUSH_LEFT);
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

    g_region_time_count[curr_region]++;
    return;
}