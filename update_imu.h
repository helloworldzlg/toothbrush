/*
 *  1. 调用StartSampleImu使能牙刷姿态解算
 *  2. 调用StopSmapleImu结束牙刷姿态解算
 *  3. 每10m调用AnalysisBrushPose解算姿态并分析刷牙区域
 *  4. 结束欧拉角解算后，调用GetBrushScore获取本次刷牙得分
 *  5. 实时获取是否刷牙频率过快告警, 1为告警, 0为无告警
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

#define TEST_DEBUG                    (12.0f)
#define LEFT_OUTSIDE_SCORE            (2000.0f/TEST_DEBUG)
#define RIGHT_OUTSIDE_SCORE           (2000.0f/TEST_DEBUG)
#define LEFT_INSIDE_SCORE             (1000.0f/TEST_DEBUG)
#define RIGHT_INSIDE_SCORE            (1000.0f/TEST_DEBUG)
#define LEFT_DOWNSIDE_SCORE           (1500.0f/TEST_DEBUG)
#define RIGHT_DOWNSIDE_SCORE          (1500.0f/TEST_DEBUG)
#define LEFT_UPSIDE_SCORE             (1000.0f/TEST_DEBUG)
#define RIGHT_UPSIDE_SCORE            (1000.0f/TEST_DEBUG)

/************************************* 需要配置的参数 *************************************/
#define INIT_SCORE                    (60.0f)             //初始时的分值
#define SHORT_MAX                     (32768)
#define ACCELERATE_MAX                (16)                //加速度计量程,单位g,即16g,需要根据实际配置修改
#define ACCELERATE_1G                 (SHORT_MAX/ACCELERATE_MAX)

/************************************* 可调节的参数 *************************************/
#define ACCELERATE_RANGE              (ACCELERATE_1G/4)   //测定的加速度计容忍的误差限制
#define WARNING_ACCE_X                (2000)              //x方向的加速度告警阈值
#define WARNGIN_ACCE_Y                (5000)              //y方向的加速度告警阈值

void StartSampleImu(void);
void StopSmapleImu(void);
void AnalysisBrushPose(unsigned char *bmi160_data_buf, unsigned int buf_len);
int GetBrushScore(void);
int GetWarningAcceX();
int GetWarningAcceY();
