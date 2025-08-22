TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

CFLAGS		= -O -Wall

LEX?=	flex

# libbsd-dev package on Linux in overlay mode.
CFLAGS+=	${_LIBBSD_CFLAGS}
LIBS+=		${_LIBBSD_LIBS}

OBJS            = config.o main.o lang.o mkioconf.o mkmakefile.o mkswapconf.o
PROG		= kconfig
MAN		= kconfig.0
MANSRC		= kconfig.8

all:		${PROG} ${MAN}

install:	all
		${INSTALL} ${PROG} ${TOOLBINDIR}/${PROG}

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
	rm -f *~ *.o ${PROG} ${MAN} ${PROG}.dSYM y.tab.[ch] lang.c config.c

$(PROG):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

config.o: config.c y.tab.h config.h
main.o: main.c y.tab.h config.h
mkioconf.o: mkioconf.c y.tab.h config.h
mkmakefile.o: mkmakefile.c y.tab.h config.h
mkswapconf.o: mkswapconf.c config.h

# Generate parser and lexer sources from the corresponding grammar.
config.c y.tab.h: config.y
	${YACC} -d config.y
	mv -f y.tab.c config.c

lang.c: lang.l y.tab.h
	${LEX} -o lang.c lang.l

lang.o: lang.c y.tab.h

.PHONY: all install clean
