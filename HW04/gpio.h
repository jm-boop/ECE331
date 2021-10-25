#ifndef GPIO_H
#define GPIO_H

void exportgpio(char *pin);
void unexportgpio(char *pin);
void setvalue(char *pin, char *onezero);
void pindirection(char *pin, char *inout);

#endif
