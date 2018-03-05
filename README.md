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

## 可调节参数说明