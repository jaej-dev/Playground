/*
 * Title      :   modelx_signal.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _MODELX_SIGNAL_H_
#define _MODELX_SIGNAL_H_

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <xlog/xlog.h>
#include "xcfg/xcfg.h"

namespace tesla 
{

inline int32_t gen_signal_term() {
  std::stringstream ss;
  ss << "kill -TERM " << getpid();
  return system(ss.str().c_str());
}

inline int32_t gen_signal_usr1() {
  std::stringstream ss;
  ss << "kill -USR1 " << getpid();
  return system(ss.str().c_str());
}

inline int32_t gen_signal_usr2() {
  std::stringstream ss;
  ss << "kill -USR2 " << getpid();
  std::cerr << "\n";
  xlogf << "Critical error has occur, modelx shutdown enforced...";
  std::cerr << "\n";
  sleep(1);

  return system(ss.str().c_str());
}


} // !namespace tesla

#endif // !_MODELX_SIGNAL_H_
