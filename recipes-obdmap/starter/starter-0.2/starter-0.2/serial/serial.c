#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ioctl.h>


static int rate_to_constant(int baudrate) {
#define B(x) case x: return B##x
        switch(baudrate) {
        B(50);     B(75);     B(110);    B(134);    B(150);
        B(200);    B(300);    B(600);    B(1200);   B(1800);
        B(2400);   B(4800);   B(9600);   B(19200);  B(38400);
        B(57600);  B(115200); B(230400); B(460800); B(500000);
        B(576000); B(921600); B(1000000);B(1152000);B(1500000);
    default: return 0;
    }
#undef B
}


static int usec_to_msec(int ms){
 return ms * 1000;
}

struct termios options;
struct timeval timeout;


int fd;  /* File descriptor for the port */

int open_port(int baudRate)
{

  fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY);
//    fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyS1 - ");
  }
  else
   fcntl(fd, F_SETFL, 0);
//   fcntl(fd, F_SETFL, O_ASYNC|O_NONBLOCK);
/*
 * Get the current options for the port...
 */

tcgetattr(fd, &options);

/*
 * Set the baud rates to 19200...
 */

int speed = rate_to_constant(baudRate);

cfsetispeed(&options, speed ?: B115200);
cfsetospeed(&options, speed ?: B115200);

/*
 * Enable the receiver and set local mode...
 */

/* 8 bits, no parity, no stop bits */
// options.c_cflag &= ~PARENB;
// options.c_cflag &= ~CSTOPB;
// options.c_cflag &= ~CSIZE;
// options.c_cflag |= CS8;
 /* no hardware flow control */
// options.c_cflag &= ~CRTSCTS;
 /* enable receiver, ignore status lines */
// options.c_cflag |= CREAD | CLOCAL;
 /* disable input/output flow control, disable restart chars */
// options.c_iflag &= ~(IXON | IXOFF | IXANY);
 /* disable canonical input, disable echo,
 disable visually erase chars,
 disable terminal-generated signals */
// options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 /* disable output processing */
// options.c_oflag &= ~OPOST;
//options.c_cflag |= (speed | CS8 | CREAD | CLOCAL); //Device not to become master
//options.c_cflag &= ~CSTOPB; // 1 stop bit
//options.c_cflag &= ~PARENB; // No parity bit
//options.c_cflag &= ~CRTSCTS; // No hardware control
//options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //Set echo off
//options.c_lflag &= ~(IXON | IXOFF | IXANY); // No software control

//options.c_cc[VMIN]   =  51;                  // read doesn't block
//options.c_cc[VTIME]  =  0;                  // 0.5 seconds read timeout
//options.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

/* Make raw */
cfmakeraw(&options);

tcflush(fd,TCIFLUSH); // Flush input buffer
tcflush(fd,TCOFLUSH); // Flush output buffer

tcsetattr(fd,TCSANOW,&options);


  return (fd);
}


int write_bytes(char data[],int len){
 if(fd >= 0)
 {
   int n = write(fd,data,len);
   if(n < 0)
   {
    return n;
   }else{
    return 0;
   }
 } 
}


int bytes_available(){
 int bytes_avail = 0;
 ioctl(fd, FIONREAD, &bytes_avail);
return bytes_avail;
}

//int read_bytes(int len, int milis, char *buffer)
long read_bytes(char buff[],long len,int milis)
{


  fd_set set;
  struct timeval timeout;

  FD_ZERO(&set); /* clear the set */
  FD_SET(fd, &set); /* add our file descriptor to the set */

  timeout.tv_sec = 1;


 int  rv = select(fd + 1, &set, NULL, NULL, &timeout);
  if(rv == -1)
 	return -1;
  else if(rv == 0)
	return 0;
  else
  rv =  read(fd , buff, len ); /* there was data to read */

return rv;

// return read( fd, buff, len ); /* there was data to read */
 
}

long read_bytes_sec(char buff[],long len,int seconds)
{


  fd_set set;
  struct timeval timeout = {seconds, 0};

  FD_ZERO(&set); /* clear the set */
  FD_SET(fd, &set); /* add our file descriptor to the set */



 int  rv = select(fd + 1, &set, NULL, NULL, &timeout);
  if(rv == -1)
        return -1;
  else if(rv == 0)
        return 0;
  else
  rv =  read(fd , buff, len ); /* there was data to read */
 
return rv; 
   
// return read( fd, buff, len ); /* there was data to read */
    
}

void clear_buffer()
{
 tcflush(fd,TCIOFLUSH);
}
