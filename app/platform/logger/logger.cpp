#include "platform/logger/logger.h"

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/null_sink.h>

void registerLoggers() {
  auto console = spdlog::stdout_color_mt(LOGGER_DEFAULT);
}

void registerNullLoggers() {
  auto console = spdlog::create<spdlog::sinks::null_sink_st>("null_logger");
}