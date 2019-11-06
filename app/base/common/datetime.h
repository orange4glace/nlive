#ifndef NLIVE_DATETIME_H_
#define NLIVE_DATETIME_H_

#include <QString>
#include <stdint.h>
#include <string>
#include <sstream>

namespace nlive {

namespace datetime {

inline std::string secondsToHHmmss(int64_t seconds) {
  int HH = seconds / 3600;
  seconds -= HH * 3600;
  int mm = seconds / 60;
  seconds -= mm * 60;
  int ss = seconds;
  std::stringstream sst;
  sst << HH << ":" << mm << ":" << ss;
  return sst.str();
}

}

}

#endif