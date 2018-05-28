/*
 * spiSpeed.c:
 *	Code to measure the SPI speed/latency.
 *	Copyright (c) 2014 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************

 *
 * modifcato per project work: uso del convertitore AD MCP3204


 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/spi/spidev.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define	TRUE	(1==1)
#define	FALSE	(!TRUE)

#define	SPI_CHAN		0
#define	NUM_TIMES		100
#define	MAX_SIZE		(1024*1024)

static int myFd ;


void spiSetup (int speed)
{
  if ((myFd = wiringPiSPISetup (SPI_CHAN, speed)) < 0)
  {
    fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE) ;
  }
}


int main (void)
{
  int speed, times, size, valore;
  unsigned int start, end ;
  int spiFail ;
  unsigned char *myData, prova[3], test ;
  double timePerTransaction, perfectTimePerTransaction, dataSpeed ;

  if ((myData = malloc (MAX_SIZE)) == NULL)
  {
    fprintf (stderr, "Unable to allocate buffer: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE) ;
  }

  wiringPiSetup () ;


  spiFail = FALSE ;
  /// fck = 1MHz
  spiSetup (1000000) ;

  while(1){
	  /// first byte: switch on and single eneded channel
	  prova[0] = 0x7;
	  /// read channel 0
	  prova[1] = 0x0;
	  /// don't care
	  prova[2] = 0x0;
		//if (wiringPiSPIDataRW (SPI_CHAN, myData, size) == -1)
	  /// send 3 bytes and collect the answer
	  if (wiringPiSPIDataRW (SPI_CHAN, prova, 3) == -1)
	  {
		printf ("SPI failure: %s\n", strerror (errno)) ;
		spiFail = TRUE ;
		
	  }
	  else{
		//printf("OK\n");
		  valore = (prova[1] & 0xF) << 8;
		  valore |= prova[2];
		  /// print raw read values
		printf("%x\t%x\tvalore: %d\n", (prova[1] & 0xF), prova[2], valore);
	  }    
	  /// ripete ogni ms.
	   usleep(1000);  
  }
  close (myFd) ;
  printf ("+-------+--------+----------+----------+-----------+------------+\n") ;
  printf ("\n") ;
  
  return 0 ;
}
