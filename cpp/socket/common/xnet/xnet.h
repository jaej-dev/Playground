#ifndef _XNET_H_
#define _XNET_H_

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <string>

// socket utility
class xnet_util_t {
public:
  xnet_util_t();
  ~xnet_util_t();

  static int setaddress(const char *protocol, const char *addr,
                        const char *portstr, struct sockaddr_in *sockaddr);
  static int setsockopt(int s, int level, int optname, const void *flag,
                        socklen_t flaglen);
  static ssize_t readn(int s, char *buf, ssize_t len);
  static ssize_t read(int s, char *buf, ssize_t len);
  static ssize_t recvfrom(int s, char *buf, ssize_t len);
  static ssize_t writen(int s, const char *buf, ssize_t len);
  static ssize_t sendto(int s, const char *buf, ssize_t len,
                        const struct sockaddr *to, socklen_t tolen);
  static int socket(const char *protocol);
  static int bind(int s, struct sockaddr_in *sockaddr);
  static int listen(int s);
  static int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
  static int connect(int s, struct sockaddr_in *sockaddr);
  // if connectnb goes wrong, 's' is closed (close()).
  static int connectnb(int s, struct sockaddr_in *sockaddr, int msec);
  static void close(int s);
  static bool messageTimedWait(int s, int usec);
};

class xnet_t {
public:
  static const int DEFAULT_CONNECT_TIMEOUT;
  static const int DEFAULT_ACCEPT_TIMEOUT;
  static const int DEFAULT_READ_TIMEOUT;
  static const char *PROTOCOL;

  void setEndPoint(const std::string &address, const std::string &port);
  void setPort(const std::string &port);
  const std::string getAddress();
  const std::string getPort();
  int connect();
  int accept();
  ssize_t read(void *buffer, size_t bufferSize);
  ssize_t readn(void *buffer, size_t bufferSize);
  ssize_t write(const void *buffer, size_t bufferSize);
  void close();

  void setConnectTimeout(int timeInMillisec);
  void setAcceptTimeout(int timeInMillisec);
  void setReadTimeout(int timeInMillisec);
  void setNoDelayOption(bool noDelay);

  xnet_t();
  ~xnet_t();

  bool isConnected();

private:
  enum Side { SERVER = 0, CLIENT, SIDE_UNDEFINED };

  int connectTimeout_;
  int acceptTimeout_;
  int readTimeout_;
  int s_;
  int conn_;
  bool connected_;
  Side side_;
  std::string address_;
  std::string port_;
};

#endif // _XNET_H_
