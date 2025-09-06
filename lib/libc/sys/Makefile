#
# Copyright (c) 2024 Jules
# All rights reserved.
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= ${DEFS} -Os

SRCS		= openat.c fstatat.c unlinkat.c mq_open.c
OBJS		= openat.o fstatat.o unlinkat.o mq_open.o

sys.a:		${OBJS}
		@echo "building sys.a"
		@${AR} cr sys.a ${OBJS}

clean:
		rm -f sys.a *.o *~

depend:
		# Add depend logic later if needed

.PHONY: all install clean depend
