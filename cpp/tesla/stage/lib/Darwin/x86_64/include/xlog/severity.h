/*
 * Title      :   severity.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

namespace xlog 
{

enum severity_e {
  fatal   = 0,
  error,
  warning,
  info,
  debug,
  verbose,
  none
};

} // namespace xlog
