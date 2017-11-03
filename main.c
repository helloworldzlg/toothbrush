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


unsigned char g_rxbuff[512];
unsigned int g_rxcount = 0;

int OpenDev(char *Dev)
{
    int fd = open( Dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == fd)    
    {             
        perror("Can't Open Serial Port");
        return -1;        
    }    
    else    
        return fd;
}

void parse_protocol()
{
	int i, j;
	unsigned char *pdata = &g_rxbuff[0];

	unsigned char frame_type;
	signed short gx, gy, gz;
	int roll, pitch, yaw;
	unsigned short m_roll, m_pitch, m_yaw;
	static int m_is_ready = 0;

	for (i = 0; i < g_rxcount; i++)
	{
		if ((0xA5 == pdata[i]) && (0x5A == pdata[i+1]) && (i < g_rxcount-18))
		{
			frame_type = pdata[i+3];
//			for (j = 0; j < 12; j++)
//			{
//				printf("%x ", pdata[i+j]);
//			}
//			printf("\n");

			if (0xA1 == frame_type)
			{
				m_roll  = ((pdata[i+4] << 8) | pdata[i+5]) & 0xFFFF;
				m_pitch = ((pdata[i+6] << 8) | pdata[i+7]) & 0xFFFF;
				m_yaw   = ((pdata[i+8] << 8) | pdata[i+9]) & 0xFFFF;

				(m_roll > 0x8000) ? (roll = -(m_roll - 0x8000)/10) : (roll = m_roll/10);
				(m_pitch > 0x8000) ? (pitch = -(m_pitch - 0x8000)/10) : (pitch = m_pitch/10);
				(m_yaw > 0x8000) ? (yaw = -(m_yaw - 0x8000)/10) : (yaw = m_yaw/10);
				yaw = yaw - 180;
				
				printf("r = %d, p = %d, y = %d\n", roll, pitch, yaw);
				m_is_ready = 1;
			}

			if (0xA2 == frame_type)
			{
				gx = ((pdata[i+4] << 8) | pdata[i+5]);
				gy = ((pdata[i+6] << 8) | pdata[i+7]);
				gz = ((pdata[i+8] << 8) | pdata[i+9]);

				printf("gx = %d, gy = %d, gz = %d\n", gx, gy, gz);
				m_is_ready = 2;
			}

			if (2 == m_is_ready)
			{
				calc_region(gx, gy, gz, roll, pitch, yaw);

				m_is_ready = 0;
			}
		}
	}
}

int main(int argc, char **argv)
{
    int fd;
    int nread;
    char buff[256];
    char *dev  = "/dev/ttyUSB0";
    struct termios options;
    
    fd = OpenDev(dev);

	tcgetattr(fd, &options);

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);
	
	//循环读取数据
	while (1)
	{   
		while((nread = read(fd, buff, sizeof(buff))) > 0)
		{ 
			memcpy(&g_rxbuff[g_rxcount], buff, nread);
			g_rxcount += nread;

			if (g_rxcount >= 64)
			{
				parse_protocol();

				memset(g_rxbuff, 0, sizeof(g_rxbuff));
				g_rxcount = 0;				
			}
		}
	}
#if 0
    close(fd); 
    exit (0);
#endif
}
