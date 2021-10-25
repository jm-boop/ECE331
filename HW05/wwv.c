#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"

void blink(char *segment) 
{
	/* initialize input strings to blink LED on GPIO#12 */
	char pin[3] = "12";
	char one[2] = "1";
	char zero[2] = "0";
	int i = 0;
		
	for (i = 0; i < 10; i++) {
		/* zero bit(0) represented by LED on for 170ms, off for rest 
		 * of second 
	         */
		if (segment[i] == 0) {
			setvalue(pin, one);
			usleep(170000);
			setvalue(pin, zero);
			usleep(830000);
		/* one bit(1) represented by LED on for 470ms, off for rest 
		 * of second
		 */
		} else if (segment[i] == 1) {
			setvalue(pin, one);
			usleep(470000);
			setvalue(pin, zero);
			usleep(530000);
		/* position identifier(2) represented by LED on for 770ms, 
		 * off for rest of second
	         */
		}else if ( segment[i] == 2) {
			setvalue(pin, one);
			usleep(770000);
			setvalue(pin, zero);
			usleep(230000);
		/* always zero(3) represented by LED off for duration of 
	         * one second
	         */
		} else if ( segment[i] == 3) {
			setvalue(pin, zero);
			usleep(1000000);
		}
	}
}

int calcdoy(char *year, char *month, char *day)
{
	int yearint, isleap, doy, flag, dayint;
	int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	/* change days in February based on leap year */
	yearint = atoi(year); 
	isleap = isleapyear(yearint);

	if (isleap) {
		days[1] = 29;
	}

	dayint = atoi(day);
	/* initialize day of year with number of days in current month */
	doy = dayint;
	flag = 0;
	for (int i = 11; i >= 0; --i) {
		if (flag) {
			doy += days[i];
		}
		if (strcmp(month, months[i]) == 0) {
			flag = 1; 			
		}
	}
return doy;
}

char *segzero(char *segbuff)
{
	int i;
	
	/* fill buffer with 'always zero' (3) for last 10 seconds of wwv
	 * pattern
	 */
	for (i = 0; i < 10; i++) {
		segbuff[i] = 3;
	}
return segbuff;
}

char *segfive(char *segbuff, int doy)
{
	char buff[3];
	int doyhund, i;
	char *bin;

	/* convert day of year to string to be parsed by ones, tens, and 
	 * hundreds place.
	 */
	snprintf(buff, 10, "%d", doy);
	if (strlen(buff) == 2) {
		doyhund = 0;
	}
	else {
		doyhund = buff[0];
	}
	
	/* pass day of year hundred's place to helper function to return 
	 * BCD.
	 */
	bin = int2bin(doyhund);
	for (i = 0; i < 4; i++) {
		segbuff[i] = bin[i];
	}
	free(bin);

	segbuff[4] = 0;
	for (i = 0; i < 5; i++) {
		segbuff[i + 5] = 3;
	}
return segbuff;
}

char *segfour(char *segbuff, int doy)
{
	char buff[3];
	int doyones, doytens, i;
	char *bin;

	snprintf(buff, 10, "%d", doy);
	if (strlen(buff) == 2) {
		doyones = buff[1];
		doytens = buff[0];
	}
	else {
		doyones = buff[2];
		doytens = buff[1];
	}
	
	bin = int2bin(doyones);
	for (i = 0; i < 4; i++) {
		segbuff[i] = bin[i];
	}
	free(bin);
	segbuff[4] = 0;	
	
	bin = int2bin(doytens);
	for (i = 0; i < 4; i++) {
		segbuff[i + 5] = bin[i];
	}
	free(bin);
	
	/* pos identifier */
	segbuff[9] = 2; 
return segbuff;
}

char *segthree(char *segbuff, char *times)
{
	int i, hr_ones, ho, hr_tens, ht;
	char *bin;

	/* extract one's place of hours */
	hr_ones = times[1];
	ho = hr_ones - '0';
	
	bin = int2bin(ho);
	for (int i = 0; i < 4; i++) {
		segbuff[i] = bin[i];
	}
	free(bin);
	
	segbuff[4] = 0;

	/* extract tens place of hours */
	hr_tens = times[0];
	ht = hr_tens - '0';

	bin = int2bin(ht);
	for (i = 0; i < 4; i++) {
		segbuff[i + 5] = bin[i];			
	}
	free(bin);
	
	/* pos identifier */
	segbuff[9] = 2;

return segbuff;
}

char *segtwo(char *segbuff, char *times)
{
	int i, min_ones, mo, men_tens, mt;
	char *bin;

	/* extract one's place of minutes */
	min_ones = times[4];
	mo = min_ones - '0';
	bin = int2bin(mo);
	for (i = 0; i < 4; i++) {
		segbuff[i] = bin[i];
	}
	free(bin);
	
	segbuff[4] = 0;

	/* extract ten's place */
	men_tens = times[3];
	mt = men_tens - '0';

	bin = int2bin(mt);
	for (i = 0; i < 4; i++) {
		segbuff[i + 5] = bin[i];			
	}
	free(bin);
	
	/* pos identifier */
	segbuff[9] = 2;

return segbuff;
}

char *segone(char *segbuff, char *year, int dstflag)
{
	int i, yearint, y;
	char *bin;
	
	segbuff[0] = 3;
	segbuff[1] = 0;
	segbuff[2] = dstflag;

	/* check if leapyear using helper function */
	yearint = atoi(year);
	segbuff[3] = isleapyear(yearint);

	y = year[3] - '0';	
	
	bin = int2bin(y);
	for (i = 0; i < 4; i++) {
		segbuff[i + 4] = bin[i];
	}
	segbuff[8] = 0;
	segbuff[9] = 2;
	free(bin);
return segbuff;
}

int isleapyear(int year)
{
	if (~(year % 4)) {
		if (~(year % 100)) {
			if (~(year % 400)) {
				return 1;	
			} else {
				return 0;
			}
		} else {
			return 1;
		} 
	} else {
		return 0;
	}
}

char *int2bin(int a)
{
	char *bin;
	/* allocate four bytes for BCD value to be returned */
	bin = (char *)malloc(4 * sizeof(char));

	(a & 0x1) ? (bin[0] = 1) : (bin[0] = 0);
	(a & 0x2) ? (bin[1] = 1) : (bin[1] = 0);
	(a & 0x4) ? (bin[2] = 1) : (bin[2] = 0);
	(a & 0x8) ? (bin[3] = 1) : (bin[3] = 0);
return bin;
}
