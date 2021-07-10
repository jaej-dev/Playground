#ifndef _XNET_UTIL_
#define _XNET_UTIL_

#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <string>

// socket utility
class xnet_util_t {
public:
    xnet_util_t();
    ~xnet_util_t();

    static int setaddress(
            const char* protocol,
            const char* addr,
            const char* portstr,
            struct sockaddr_in *sockaddr);
    static int setsockopt(
            int s,
            int level,
            int optname,
            const void* flag,
            socklen_t flaglen
            );
    static ssize_t readn(
            int s,
            char* buf,
            ssize_t len
            );
    static ssize_t read(
            int s,
            char* buf,
            ssize_t len
            );
    static ssize_t recvfrom(
            int s,
            char* buf,
            ssize_t len
            );
    static ssize_t writen(
            int s,
            const char* buf,
            ssize_t len
            );
    static ssize_t sendto(
            int s,
            const char* buf,
            ssize_t len,
            const struct sockaddr * to,
            socklen_t tolen
            );
    static int socket(const char* protocol);
    static int bind(int s, struct sockaddr_in *sockaddr);
    static int listen(int s);
    static int accept(
            int s,
            struct sockaddr* addr,
            socklen_t* addrlen
            );
    static int connect(int s, struct sockaddr_in *sockaddr);
    // if connectnb goes wrong, 's' is closed (close()).
    static int connectnb(int s, struct sockaddr_in *sockaddr, int msec);
    static void close(int s);
    static bool messageTimedWait(int s, int usec);
};

#endif // _XNET_UTIL_
