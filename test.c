#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toothbrush.h"

#define BAUDRATE        B115200
#define UART_DEVICE     "/dev/ttyUSB0"

#define FALSE  -1
#define TRUE   0
////////////////////////////////////////////////////////////////////////////////
/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
          		   B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, 
		  		  115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed){
  int   i; 
  int   status; 
  struct termios   Opt;
  tcgetattr(fd, &Opt); 
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) { 
    if  (speed == name_arr[i]) {     
      tcflush(fd, TCIOFLUSH);     
      cfsetispeed(&Opt, speed_arr[i]);  
      cfsetospeed(&Opt, speed_arr[i]);   
      status = tcsetattr(fd, TCSANOW, &Opt);  
      if  (status != 0) {        
        perror("tcsetattr fd1");  
        return;     
      }    
      tcflush(fd,TCIOFLUSH);   
    }  
  }
}
////////////////////////////////////////////////////////////////////////////////
/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
int set_Parity(int fd,int databits,int stopbits,int parity)
{ 
	struct termios options; 
	if  ( tcgetattr( fd,&options)  !=  0) { 
		perror("SetupSerial 1");     
		return(FALSE);  
	}
	options.c_cflag &= ~CSIZE; 
	switch (databits) /*设置数据位数*/
	{   
	case 7:		
		options.c_cflag |= CS7; 
		break;
	case 8:     
		options.c_cflag |= CS8;
		break;   
	default:    
		fprintf(stderr,"Unsupported data size\n"); return (FALSE);  
	}
	switch (parity) 
	{   
		case 'n':
		case 'N':    
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
			break;  
		case 'o':   
		case 'O':     
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
			options.c_iflag |= INPCK;             /* Disnable parity checking */ 
			break;  
		case 'e':  
		case 'E':   
			options.c_cflag |= PARENB;     /* Enable parity */    
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S': 
		case 's':  /*as no parity*/   
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;break;  
		default:   
			fprintf(stderr,"Unsupported parity\n");    
			return (FALSE);  
		}  
	/* 设置停止位*/  
	switch (stopbits)
	{   
		case 1:    
			options.c_cflag &= ~CSTOPB;  
			break;  
		case 2:    
			options.c_cflag |= CSTOPB;  
		   break;
		default:    
			 fprintf(stderr,"Unsupported stop bits\n");  
			 return (FALSE); 
	} 
	/* Set input parity option */ 
	if (parity != 'n')   
		options.c_iflag |= INPCK; 
	tcflush(fd,TCIFLUSH);
	options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/   
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)   
	{ 
		perror("SetupSerial 3");   
		return (FALSE);  
	} 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/
	return (TRUE);  
}
////////////////////////////////////////////////////////////////////////////////
unsigned char g_rxbuff[512];
unsigned int g_rxcount = 0;

void parse_protocol()
{
	int i, j;
	unsigned char *pdata = &g_rxbuff[0];

	unsigned char frame_type;
	signed short gx, gy, gz;
	int roll, pitch, yaw;
	unsigned short m_gx, m_gy, m_gz;
	unsigned short m_roll, m_pitch, m_yaw;
	static int m_is_ready = 0;

	for (i = 0; i < g_rxcount; i++)
	{
		if ((0xA5 == pdata[i]) && (0x5A == pdata[i+1]) && (i < g_rxcount-18))
		{
			frame_type = pdata[i+3];
			#if 0
			for (j = 0; j < 12; j++)
			{
				printf("%x ", pdata[i+j]);
			}
			printf("\n");
			#endif

			if (0xA1 == frame_type)
			{
			    m_yaw   = ((pdata[i+4] << 8) | pdata[i+5]) & 0xFFFF;				
				m_pitch = ((pdata[i+6] << 8) | pdata[i+7]) & 0xFFFF;
				m_roll  = ((pdata[i+8] << 8) | pdata[i+9]) & 0xFFFF;
				

				(m_roll > 0x8000) ? (roll = -(m_roll - 0x8000)/10) : (roll = m_roll/10);
				(m_pitch > 0x8000) ? (pitch = -(m_pitch - 0x8000)/10) : (pitch = m_pitch/10);
				(m_yaw > 0x8000) ? (yaw = -(m_yaw - 0x8000)/10) : (yaw = m_yaw/10);
				pitch += 90;
				roll  += 180;
								
				m_is_ready = 1;
			}

			if (0xA2 == frame_type)
			{
				m_gx = ((pdata[i+4] << 8) | pdata[i+5]);
				m_gy = ((pdata[i+6] << 8) | pdata[i+7]);
				m_gz = ((pdata[i+8] << 8) | pdata[i+9]);

				(m_gx > 0x8000) ? (gx = -(m_gx - 0x8000)) : (gx = m_gx);
				(m_gy > 0x8000) ? (gy = -(m_gy - 0x8000)) : (gy = m_gy);
				(m_gz > 0x8000) ? (gz = -(m_gz - 0x8000)) : (gz = m_gz);
				
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

int main(int argc, char *argv[])
{
	int nread;
    int    fd, c=0, res;

    unsigned char  buf[256];

    printf("Start...\n");
    fd = open(UART_DEVICE, O_RDWR);

    if (fd < 0) {
        perror(UART_DEVICE);
        exit(1);
    }

    printf("Open...\n");
    set_speed(fd,115200);
	if (set_Parity(fd,8,1,'N') == FALSE)  {
		printf("Set Parity Error\n");
		exit (0);
	}

    printf("Reading...\n");
    while(1) 
    {
		while((nread = read(fd, buf, sizeof(buf))) > 0)
		{ 
			memcpy(&g_rxbuff[g_rxcount], buf, nread);
			g_rxcount += nread;

			if (g_rxcount >= 64)
			{
				parse_protocol();

				memset(g_rxbuff, 0, sizeof(g_rxbuff));
				g_rxcount = 0;				
			}
		}
    }

    printf("Close...\n");
    close(fd);

    return 0;
}
