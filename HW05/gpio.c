#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"

void exportgpio(char *pin) 
{
	int file, length;
	char *path;

	/* initialize path to sysfys file as a char pointer.
	 * can initialize as a pointer because not modifying string
	 */
	path = "//sys/class/gpio/export";
	/* open returns an int type file descriptor as 'open for
	 * writing only.
	 */
	file = open(path, O_WRONLY);
	/* error check, descriptor will return -1 if not opened */
	if (file < 0) {
		printf("Error: can not open file\n");
		exit(EXIT_FAILURE);
	}

	length = strlen(pin);
	/* system call 'write()' writes data in bytes to specified file.
	 * parameters are the file descriptor, string that gets written,
	 * and number of bytes to be read from string.
	 * in this case, write the GPIO pin # desired to /export directory
	 * to create GPIO node
	 */
	write(file, pin, length);
	/* calling close actives the written value */
	close(file);
}

void unexportgpio(char *pin)
{
	int file, length;
	char *path;

	path = "//sys/class/gpio/unexport";
	file = open(path, O_WRONLY);

	if (file < 0) {
		printf("Error: can not open file\n");
		exit(EXIT_FAILURE);
	}

	length = strlen(pin);
	/* write GPIO pin # desired to /unexport directory to remove GPIO
	 * node.
	 */
	write(file, pin, length);
	close(file);
}

void setvalue(char *pin, char *onezero)
{
	int file, length;
	/* allocate 30 spaces for path because modify the string based
	 * on GPIO pin input
	 */
	char path[30] = "//sys/class/gpio/gpio";
	/* append GPIO input value to path string, along with appropiate
	 * sysfs file
	 */
	strcat(path, pin);
	strcat(path, "/value");

	file = open(path, O_WRONLY);

	if (file < 0) {
		printf("Error: can not open file\n");
		exit(EXIT_FAILURE);
	}

	length = strlen(onezero);
	write(file, onezero, length);
	close(file);
}

void pindirection(char *pin, char *inout)
{
	int file, length;
	char path[34] = "//sys/class/gpio/gpio";

	strcat(path, pin);
	strcat(path, "/direction");

	file = open(path, O_WRONLY);
	
	if (file <0) {
		printf("Error: can not open file\n");
		exit(EXIT_FAILURE);
	}

	length = strlen(inout);
	write(file, inout, length);
	close(file);
}
