#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "xnet_util.h"

using namespace std;

xnet_util_t::xnet_util_t()
{

}

xnet_util_t::~xnet_util_t()
{

}

int xnet_util_t::setaddress(
        const char* protocol,
        const char* host,
        const char* portstr,
        struct sockaddr_in *sockaddr
        )
{
    struct servent* sp;
    struct hostent* hp;
    int port;
    char* endptr;
    int ret;

    memset(sockaddr, 0, sizeof(*sockaddr));
    sockaddr->sin_family = AF_INET;
    if (host != NULL) {
        ret = inet_aton(host, &(sockaddr->sin_addr));
        if (ret == 0) {
            hp = gethostbyname(host);
            if (hp == NULL) {
                errno = EINVAL;
                return -1;
            }
            sockaddr->sin_addr =
                *reinterpret_cast<struct in_addr *>(hp->h_addr);
        }
    } else {
        sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    }

    long tmp_port = strtol(portstr, &endptr, 0);
    if (*endptr == '\0') {
        if (tmp_port <= 65535L) {
            port = tmp_port;
            sockaddr->sin_port = htons(port);
        } else {
            errno = EINVAL;
            return -1;
        }
    } else {
        sp = getservbyname(portstr, protocol);
        if (sp == NULL) {
            errno = EINVAL;
            return -1;
        }
        sockaddr->sin_port = sp->s_port;
    }
    return 0;
}

int xnet_util_t::setsockopt(
        int s,
        int level,
        int optname,
        const void* flag,
        socklen_t flaglen
        )
{
    return ::setsockopt(s, level, optname, flag, flaglen);
}

ssize_t xnet_util_t::readn(int s, char* buf, ssize_t len)
{
    ssize_t left;
    ssize_t rc;

    left = len;
    while (left > 0) {
        rc = recv(s, buf, left, 0);
        if (rc < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (rc == 0) // EOF
            return len - left;
        buf += rc;
        left -= rc;
    }
    return len;
}

ssize_t xnet_util_t::read(int s, char* buf, ssize_t len)
{
    return recv(s, buf, len, 0);
}

ssize_t xnet_util_t::recvfrom(int s, char* buf, ssize_t len)
{
    return ::recvfrom(s, buf, len, 0, NULL, 0);
}

ssize_t xnet_util_t::writen(int s, const char* buf, ssize_t len)
{
    ssize_t left;
    ssize_t rc;

    left = len;
    while (left > 0) {
        rc = send(s, buf, left, MSG_NOSIGNAL);
        if (rc < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }

        buf += rc;
        left -= rc;
    }
    return len;
}

ssize_t xnet_util_t::sendto(
            int s,
            const char* buf,
            ssize_t len,
            const struct sockaddr * to,
            socklen_t tolen
            )
{
    return ::sendto(s, buf, len, 0, to, tolen);
}

int xnet_util_t::socket(const char* protocol)
{
    int type;
    int s;

    if (!strcmp(protocol, "tcp"))
    {
        type = SOCK_STREAM;
    }
    else if (!strcmp(protocol, "udp"))
    {
        type = SOCK_DGRAM;
    }
    else
    {
        return -1;
    }

    s = ::socket(AF_INET, type, 0);
    return s;
}

int xnet_util_t::bind(int s, struct sockaddr_in *sockaddr)
{
    return ::bind(
            s,
            reinterpret_cast<struct sockaddr*>(sockaddr),
            sizeof(*sockaddr)
            );
}

int xnet_util_t::listen(int s)
{
    return ::listen(s, 5);
}

int xnet_util_t::accept(
        int s,
        struct sockaddr* addr,
        socklen_t* addrlen
        )
{
    int client;
    *addrlen = sizeof(*addr);
    client = ::accept(s, addr, addrlen);
    return client;
}

int xnet_util_t::connect(int s, struct sockaddr_in *sockaddr)
{
    return ::connect(
            s,
            reinterpret_cast<struct sockaddr*>(sockaddr),
            sizeof(*sockaddr)
            );
}

int xnet_util_t::connectnb(
        int s,
        struct sockaddr_in *sockaddr,
        int msec
        )
{
    int flags, n, err;
    socklen_t len;
    fd_set rset, wset;
    struct timeval tval;

    flags = fcntl(s, F_GETFL, 0);
    if (flags == -1) {
        close(s);
        return -1;
    }
    fcntl(s, F_SETFL, flags | O_NONBLOCK);

    err = 0;
    n = ::connect(
            s,
            reinterpret_cast<struct sockaddr*>(sockaddr),
            sizeof(*sockaddr)
            );
    if (n < 0)
        if (errno != EINPROGRESS) {
            close(s);
            return -1;
        }

    if (n == 0)
        goto done;

    FD_ZERO(&rset);
    FD_SET(s, &rset);
    wset = rset;
    tval.tv_sec = msec / 1000;
    if (tval.tv_sec > 0)
    {
        tval.tv_usec = (msec % 1000) * 1000;
    }
    else
    {
        tval.tv_usec = msec * 1000;
    }

    n = select(s + 1, &rset, &wset, NULL, msec ? &tval : NULL);
    if (n == 0) {
        close(s); // stop 3-way handshaking, do not remove this.
        errno = ETIMEDOUT;
        return -1;
    }

    if (FD_ISSET(s, &rset) || FD_ISSET(s, &wset)) {
        len = sizeof(err);
        if (getsockopt(s, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
            close(s);
            return -1;
        }
    } else {
        close(s);
        return -1; // select() error: socket fd not set
    }

done:
    fcntl(s, F_SETFL, flags);

    if (err) {
        close(s);
        errno = err;
        return -1;
    }
    return 0;
}

void xnet_util_t::close(int s)
{
    ::close(s);
}

bool xnet_util_t::messageTimedWait(int s, int usec)
{
    fd_set reads;
    struct timeval tv = {0, 0};
    int rc;

    FD_ZERO(&reads);
    FD_SET(s, &reads);

    tv.tv_sec = usec / 1000000L;
    if (tv.tv_sec > 0)
    {
        tv.tv_usec = usec % (tv.tv_sec * 1000000L);
    }
    else
    {
        tv.tv_usec = usec;
    }

    errno = 0;
    rc = select(s + 1, &reads, NULL, NULL, &tv);
    if (rc < 0) {
        return false;
    } else if (rc == 0) {
        errno = ETIMEDOUT;
        return false;
    }
    return true;
}
