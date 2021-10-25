TARGET=forme
OBJS=fileone.o filetwo.o filethree.o
CFLAGS=-g -Wall -O2
LIBS=-lm

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LIBS}

clean:
	rm -f ${TARGET} ${OBJS} core*
