#include <unistd.h>
#include <fcntl.h>

int
unlinkat(int fd, const char *path, int flag)
{
    return syscall(177, fd, path, flag);
}
