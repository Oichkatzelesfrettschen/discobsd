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

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/mqueue.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/malloc.h>
#include <sys/string.h>

static struct mqueue mqueues[MQ_MAX_QUEUES];

mqd_t
mq_open(const char *name, int oflag, ...)
{
    int i;
    struct mqueue *mq = NULL;
    struct proc *p = u.u_procp;

    /* Find existing or allocate new */
    for (i = 0; i < MQ_MAX_QUEUES; i++) {
        if (mqueues[i].mq_name[0] == '\0') {
            if (mq == NULL)
                mq = &mqueues[i];
        } else if (strcmp(mqueues[i].mq_name, name) == 0) {
            return i;  /* Found existing */
        }
    }

    if (!(oflag & O_CREAT) || mq == NULL) {
        u.u_error = ENOENT;
        return (-1);
    }

    /* Initialize new queue with minimal resources */
    strncpy(mq->mq_name, name, sizeof(mq->mq_name));
    mq->mq_maxmsg = MQ_MAX_MESSAGES;
    mq->mq_msgsize = MQ_MAX_MSG_SIZE;
    mq->mq_buffer = malloc(MQ_MAX_MESSAGES * MQ_MAX_MSG_SIZE, M_TEMP, M_WAITOK);

    if (mq->mq_buffer == NULL) {
        u.u_error = ENOMEM;
        return (-1);
    }

    return (mq - mqueues);
}
