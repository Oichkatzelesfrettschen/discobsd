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
#include <sys/thread.h>
#include <sys/errno.h>

static struct thread threads[MAX_THREADS];
static int current_thread = 0;

/*
 * Architecture-specific context setup (placeholders).
 * These would be implemented in sys/arch/foo/foo/thread_machdep.c
 */
void thread_setup_arm(struct thread *t, void (*func)(void *), void *arg) {}
void thread_setup_mips(struct thread *t, void (*func)(void *), void *arg) {}
void thread_switch(int old_tid, int new_tid) {}


int
thread_create(void (*func)(void *), void *arg, size_t stack_size)
{
    int tid;
    struct proc *p = u.u_procp;

    /* Find free thread slot */
    for (tid = 0; tid < MAX_THREADS; tid++) {
        if (!(threads[tid].t_flags & (TF_READY | TF_RUNNING)))
            break;
    }

    if (tid >= MAX_THREADS) {
        u.u_error = EAGAIN;
        return (-1);
    }

    /*
     * Minimal setup - stack must be pre-allocated.
     * In a real implementation, we would allocate a kernel stack.
     */
    threads[tid].t_flags = TF_READY;
    threads[tid].t_tid = tid;

    /* Architecture-specific context setup */
#if defined(STM32) || defined(__arm__)
    thread_setup_arm(&threads[tid], func, arg);
#elif defined(PIC32) || defined(__mips__)
    thread_setup_mips(&threads[tid], func, arg);
#endif

    return (tid);
}

void
thread_yield()
{
    int next = (current_thread + 1) % MAX_THREADS;
    int original_next = next;
    struct proc *p = u.u_procp;

    while (next != current_thread) {
        if (threads[next].t_flags & TF_READY) {
            int old_thread = current_thread;
            threads[old_thread].t_flags &= ~TF_RUNNING;
            threads[old_thread].t_flags |= TF_READY;

            threads[next].t_flags &= ~TF_READY;
            threads[next].t_flags |= TF_RUNNING;

            current_thread = next;
            thread_switch(old_thread, next);
            return;
        }
        next = (next + 1) % MAX_THREADS;
        if (next == original_next) {
            /* No other ready threads to yield to */
            return;
        }
    }
}
