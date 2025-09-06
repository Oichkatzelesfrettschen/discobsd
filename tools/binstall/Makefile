TOPSRC!=cd ../..; pwd
include ../Makefile.inc

PROG=   xinstall
SRCS=   xinstall.c
OBJS=   xinstall.o

CFLAGS+= ${_LIBBSD_CFLAGS}
LDLIBS+= ${_LIBBSD_LIBS}

all: ${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} -o ${PROG} ${OBJS} ${LDLIBS}

install: all
	./xinstall xinstall ${TOOLBINDIR}/binstall

clean:
	rm -f *.o ${PROG}
