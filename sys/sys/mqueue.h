/*
 * Copyright (c) 2024 The DiscoBSD Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _SYS_MQUEUE_H_
#define _SYS_MQUEUE_H_

#include <sys/types.h>

#define MQ_MAX_MESSAGES 8
#define MQ_MAX_MSG_SIZE 64
#define MQ_MAX_QUEUES   2  /* System-wide limit */

typedef int mqd_t;

struct mqueue {
    char        mq_name[16];
    u_int8_t    mq_flags;
    u_int8_t    mq_count;      /* Current message count */
    u_int8_t    mq_maxmsg;     /* Max messages */
    u_int16_t   mq_msgsize;    /* Max message size */
    u_int16_t   mq_head;       /* Queue head index */
    u_int16_t   mq_tail;       /* Queue tail index */
    char        *mq_buffer;    /* Message buffer */
};

#endif /* !_SYS_MQUEUE_H_ */
