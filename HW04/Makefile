TARGET=gpioLED
OBJS=main.o gpio.o
CFLAGS=-g -Wall -O2

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS}

clean:
	rm -f ${TARGET} ${OBJS} core*
gpio.o: gpio.h
