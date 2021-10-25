#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "gpio.h"

int main(int argc, char *argv[]) 
{
	struct tm *datetime;
	time_t t;
	char *utc, *tok, *arr[5], *year, *times;
	char *seg1, *seg2, *seg3, *seg4, *seg5, *seg0;
	int i, dstflag;
	char *segbuff;
	char pin[3] = "12";
	char inout[4] = "out";	

	/* allocate memory to hold a 'segment buffer', holding each 10 time
	 * indexes upon iteration.
	 */
	segbuff = (char *)malloc(10 * sizeof(int));


	/* create "gpio12" node */
	exportgpio(pin);
	usleep(1000000);
	/* write 'out' to GPIO#12 setting it as an output */
	pindirection(pin, inout);
	usleep(1000000);
	printf("Begin wwv signal.\n");

	/* generate current time and date in UTC */
	t = time(NULL);
	datetime = gmtime(&t);
	utc = asctime(datetime);
	/* read 'tm_isdst' flag based off current date to check if in DST */
	dstflag = datetime->tm_isdst;

	/* UTC time and dat in format:
	 * 'Thu Mar 5 17:56:40 2020'.
	 *  parse into tokens and store into 'arr[]'.
	 */
	tok = strtok(utc, " ");
	i = 0;
	while (tok != NULL) {
		arr[i++] = tok;
		tok = strtok(NULL, " ");
	}	

	/* 'Segment 1' blinking */
	year = arr[4];
	seg1 = segone(segbuff, year, dstflag);
	blink(seg1);

	/* 'Segment 2' blinking */
	times = arr[3];	
	seg2 = segtwo(segbuff, times);
	blink(seg2);
	
	/* 'Segment 3' blinking */
	seg3 = segthree(segbuff, times);
	blink(seg3);

	/* 'Segment 4' blinking */
	char *month = arr[1];
	char *day = arr[2];
	/* calculate day of year with helper function */
	int doy = calcdoy(year, month, day);
	seg4 = segfour(segbuff, doy);
	blink(seg4);
	
	/* 'Segment 5' blinking */
	seg5 = segfive(segbuff, doy);
	blink(seg5);

	/* 'Segment 0' blinking */
	seg0 = segzero(segbuff);
	blink(seg0);
		 
	/* free allocated memory */
	free(segbuff);
	
	/* remove "gpio12" node */
	printf("End of signal.\n");
	unexportgpio(pin);
	usleep(1000000);
	
return 0;	
}
