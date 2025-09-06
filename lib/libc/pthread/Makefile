#
# Copyright (c) 2024 The DiscoBSD Project.
# All rights reserved.
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= ${DEFS} -Os

SRCS		= pthread_stubs.c
OBJS		= pthread_stubs.o

pthread.a:	${OBJS}
		@echo "building pthread.a"
		@${AR} cr pthread.a ${OBJS}

clean:
		rm -f pthread.a *.o *~

depend:
		# Add depend logic later if needed

.PHONY: all install clean depend
