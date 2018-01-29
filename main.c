/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : main.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include "toothbrush.h"

#define FALSE  -1
#define TRUE   0

#define TO_ANGLE(val)            ((val * 180.0f)/32768)
#define TO_ACCELERATE(val)       ((val * 16.0f/32768))
#define TO_ANGULAR(val)          ((val * 2000.0f/32768))

unsigned char g_rxbuff[512];
unsigned int g_rxcount = 0;

unsigned char get_sum_check(unsigned char* pbuff, int len)
{
    int i;
    unsigned char sum_check = 0;
    for (i = 0; i < len; i++)
        sum_check += pbuff[i];
    return sum_check;
}

void parse_protocol()
{
	int i, j;
	unsigned char *pdata = &g_rxbuff[0];

	unsigned char frame_type;
	static short data_buff[6] = {0};
	static int m_is_ready = 0;
    unsigned char sum_check = 0;

	for (i = 0; i < g_rxcount-1; i++)
	{
#if 1
        if ((0 == m_is_ready) && (0x55 == pdata[i]) && (0x52 == pdata[i+1]))
        {
            if ((g_rxcount-i) >= 11)
            {
                sum_check = get_sum_check(&pdata[i], 10);
                if (sum_check == pdata[i+10])
                {
                    data_buff[0] = (short)((unsigned short)pdata[i+3] << 8)|pdata[i+2];
                    data_buff[1] = (short)((unsigned short)pdata[i+5] << 8)|pdata[i+4];
                    data_buff[2] = (short)((unsigned short)pdata[i+7] << 8)|pdata[i+6];
                    m_is_ready = 1;
                    return;
                }
            }
        }
#endif
#if 1
        if ((1 == m_is_ready) && (0x55 == pdata[i]) && (0x51 == pdata[i+1]))
        {
            if ((g_rxcount-i) >= 11)
            {
                sum_check = get_sum_check(&pdata[i], 10);
                if (sum_check == pdata[i+10])
                {
                    data_buff[3] = (short)((unsigned short)pdata[i+3] << 8)|pdata[i+2];
                    data_buff[4] = (short)((unsigned short)pdata[i+5] << 8)|pdata[i+4];
                    data_buff[5] = (short)((unsigned short)pdata[i+7] << 8)|pdata[i+6];
                    m_is_ready = 2;
                    return;
                }
            }
        }
#endif
#if 0
        if ((1 == m_is_ready) && (0x55 == pdata[i]) && (0x53 == pdata[i+1]))
        {
            if ((g_rxcount-i) >= 11)
            {
                sum_check = get_sum_check(&pdata[i], 10);
                if (sum_check == pdata[i+10])
                {
                    roll  = (short)((unsigned short)pdata[i+3] << 8)|pdata[i+2];
                    pitch = (short)((unsigned short)pdata[i+5] << 8)|pdata[i+4];
                    yaw   = (short)((unsigned short)pdata[i+7] << 8)|pdata[i+6];
                    //printf("r = %f, p = %f, y = %f\n", roll, pitch, yaw);
                    //calc_region(ax, ay, az, roll, pitch, yaw);
                    m_is_ready = 2;
                }
            }
        }
#endif
	}

    if (2 == m_is_ready)
    {
        CalcEulerAngle((unsigned char*)data_buff, 12);
        m_is_ready = 0;
    }
}

int main(int argc, char **argv)
{
    int ret, fd, nread, i;
    struct termios  oldtio, newtio;
    unsigned char buff[128];

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        printf("open /dev/ttyUSB0 error!!!");
        return -1;
    }
    printf("fd = %d\n", fd);
    sleep(2);

    /* 获取当前操作模式参数 */
    tcgetattr(fd, &oldtio);
    memset(&newtio, 0, sizeof(newtio));

    /* 波特率=115200 数据位=8 使能数据接收 */
    newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    StartSampleImu();

	//循环读取数据
	for (i = 0; i < 12000; i++)
	{
        nread = read(fd, buff, sizeof(buff));
        if (nread > 0)
        {
            memcpy(&g_rxbuff[g_rxcount], buff, nread);
            g_rxcount += nread;

            if (g_rxcount >= 22)
            {
                parse_protocol();

                memset(g_rxbuff, 0, sizeof(g_rxbuff));
                g_rxcount = 0;
            }
        }
        else
            //printf("nread = %d\n", nread);
        usleep(10000);
	}

    StopSmapleImu();

    GetBrushScore();

    close(fd);
    exit(0);
}
