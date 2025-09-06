#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
fstatat(int fd, const char *path, struct stat *buf, int flag)
{
    return syscall(176, fd, path, buf, flag);
}
