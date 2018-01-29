/*
 *  1. 调用StartSampleImu使能欧拉角解算
 *  2. 调用StopSmapleImu结束欧拉角解算
 *  3. 结束欧拉角解算后，调用GetBrushScore获取本次刷牙得分
 */

typedef enum
{
	BRUSH_LEFT  = 0,
	BRUSH_RIGHT = 1,
}TOOTHBRUSH_DIR_E;

typedef enum
{
	INIT_UPRIGHT    = 0,
	LEFT_OUTSIDE    = 1,
	LEFT_INSIDE     = 2,
	LEFT_UPSIDE     = 3,
	LEFT_DOWNSIDE   = 4,
	RIGHT_OUTSIDE   = 5,
	RIGHT_INSIDE    = 6,
	RIGHT_UPSIDE    = 7,
	RIGHT_DOWNSIDE  = 8,
	
	TOOTH_REGION_NUM = 9,
}TOOTH_BRUSH_REGION_E;

void StartSampleImu(void);
void StopSmapleImu(void);
void CalcEulerAngle(unsigned char *bmi160_data_buf, unsigned int buf_len);
int GetBrushScore(void);
