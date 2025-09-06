/*
 * System calls for file operations on directories.
 *
 * Copyright (c) 2024 The DiscoBSD Project.
 * All rights reserved.
 *
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/namei.h>
#include <sys/vnode.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>

/*
 * Open a file relative to a directory file descriptor.
 */
int
sys_openat(p)
    struct proc *p;
{
    struct a {
        int     fd;
        char    *path;
        int     flags;
        int     mode;
    } *uap = (struct a *)p->p_uap;
    struct file *fp;
    struct vnode *vp;
    struct nameidata nd;
    int error;
    int fd;

    if (uap->fd == AT_FDCWD) {
        /*
         * Fallback to regular open() if AT_FDCWD is used.
         * This is not a perfect implementation, but it's a start.
         */
        p->p_uap = (int *)((caddr_t)uap + sizeof(int));
        return sys_open(p);
    }

    /* Get the directory vnode */
    error = getvnode(p->p_fd, uap->fd, &fp);
    if (error)
        return (error);
    vp = (struct vnode *)fp->f_data;
    if (vp->v_type != VDIR) {
        vrele(vp);
        return (ENOTDIR);
    }

    /*
     * To handle relative paths correctly, we can temporarily
     * change the current working directory of the process.
     */
    struct vnode *old_cdir = p->p_cdir;
    p->p_cdir = vp;

    /*
     * Now we can call the regular open() syscall logic, which will
     * use the new current directory.
     */
    p->p_uap = (int *)((caddr_t)uap + sizeof(int));
    error = sys_open(p);

    /* Restore the original current directory */
    p->p_cdir = old_cdir;
    vrele(vp);

    return (error);
}

/*
 * Get file status relative to a directory file descriptor.
 */
int
sys_fstatat(p)
    struct proc *p;
{
    struct a {
        int	fd;
        char	*path;
        struct stat *ub;
        int	flag;
    } *uap = (struct a *)p->p_uap;
    struct file *fp;
    struct vnode *vp;
    int error;

    if (uap->fd == AT_FDCWD) {
        p->p_uap = (int *)((caddr_t)uap + sizeof(int));
        return sys_stat(p);
    }

    /* Get the directory vnode */
    error = getvnode(p->p_fd, uap->fd, &fp);
    if (error)
        return (error);
    vp = (struct vnode *)fp->f_data;
    if (vp->v_type != VDIR) {
        vrele(vp);
        return (ENOTDIR);
    }

    struct vnode *old_cdir = p->p_cdir;
    p->p_cdir = vp;

    /*
     * The `stat` syscall expects the path and stat buffer in the
     * uap. We need to adjust the arguments before calling it.
     */
    p->p_uap = (int *)((caddr_t)uap + sizeof(int));
    error = sys_stat(p);

    p->p_cdir = old_cdir;
    vrele(vp);

    return (error);
}

/*
 * Unlink a file relative to a directory file descriptor.
 */
int
sys_unlinkat(p)
    struct proc *p;
{
    struct a {
        int	fd;
        char	*path;
        int	flag;
    } *uap = (struct a *)p->p_uap;
    struct file *fp;
    struct vnode *vp;
    int error;

    if (uap->fd == AT_FDCWD) {
        p->p_uap = (int *)((caddr_t)uap + sizeof(int));
        return sys_unlink(p);
    }

    /* Get the directory vnode */
    error = getvnode(p->p_fd, uap->fd, &fp);
    if (error)
        return (error);
    vp = (struct vnode *)fp->f_data;
    if (vp->v_type != VDIR) {
        vrele(vp);
        return (ENOTDIR);
    }

    struct vnode *old_cdir = p->p_cdir;
    p->p_cdir = vp;

    /*
     * The `unlink` syscall expects the path in the uap.
     */
    p->p_uap = (int *)((caddr_t)uap + sizeof(int));
    error = sys_unlink(p);

    p->p_cdir = old_cdir;
    vrele(vp);

    return (error);
}
