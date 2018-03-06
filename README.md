# toothbrush
智能牙刷姿态检测算法

## API说明
* `void StartSampleImu(void)`
使能IMU数据解算

* `void StopSmapleImu(void)`
停止IMU数据解算

* `void AnalysisBrushPose(unsigned char *bmi160_data_buf, unsigned int buf_len)`
解算牙刷的姿态

* `int GetBrushScore(void)`
获取本次刷牙过程得分

* `int GetWarningAcceX()`
获取x轴方向的加速度过大告警状态

* `int GetWarningAcceY()`
获取y轴方向的加速度过大告警状态

## IMU传感器配置参数

* `#define ACCELERATE_MAX             (16)                //加速度计量程,单位g,即16g,需要根据实际配置修改`

## 可调节参数说明
* `#define ACCELERATE_RANGE           (ACCELERATE_1G/4)   //测定的加速度计容忍的误差限制`
* `#define WARNING_ACCE_X                (2000) //x方向的加速度告警阈值`
* `#define WARNGIN_ACCE_Y                (5000) //y方向的加速度告警阈值`

/*
 *  1. 调用StartSampleImu使能牙刷姿态解算
 *  2. 调用StopSmapleImu结束牙刷姿态解算
 *  3. 每10m调用AnalysisBrushPose解算姿态并分析刷牙区域
 *  4. 结束欧拉角解算后，调用GetBrushScore获取本次刷牙得分
 *  5. 实时获取是否刷牙频率过快告警, 1为告警, 0为无告警
 */
