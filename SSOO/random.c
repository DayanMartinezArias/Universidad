#include <linux/cdrom.h>

int fd = open( "/dev/urandom", O_RDONLY );

if (fd >= 0) {
    char random_data[64];
    ssize_t bytes_read = read( fd, random_data, sizeof(random_data) );   

    close(fd);
}

int main () {

}