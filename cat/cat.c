#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUF_SIZE = 1024;
const int STDIN = 0;
const int STDOUT = 1;

ssize_t report_error() {
	printf("Error: %s\n", strerror(errno));
	exit(-1);
}

ssize_t Read(int fd, void *buf, size_t nbyte) {
	ssize_t n;
	while (1) {
		if ((n = read(fd, buf, nbyte)) >= 0) {
			return n;
		} else {
			if (errno == EINTR) {
				continue;
			}
			return report_error();
		}
	}
}

ssize_t Write(int fd, void *buf, size_t nbyte) {
	size_t written = 0;
	while (written < nbyte) {
		ssize_t n = write(fd, buf + written, nbyte - written);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			return report_error();
		}
		written += n;
	}
	return written;
}

int main(int argc, char* argv[]) {
	char buf[BUF_SIZE];
	ssize_t n;
	int fd = STDIN;
	if (argc > 1) {
		fd = open(argv[1], O_RDONLY);
		if (fd < 0) {
			report_error();
		}
	}
	
	while ((n = Read(fd, buf, BUF_SIZE)) > 0) {
		Write(STDOUT, buf, n);
	}
	return 0;
}
