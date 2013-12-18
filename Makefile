# See LICENSE file for copyright and license details.

include config.mk

SRC = al.c \
      dwmstatus.c \

	
#OBJ = ${SRC:.c=.o}
objs = al.o dwmstatus.o


#all: options ${NAME}  
all: options dwmstatus 

options:
	@echo ${NAME} build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

al.o:al.c common.h
	$(CC) -c al.c -o al.o

dwmstatus.o:dwmstatus.c common.h
	@${CC} -c ${CFLAGS} dwmstatus.c -o dwmstatus.o


dwmstatus: ${objs}
	@echo CC -o $@
	${CC} -o $@ *.o ${LDFLAGS}


clean:
	@echo cleaning
	@rm -f ${NAME} ${objs}


.PHONY: all options clean 
