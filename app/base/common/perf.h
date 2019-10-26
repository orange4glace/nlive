#ifndef NLIVE_PERF_H_
#define NLIVE_PERF_H_

#include <chrono>
#include <QDebug>

#define MEASURE_PERF_TIME(CODE) \
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now(); \
  CODE \
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start; \
  qDebug() << "Measured perf time = " <<  sec.count();

#endif