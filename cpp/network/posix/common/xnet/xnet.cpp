#include "xnet/xnet.h"

const int xnet_t::DEFAULT_CONNECT_TIMEOUT = 0; // ms, 0 means no timeout
const int xnet_t::DEFAULT_ACCEPT_TIMEOUT = 0;  // ms, 0 means no timeout
const int xnet_t::DEFAULT_READ_TIMEOUT = 0;    // ms, 0 means no timeout
const char *xnet_t::PROTOCOL = "tcp";

xnet_t::xnet_t()
    : connectTimeout_(DEFAULT_CONNECT_TIMEOUT),
      acceptTimeout_(DEFAULT_ACCEPT_TIMEOUT),
      readTimeout_(DEFAULT_READ_TIMEOUT), s_(-1), conn_(-1), connected_(),
      side_(SIDE_UNDEFINED), address_(), port_() {}

xnet_t::~xnet_t() {}

void xnet_t::setEndPoint(const std::string &address, const std::string &port) {
  address_ = address;
  port_ = port;
}

void xnet_t::setPort(const std::string &port) { port_ = port; }

const std::string xnet_t::getAddress() { return address_; }

const std::string xnet_t::getPort() { return port_; }

int xnet_t::connect() {
  if (s_ == -1) {
    s_ = xnet_util_t::socket(PROTOCOL);
    if (s_ == -1) {
      return -1;
    }
  }

  struct sockaddr_in sockaddr;
  int ret = xnet_util_t::setaddress(PROTOCOL, address_.c_str(), port_.c_str(),
                                    &sockaddr);
  if (ret == 0) {
    if (connectTimeout_ == DEFAULT_CONNECT_TIMEOUT) {
      ret = xnet_util_t::connect(s_, &sockaddr);
      if (ret == 0) {
        connected_ = true;
        side_ = CLIENT;
        return 0;
      } else {
        return -1;
      }
    } else {
      ret = xnet_util_t::connectnb(s_, &sockaddr, connectTimeout_);
      if (ret == 0) {
        connected_ = true;
        side_ = CLIENT;
        return 0;
      } else {
        return -1;
      }
    }
  } else {
    return -1;
  }
}

int xnet_t::accept() {
  if (s_ == -1) {
    s_ = xnet_util_t::socket(PROTOCOL);
    if (s_ == -1) {
      return -1;
    }
  }

  int ret;
  int flag = 1;
  ret = xnet_util_t::setsockopt(s_, SOL_SOCKET, SO_REUSEADDR, &flag,
                                sizeof(flag));
  if (ret != 0) {
    return -1;
  }

  struct sockaddr_in sockaddr;
  ret = xnet_util_t::setaddress(PROTOCOL, NULL, port_.c_str(), &sockaddr);
  if (ret == 0) {
    struct sockaddr_in peer;
    socklen_t peerlen = sizeof(peer);

    errno = 0;
    ret = xnet_util_t::bind(s_, &sockaddr);
    // EINVAL: the socket is already bound to an address.
    if ((ret != 0) && (errno != EINVAL)) {
      return -1;
    }

    ret = xnet_util_t::listen(s_);
    if (ret != 0) {
      return -1;
    }

    if (acceptTimeout_ == DEFAULT_ACCEPT_TIMEOUT) {
      conn_ = xnet_util_t::accept(
          s_, reinterpret_cast<struct sockaddr *>(&peer), &peerlen);
      if (conn_ >= 0) {
        connected_ = true;
        side_ = SERVER;
        return 0;
      } else {
        return -1;
      }
    } else {
      if (xnet_util_t::messageTimedWait(s_, acceptTimeout_ * 1000)) {
        conn_ = xnet_util_t::accept(
            s_, reinterpret_cast<struct sockaddr *>(&peer), &peerlen);
        if (conn_ >= 0) {
          connected_ = true;
          side_ = SERVER;
          return 0;
        } else {
          return -1;
        }
      } else {
        return -1;
      }
    }
  } else {
    return -1;
  }
}

ssize_t xnet_t::read(void *buffer, size_t bufferSize) {
  int s;
  if (side_ == SERVER) {
    s = conn_;
  } else if (side_ == CLIENT) {
    s = s_;
  } else {
    return -1;
  }

  if (readTimeout_ == DEFAULT_READ_TIMEOUT) {
    return xnet_util_t::read(s, (char *)buffer, bufferSize);
  } else {
    if (xnet_util_t::messageTimedWait(s_, readTimeout_ * 1000)) {
      return xnet_util_t::read(s, (char *)buffer, bufferSize);
    } else {
      return -1;
    }
  }
}

ssize_t xnet_t::readn(void *buffer, size_t bufferSize) {
  int s;
  if (side_ == SERVER) {
    s = conn_;
  } else if (side_ == CLIENT) {
    s = s_;
  } else {
    return -1;
  }

  if (readTimeout_ == DEFAULT_READ_TIMEOUT) {
    return xnet_util_t::readn(s, (char *)buffer, bufferSize);
  } else {
    if (xnet_util_t::messageTimedWait(s_, readTimeout_ * 1000)) {
      return xnet_util_t::readn(s, (char *)buffer, bufferSize);
    } else {
      return -1;
    }
  }
}

ssize_t xnet_t::write(const void *buffer, size_t bufferSize) {
  int s;
  if (side_ == SERVER) {
    s = conn_;
  } else if (side_ == CLIENT) {
    s = s_;
  } else {
    return -1;
  }
  return xnet_util_t::writen(s, (char *)buffer, bufferSize);
}

void xnet_t::setConnectTimeout(int timeInMillisec) {
  connectTimeout_ = timeInMillisec;
}

void xnet_t::setAcceptTimeout(int timeInMillisec) {
  acceptTimeout_ = timeInMillisec;
}

void xnet_t::setReadTimeout(int timeInMillisec) {
  readTimeout_ = timeInMillisec;
}

void xnet_t::close() {
  if (side_ == SERVER) {
    xnet_util_t::close(s_);
    xnet_util_t::close(conn_);
  } else if (side_ == CLIENT) {
    xnet_util_t::close(s_);
  }
  s_ = -1;
  conn_ = -1;
  connected_ = false;
}

void xnet_t::setNoDelayOption(bool noDelayOption) {
  int s;
  if (side_ == SERVER) {
    s = conn_;
  } else if (side_ == CLIENT) {
    s = s_;
  } else {
    return;
  }

  int flag;
  if (noDelayOption) {
    flag = 1;
  } else {
    flag = 0;
  }

  (void)xnet_util_t::setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &flag,
                                sizeof(flag));
  // ignore the result of Setsockopt.
}

bool xnet_t::isConnected() { return connected_; }
