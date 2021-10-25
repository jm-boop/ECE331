#ifndef GPIO_H
#define GPIO_H

/* takes each segment's 10 time indices and blinks LEDs appropiatly for 
 * 10-second segment
 */
void blink(char *segment);

/* takes parsed year, month, and day; returns calculated 'day of year' */
int calcdoy(char *year, char *month, char *day);

/* each 'seg' func returns 10 bit string to be passed to 'blink()' func */
char *segzero(char *segbuff);
char *segfive(char *segbuff, int doy);
char *segfour(char *segbuff, int doy);
char *segthree(char *segbuff, char *times);
char *segtwo(char *segbuff, char *times);
char *segone(char *segbuff, char *year, int dstflag);

/* takes the parsed year field as an integer; returns 1 if leap year, returns * 0 if not
 */
int isleapyear(int year);

/* takes an integer and returns 4 bit BCD value */
char *int2bin(int dec);

/* gpio functions to set up GPIO#12 */
void exportgpio(char *pin);
void unexportgpio(char *pin);
void setvalue(char *pin, char *onezero);
void pindirection(char *pin, char *inout);

#endif
