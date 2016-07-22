#include "net_utils.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>


int setNonblock(int fd)
{
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0))) flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

bool ipStrIsValid(const char * const str)
{
	struct sockaddr_in sa;
	return 0 != inet_pton(AF_INET, str, &(sa.sin_addr));
}
